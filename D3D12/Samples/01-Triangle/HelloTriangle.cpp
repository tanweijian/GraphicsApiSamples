#include "HelloTriangle.h"
#include <dxgi1_6.h>
#include <dxcapi.h>
#include <d3d12shader.h>

HelloTriangle::HelloTriangle()
{
}

HelloTriangle::~HelloTriangle()
{
}

void HelloTriangle::OnInit()
{
    GraphicsPipelineInit();
    LoadAssets();
}

void HelloTriangle::OnUpdate()
{
}

void HelloTriangle::OnDraw()
{
    WaitForPreviousFrame();
    PopulateCommandList();
    ExecuteCommandList();
    Display();
}

void HelloTriangle::OnExit()
{
    WaitForPreviousFrame();
    CloseHandle(_FenceEvent);
}

void HelloTriangle::GraphicsPipelineInit()
{
    HRESULT hr = E_FAIL;

#if _DEBUG
    bool enableDebugLayer = true;
#else
    bool enableDebugLayer = options.EnableDebugLayer;
#endif
    bool enableGBV = Options.EnableGPUBasedValidation;

    if (enableDebugLayer)
    {
        ComPtr<ID3D12Debug> d3d12Debug;
        hr = D3D12GetDebugInterface(IID_PPV_ARGS(d3d12Debug.GetAddressOf()));
        if (SUCCEEDED(hr))
        {
            d3d12Debug->EnableDebugLayer();
            LOG_I("D3D12 Debug Layer enabled.");
            if (enableGBV)
            {
                ComPtr<ID3D12Debug1> d3d12Debug1;
                hr = d3d12Debug.As(&d3d12Debug1);
                if (SUCCEEDED(hr))
                {
                    d3d12Debug1->SetEnableGPUBasedValidation(TRUE);
                    LOG_I("GPU-Based Validation enabled.");
                }
                else
                {
                    LOG_F("Failed to enable GPU-Based Validation. HRESULT = 0x{:08X}", hr);
                }
            }
        }
        else
        {
            LOG_F("Failed to get D3D12 Debug Interface. HRESULT = 0x{:08X}", hr);
        }

        if (Options.UseDRED)
        {
            ComPtr<ID3D12DeviceRemovedExtendedDataSettings1> dredSettings;
            hr = D3D12GetDebugInterface(IID_PPV_ARGS(dredSettings.GetAddressOf()));
            if (SUCCEEDED(hr))
            {
                dredSettings->SetAutoBreadcrumbsEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
                dredSettings->SetPageFaultEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
                dredSettings->SetBreadcrumbContextEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
                dredSettings->SetWatsonDumpEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
                LOG_I("DRED enabled.");
            }
            else
            {
                LOG_F("Failed to enable DRED. HRESULT = 0x{:08X}", hr);
            }
        }
    }

    ComPtr<IDXGIFactory> factory;
    hr = CreateDXGIFactory2(enableDebugLayer ? DXGI_CREATE_FACTORY_DEBUG : 0, IID_PPV_ARGS(factory.GetAddressOf()));
    if (FAILED(hr))
    {
        LOG_F("Failed to create DXGI Factory. HRESULT = 0x{:08X}", hr);
    }

    ComPtr<IDXGIAdapter> adapter;
    if (Options.UseWarp)
    {
        ComPtr<IDXGIFactory4> factory4;
        hr = factory.As(&factory4);
        if (SUCCEEDED(hr))
        {
            hr = factory4->EnumWarpAdapter(IID_PPV_ARGS(adapter.GetAddressOf()));
            if (FAILED(hr))
            {
                LOG_F("Failed to enumerate WARP adapter. HRESULT = 0x{:08X}", hr);
            }
        }
    }
    else
    {
        ComPtr<IDXGIFactory6> factory6;
        hr = factory.As(&factory6);
        if (SUCCEEDED(hr))
        {
            hr = factory6->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(adapter.GetAddressOf()));
            if (FAILED(hr))
            {
                LOG_F("Failed to enumerate adapter by GPU preference. HRESULT = 0x{:08X}", hr);
            }
        }
    }

    hr = D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(_D3D12Device.GetAddressOf()));
    if (FAILED(hr))
    {
        LOG_F("Failed to create D3D12 Device. HRESULT = 0x{:08X}", hr);
    }

    _Capabilities.Initialize(_D3D12Device.Get());

    {
        D3D12_COMMAND_QUEUE_DESC queueDesc = {};
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.NodeMask = 0;
        hr = _D3D12Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(_D3D12GraphicsQueue.GetAddressOf()));
        if (FAILED(hr))
        {
            LOG_F("Failed to create D3D12 Command Queue. HRESULT = 0x{:08X}", hr);
        }
    }

    hr = _D3D12Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(_D3D12CommandAllocator.GetAddressOf()));
    if (FAILED(hr))
    {
        LOG_F("Failed to create D3D12 Command Allocator. HRESULT = 0x{:08X}", hr);
    }

    hr = _D3D12Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _D3D12CommandAllocator.Get(), nullptr, IID_PPV_ARGS(_D3D12CommandList.GetAddressOf()));
    if (FAILED(hr))
    {
        LOG_F("Failed to create D3D12 Command List. HRESULT = 0x{:08X}", hr);
    }
    else
    {
        hr = _D3D12CommandList->Close();
        if (FAILED(hr))
        {
            LOG_F("Failed to close D3D12 Command List. HRESULT = 0x{:08X}", hr);
        }
    }

    {
        Vector2 displaySize = Window.GetScreenSize();
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.Width = static_cast<UINT>(displaySize.x);
        swapChainDesc.Height = static_cast<UINT>(displaySize.y);
        swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.Stereo = FALSE;
        swapChainDesc.SampleDesc = { 1, 0 };
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
        swapChainDesc.BufferCount = GD3D12RHINumBackBuffers;
        swapChainDesc.Scaling = DXGI_SCALING_NONE;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
        swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        _Viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, displaySize.x, displaySize.y);
        _ScissorRect = CD3DX12_RECT(0, 0, static_cast<LONG>(displaySize.x), static_cast<LONG>(displaySize.y));

        ComPtr<IDXGIFactory2> factory2;
        hr = factory.As(&factory2);
        if (FAILED(hr))
        {
            LOG_F("Failed to get IDXGIFactory2. HRESULT = 0x{:08X}", hr);
        }
        ComPtr<IDXGISwapChain1> swapChain1;
        hr = factory2->CreateSwapChainForHwnd(_D3D12GraphicsQueue.Get(), Window.GetHWND(), &swapChainDesc, nullptr, nullptr, swapChain1.GetAddressOf());
        if (FAILED(hr))
        {
            LOG_F("Failed to create Swap Chain. HRESULT = 0x{:08X}", hr);
        }
        _SwapChain = swapChain1;

        factory->MakeWindowAssociation(Window.GetHWND(), DXGI_MWA_NO_WINDOW_CHANGES);

        ComPtr<IDXGISwapChain3> swapChain3;
        hr = _SwapChain.As(&swapChain3);
        if (SUCCEEDED(hr))
        {
            _currentBackBufferIndex = swapChain3->GetCurrentBackBufferIndex();
        }
    }

    {
        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
        rtvHeapDesc.NumDescriptors = GD3D12RHINumBackBuffers;
        rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        hr = _D3D12Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(_RTVHeap.GetAddressOf()));
        if (FAILED(hr))
        {
            LOG_F("Failed to create RTV Descriptor Heap. HRESULT = 0x{:08X}", hr);
        }
        _RTVDescriptorSize = _D3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(_RTVHeap->GetCPUDescriptorHandleForHeapStart());
        for (UINT i = 0; i < GD3D12RHINumBackBuffers; i++)
        {
            hr = _SwapChain->GetBuffer(i, IID_PPV_ARGS(&_BackBuffers[i]));
            if (FAILED(hr))
            {
                LOG_F("Failed to get Swap Chain Buffer {}. HRESULT = 0x{:08X}", i, hr);
            }
            _D3D12Device->CreateRenderTargetView(_BackBuffers[i].Get(), nullptr, rtvHandle);
            rtvHandle.Offset(1, _RTVDescriptorSize);
        }
    }
}

