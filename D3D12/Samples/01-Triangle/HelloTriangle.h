#pragma once

#include "Core/Application.h"
#include "Core/RHI/D3D12Capabilities.h"

using Microsoft::WRL::ComPtr;

class HelloTriangle : public Application
{
public:
    HelloTriangle();
    ~HelloTriangle();

protected:
    void OnInit() override;
    void OnUpdate() override;
    void OnDraw() override;
    void OnExit() override;

private:
    void GraphicsPipelineInit();
    void LoadAssets();
    void PopulateCommandList();
    void ExecuteCommandList();
    void Display();
    void WaitForPreviousFrame();

private:
    D3D12Capabilities _Capabilities;

    ComPtr<ID3D12Device> _D3D12Device;
    ComPtr<ID3D12CommandQueue> _D3D12GraphicsQueue;
    ComPtr<ID3D12CommandAllocator> _D3D12CommandAllocator;
    ComPtr<ID3D12GraphicsCommandList> _D3D12CommandList;

    ComPtr<IDXGISwapChain> _SwapChain;
    ComPtr<ID3D12Resource> _BackBuffers[GD3D12RHINumBackBuffers];
    ComPtr<ID3D12DescriptorHeap> _RTVHeap;
    UINT _RTVDescriptorSize;

    ComPtr<ID3D12RootSignature> _D3D12RootSignature;
    ComPtr<ID3D12PipelineState> _D3D12PipelineState;

    CD3DX12_VIEWPORT _Viewport;
    CD3DX12_RECT _ScissorRect;

    UINT64 _FenceValue;
    HANDLE _FenceEvent;
    ComPtr<ID3D12Fence> _D3D12Fence;

private:
    struct Vertex
    {
        Vector3 Position;
        Vector4 Color;
    };

    Vertex _triangleMesh[3] =
    {
        { Vector3( 0.0f,  0.5f, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
        { Vector3( 0.3f, -0.5f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
        { Vector3(-0.3f, -0.5f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f) },
    };

    UINT _currentBackBufferIndex;
    ComPtr<ID3D12Resource> _triangleMeshBuffer;
    D3D12_VERTEX_BUFFER_VIEW _triangleMeshBufferView;
};