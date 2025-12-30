#pragma once

#include <d3dx12/d3dx12.h>

class D3D12Capabilities
{
private:
    CD3DX12FeatureSupport _featureSupport;

public:
    D3D12Capabilities();

private:
    bool _SupportsRayTracing;
    bool _SupportsWorkGraphs;
    bool _SupportsMeshShaders;
    bool _SupportsSamplerFeedback;
    bool _SupportsVariableRateShading;

public:
    void Initialize(ID3D12Device* device);
    bool SupportsRayTracing() const { return _SupportsRayTracing; }
    bool SupportsWorkGraphs() const { return _SupportsWorkGraphs; }
    bool SupportsMeshShaders() const { return _SupportsMeshShaders; }
    bool SupportsSamplerFeedback() const { return _SupportsSamplerFeedback; }
    bool SupportsVariableRateShading() const { return _SupportsVariableRateShading; }
};