void HelloTriangle::LoadAssets()
{
    HRESULT hr = E_FAIL;
    {
        CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
        rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

        ComPtr<ID3DBlob> serializedBlob = nullptr;
        ComPtr<ID3DBlob> errorBlob = nullptr;
        hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, serializedBlob.GetAddressOf(), errorBlob.GetAddressOf());
        if (FAILED(hr))
        {
            LOG_F("Failed to serialize Root Signature {}. HRESULT = 0x{:08X}", (char*)errorBlob->GetBufferPointer(), hr);
        }
        hr = _D3D12Device->CreateRootSignature(0, serializedBlob->GetBufferPointer(), serializedBlob->GetBufferSize(), IID_PPV_ARGS(_D3D12RootSignature.GetAddressOf()));
        if (FAILED(hr))
        {
            LOG_F("Failed to create Root Signature. HRESULT = 0x{:08X}", hr);
        }
    }

    ComPtr<IDxcBlob> vsShaderCodeBlob;
    ComPtr<IDxcBlob> psShaderCodeBlob;

    {
        ComPtr<IDxcUtils> dxcUtils;
        ComPtr<IDxcCompiler3> dxcCompiler;

        hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(dxcUtils.GetAddressOf()));
        if (FAILED(hr))
        {
            LOG_F("Failed to create DxcUtils instance. HRESULT = 0x{:08X}", hr);
        }
        hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(dxcCompiler.GetAddressOf()));
        if (FAILED(hr))
        {
            LOG_F("Failed to create DxcCompiler instance. HRESULT = 0x{:08X}", hr);
        }

        {
            ComPtr<IDxcBlobEncoding> sourceCodeBlob;
            hr = dxcUtils->LoadFile(L"Assets/Shaders/ShaderVS.hlsl", nullptr, sourceCodeBlob.GetAddressOf());
            if (FAILED(hr))
            {
                LOG_F("Failed to load Vertex Shader source file. HRESULT = 0x{:08X}", hr);
            }

            LPCWSTR args[] = { L"-Qembed_debug", DXC_ARG_DEBUG, DXC_ARG_SKIP_OPTIMIZATIONS };
            ComPtr<IDxcCompilerArgs> compilerArgs;
            hr = dxcUtils->BuildArguments(L"ShaderVS", L"main", L"vs_6_0", args, _countof(args), nullptr, 0, compilerArgs.GetAddressOf());
            if (FAILED(hr))
            {
                LOG_F("Failed to build Vertex Shader compiler arguments. HRESULT = 0x{:08X}", hr);
            }

            ComPtr<IDxcResult> results = nullptr;
            DxcBuffer sourceCodeBuffer = { sourceCodeBlob->GetBufferPointer(), sourceCodeBlob->GetBufferSize(), 0 };
            hr = dxcCompiler->Compile(&sourceCodeBuffer, compilerArgs->GetArguments(), compilerArgs->GetCount(), nullptr, IID_PPV_ARGS(results.GetAddressOf()));
            if (FAILED(hr))
            {
                LOG_F("Failed to compile Vertex Shader. HRESULT = 0x{:08X}", hr);
            }
            results->GetStatus(&hr);
            if (FAILED(hr))
            {
                ComPtr<IDxcBlobUtf8> errors;
                results->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(errors.GetAddressOf()), nullptr);
                LOG_F("Failed to compile Vertex Shader. {}", errors->GetStringPointer());
            }
            results->GetResult(vsShaderCodeBlob.GetAddressOf());

            /*ComPtr<IDxcBlob> reflectionPart = nullptr;
            if (results->HasOutput(DXC_OUT_REFLECTION))
            {
                hr = results->GetOutput(DXC_OUT_REFLECTION, IID_PPV_ARGS(reflectionPart.GetAddressOf()), nullptr);
                if (FAILED(hr))
                {
                    LOG_F("Failed to get Vertex Shader reflection blob. HRESULT = 0x{:08X}", hr);
                }
            }
            else
            {
                reflectionPart = vsShaderCodeBlob;
            }

            DxcBuffer reflectionBuffer = { reflectionPart->GetBufferPointer(), reflectionPart->GetBufferSize(), 0 };
            ComPtr<ID3D12ShaderReflection> shaderReflection;
            hr = dxcUtils->CreateReflection(&reflectionBuffer, IID_PPV_ARGS(shaderReflection.GetAddressOf()));
            D3D12_SHADER_DESC shaderDesc;
            shaderReflection->GetDesc(&shaderDesc);
            for (UINT i = 0; i < shaderDesc.InputParameters; i++)
            {
                D3D12_SIGNATURE_PARAMETER_DESC paramDesc;
                shaderReflection->GetInputParameterDesc(i, &paramDesc);
            }*/
        }

        {
            ComPtr<IDxcBlobEncoding> sourceCodeBlob;
            hr = dxcUtils->LoadFile(L"Assets/Shaders/ShaderPS.hlsl", nullptr, sourceCodeBlob.GetAddressOf());
            if (FAILED(hr))
            {
                LOG_F("Failed to load Pixel Shader source file. HRESULT = 0x{:08X}", hr);
            }

            LPCWSTR args[] = { L"-Qembed_debug", DXC_ARG_DEBUG, DXC_ARG_SKIP_OPTIMIZATIONS };
            ComPtr<IDxcCompilerArgs> compilerArgs;
            hr = dxcUtils->BuildArguments(L"ShaderPS", L"main", L"ps_6_0", args, _countof(args), nullptr, 0, compilerArgs.GetAddressOf());
            if (FAILED(hr))
            {
                LOG_F("Failed to build Vertex Shader compiler arguments. HRESULT = 0x{:08X}", hr);
            }

            ComPtr<IDxcResult> results = nullptr;
            DxcBuffer sourceCodeBuffer = { sourceCodeBlob->GetBufferPointer(), sourceCodeBlob->GetBufferSize(), 0 };
            hr = dxcCompiler->Compile(&sourceCodeBuffer, compilerArgs->GetArguments(), compilerArgs->GetCount(), nullptr, IID_PPV_ARGS(results.GetAddressOf()));
            if (FAILED(hr))
            {
                LOG_F("Failed to compile Pixel Shader. HRESULT = 0x{:08X}", hr);
            }
            results->GetStatus(&hr);
            if (FAILED(hr))
            {
                ComPtr<IDxcBlobUtf8> errors;
                results->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(errors.GetAddressOf()), nullptr);
                LOG_F("Failed to compile Pixel Shader. {}", errors->GetStringPointer());
            }
            results->GetResult(psShaderCodeBlob.GetAddressOf());
        }
    }

    {
        D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        };

        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        psoDesc.pRootSignature = _D3D12RootSignature.Get();
        psoDesc.VS = { vsShaderCodeBlob->GetBufferPointer(), vsShaderCodeBlob->GetBufferSize() };
        psoDesc.PS = { psShaderCodeBlob->GetBufferPointer(), psShaderCodeBlob->GetBufferSize() };
        psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        psoDesc.SampleMask = UINT_MAX;
        psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
        psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        psoDesc.NumRenderTargets = 1;
        psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        psoDesc.SampleDesc = { 1, 0 };
        hr = _D3D12Device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(_D3D12PipelineState.GetAddressOf()));
        if (FAILED(hr))
        {
            LOG_F("Failed to create Graphics Pipeline State. HRESULT = 0x{:08X}", hr);
        }
    }

    {
        const UINT bufferSize = sizeof(_triangleMesh);
        CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
        D3D12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);
        hr = _D3D12Device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(_triangleMeshBuffer.GetAddressOf()));
        if (FAILED(hr))
        {
            LOG_F("Failed to create Triangle Mesh Buffer. HRESULT = 0x{:08X}", hr);
        }
        UINT8* meshDataBegin = nullptr;
        CD3DX12_RANGE readRange(0, 0);
        hr = _triangleMeshBuffer->Map(0, &readRange, reinterpret_cast<void**>(&meshDataBegin));
        if (FAILED(hr))
        {
            LOG_F("Failed to map Triangle Mesh Buffer. HRESULT = 0x{:08X}", hr);
        }
        memcpy(meshDataBegin, _triangleMesh, bufferSize);
        _triangleMeshBuffer->Unmap(0, nullptr);
        _triangleMeshBufferView = { _triangleMeshBuffer->GetGPUVirtualAddress(), bufferSize, sizeof(Vertex) };
    }

    {
        hr = _D3D12Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(_D3D12Fence.GetAddressOf()));
        _FenceValue = 1;
        if (FAILED(hr))
        {
            LOG_F("Failed to create D3D12 Fence. HRESULT = 0x{:08X}", hr);
        }
        _FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    }
}

