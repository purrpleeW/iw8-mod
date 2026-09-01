#include "common.hpp"
#include "hooks/hook.hpp"
#include "ui/renderer.hpp"

template <>
HRESULT Client::Hook::Hooks::HK_SwapChainResizeBuffers::hkCallback(IDXGISwapChain1* _this, UINT bufferCount, UINT width, UINT height,
	DXGI_FORMAT newFormat, UINT swapChainFlags)
{
	if (g_Running) {
		UI::Renderer::DX12PreResize();
		const auto result = m_Original(_this, bufferCount, width, height, newFormat, swapChainFlags);
		UI::Renderer::DX12PostResize();
		return result;
	}

	return m_Original(_this, bufferCount, width, height, newFormat, swapChainFlags);
}
