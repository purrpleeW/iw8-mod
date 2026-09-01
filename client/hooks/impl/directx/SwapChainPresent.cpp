#include "common.hpp"
#include "hooks/hook.hpp"
#include "ui/renderer.hpp"

template <>
HRESULT Client::Hook::Hooks::HK_SwapChainPresent::hkCallback(IDXGISwapChain1* _this, UINT syncInterval, UINT flags) {
	if (g_Running && !UI::Renderer::IsResizing()) {
		UI::Renderer::DX12OnPresent();
	}

	return m_Original(_this, syncInterval, flags);
}
