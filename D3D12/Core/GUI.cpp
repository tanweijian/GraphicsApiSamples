#include "GUI.h"

GUI::GUI()
{
}

GUI::~GUI()
{
}

void GUI::Initialize(ID3D12Device* device)
{
    HRESULT hr = E_FAIL;
    // Create Descriptor Heap
    {
        D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
        heapDesc.NumDescriptors = _MaxDescriptorNum;
        heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        hr = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(_DescriptorHeap.GetAddressOf()));
        if (FAILED(hr))
        {
            LOG_F("Failed to create descriptor heap for GUI. HRESULT: 0x{:08X}", hr);
        }
    }

    // Create Command Allocators
    {
        for (int i = 0; i < GD3D12RHINumBackBuffers; i++)
        {
            hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(_CommandAllocator[i].GetAddressOf()));
            if (FAILED(hr))
            {
                LOG_F("Failed to create command allocator {} for GUI. HRESULT: 0x{:08X}", i, hr);
            }
        }
    }

    // Create Graphics Command List
    {
        hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _CommandAllocator[0].Get(), nullptr, IID_PPV_ARGS(_GraphicsCommandList.GetAddressOf()));
        if (FAILED(hr))
        {
            LOG_F("Failed to create graphics command list for GUI. HRESULT: 0x{:08X}", hr);
        }
        else
        {
            hr = _GraphicsCommandList->Close();
            if (FAILED(hr))
            {
                LOG_F("Failed to close graphics command list for GUI. HRESULT: 0x{:08X}", hr);
            }
        }
    }
}
