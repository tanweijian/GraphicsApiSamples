#include <assert.h>
#include "D3D12RHI.h"
#include "Core/Log/Log.h"

std::vector<std::shared_ptr<D3D12Adapter>> _ChosenAdapters;

void FindAdapter()
{
    assert(_ChosenAdapters.size() == 0);

    HRESULT hr = E_FAIL;

    ComPtr<IDXGIFactory4> factory4;
    hr = CreateDXGIFactory(IID_PPV_ARGS(factory4.GetAddressOf()));
    if (FAILED(hr))
    {
        LOG_F("Failed to create DXGI Factory. hr=0x%08X", hr);
    }
}
