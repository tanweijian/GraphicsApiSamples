#include "D3D12Capabilities.h"


D3D12Capabilities::D3D12Capabilities() :
    _SupportsRayTracing(false),
    _SupportsWorkGraphs(false),
    _SupportsMeshShaders(false),
    _SupportsSamplerFeedback(false),
    _SupportsVariableRateShading(false)
{
}

void D3D12Capabilities::Initialize(ID3D12Device* device)
{
    _featureSupport.Init(device);
    _SupportsRayTracing = _featureSupport.RaytracingTier() != D3D12_RAYTRACING_TIER_NOT_SUPPORTED;
    _SupportsWorkGraphs = _featureSupport.WorkGraphsTier() != D3D12_WORK_GRAPHS_TIER_NOT_SUPPORTED;
    _SupportsMeshShaders = _featureSupport.MeshShaderTier() != D3D12_MESH_SHADER_TIER_NOT_SUPPORTED;
    _SupportsSamplerFeedback = _featureSupport.SamplerFeedbackTier() != D3D12_SAMPLER_FEEDBACK_TIER_NOT_SUPPORTED;
    _SupportsVariableRateShading = _featureSupport.VariableShadingRateTier() != D3D12_VARIABLE_SHADING_RATE_TIER_NOT_SUPPORTED;
}
