#pragma once
#include "Core/CoreMinimal.h"
#include "Core/Misc/NonCopyable.h"
#include "Core/RHI/D3D12RHIDefinitions.h"
#include "imgui.h"

using Microsoft::WRL::ComPtr;

class GUI : public NonCopyable
{
public:
    GUI();
    ~GUI();

private:
    const uint32 _MaxDescriptorNum = 256;
    
    ComPtr<ID3D12DescriptorHeap> _DescriptorHeap;
    ComPtr<ID3D12CommandAllocator> _CommandAllocator[GD3D12RHINumBackBuffers];
    ComPtr<ID3D12GraphicsCommandList> _GraphicsCommandList;

public:
    void Initialize(ID3D12Device* device);
};
 