void HelloTriangle::PopulateCommandList()
{
    HRESULT hr = E_FAIL;
    hr = _D3D12CommandAllocator->Reset();
    if (FAILED(hr))
    {
        LOG_F("Failed to reset D3D12 Command Allocator. HRESULT = 0x{:08X}", hr);
    }
    hr = _D3D12CommandList->Reset(_D3D12CommandAllocator.Get(), _D3D12PipelineState.Get());
    if (FAILED(hr))
    {
        LOG_F("Failed to reset D3D12 Command List. HRESULT = 0x{:08X}", hr);
    }

    _D3D12CommandList->SetGraphicsRootSignature(_D3D12RootSignature.Get());
    _D3D12CommandList->RSSetViewports(1, &_Viewport);
    _D3D12CommandList->RSSetScissorRects(1, &_ScissorRect);

    D3D12_RESOURCE_BARRIER rtBarrier = CD3DX12_RESOURCE_BARRIER::Transition(_BackBuffers[_currentBackBufferIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    _D3D12CommandList->ResourceBarrier(1, &rtBarrier);

    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(_RTVHeap->GetCPUDescriptorHandleForHeapStart(), _currentBackBufferIndex, _RTVDescriptorSize);
    _D3D12CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
    _D3D12CommandList->ClearRenderTargetView(rtvHandle, Color(0.0f, 0.0f, 0.0f, 1.0f), 0, nullptr);

    _D3D12CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    _D3D12CommandList->IASetVertexBuffers(0, 1, &_triangleMeshBufferView);
    _D3D12CommandList->DrawInstanced(3, 1, 0, 0);

    D3D12_RESOURCE_BARRIER presentBarrier = CD3DX12_RESOURCE_BARRIER::Transition(_BackBuffers[_currentBackBufferIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    _D3D12CommandList->ResourceBarrier(1, &presentBarrier);

    hr = _D3D12CommandList->Close();
    if (FAILED(hr))
    {
        LOG_F("Failed to close D3D12 Command List. HRESULT = 0x{:08X}", hr);
    }
}

void HelloTriangle::ExecuteCommandList()
{
    HRESULT hr = E_FAIL;
    ID3D12CommandList* ppCommandLists[] = { _D3D12CommandList.Get() };
    _D3D12GraphicsQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
}

void HelloTriangle::Display()
{
    HRESULT hr = E_FAIL;

    hr = _SwapChain->Present(1, 0);
    if (FAILED(hr))
    {
        LOG_F("Failed to present Swap Chain. HRESULT = 0x{:08X}", hr);
    }
}

void HelloTriangle::WaitForPreviousFrame()
{
    HRESULT hr = E_FAIL;
    const UINT64 currentFenceValue = _FenceValue;
    hr = _D3D12GraphicsQueue->Signal(_D3D12Fence.Get(), currentFenceValue);
    if (FAILED(hr))
    {
        LOG_F("Failed to signal D3D12 Fence. HRESULT = 0x{:08X}", hr);
    }
    _FenceValue++;

    // Wait until the previous frame is finished.
    if (_D3D12Fence->GetCompletedValue() < currentFenceValue)
    {
        hr = _D3D12Fence->SetEventOnCompletion(currentFenceValue, _FenceEvent);
        if (FAILED(hr))
        {
            LOG_F("Failed to set event on D3D12 Fence completion. HRESULT = 0x{:08X}", hr);
        }
        WaitForSingleObject(_FenceEvent, INFINITE);
    }

    ComPtr<IDXGISwapChain3 > swapChain3;
    hr = _SwapChain.As(&swapChain3);
    if (FAILED(hr))
    {
        LOG_F("Failed to get IDXGISwapChain3. HRESULT = 0x{:08X}", hr);
    }
    _currentBackBufferIndex = swapChain3->GetCurrentBackBufferIndex();
}
