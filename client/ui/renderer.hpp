#pragma once
#include "common.hpp"
#include <imgui.h>

#define REL(o)       \
	o->Release();    \
	if (o)  {        \
		o = nullptr; \
	}

namespace Client::UI {
	using namespace Microsoft::WRL;
	using RendererCallBack = std::function<void()>;
	using WindowProcedureCallback = std::function<void(HWND, UINT, WPARAM, LPARAM)>;

	struct FrameContext {
		ID3D12CommandAllocator* m_CommandAllocator;
		ID3D12Resource* m_Resource;
		D3D12_CPU_DESCRIPTOR_HANDLE m_Descriptor;
		UINT64 m_FenceValue;
	};

	class Renderer final {
	private:
		Renderer() {}

	public:
		~Renderer() {}

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;

		static void Destroy() {
			GetInstance().DestroyImpl();
		}

		static bool Init() {
			return GetInstance().InitImpl();
		}

		/**
		 * @brief Add a callback function to draw using ImGui
		 *
		 * @param callback Callback function
		 * @param priority Low values will be drawn before higher values.
		 * @return true Successfully added callback.
		 * @return false Duplicate render priority was given.
		 */
		static bool AddRendererCallBack(RendererCallBack&& callback, std::uint32_t priority) {
			return GetInstance().AddRendererCallBackImpl(std::move(callback), priority);
		}

		/**
		 * @brief Add a callback function to handle Windows WindowProcedure
		 *
		 * @param callback Callback function
		 */
		static void AddWindowProcedureCallback(WindowProcedureCallback&& callback) {
			GetInstance().AddWindowProcedureCallbackImpl(std::move(callback));
		}

		static void DX12OnPresent() {
			GetInstance().DX12OnPresentImpl();
		}

		static LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
			return GetInstance().WndProcImpl(hwnd, msg, wparam, lparam);
		}

		static void WaitForLastFrame();
		static void WaitForNextFrame();

		static void DX12PreResize();
		static void DX12PostResize();

		static bool IsResizing() {
			return GetInstance().m_Resizing;
		}

		static void SetResizing(const bool& status) {
			GetInstance().m_Resizing = status;
		}

		static void Resize(float scale) {
			GetInstance().ResizeImpl(scale);
		}

	private:
		static void DX12NewFrame();
		static void DX12EndFrame();

	private:
		void DestroyImpl();
		bool InitImpl();

		bool InitDX12();

		void ResizeImpl(float scale);

		bool AddRendererCallBackImpl(RendererCallBack&& callback, std::uint32_t priority);
		void AddWindowProcedureCallbackImpl(WindowProcedureCallback&& callback);

		void DX12OnPresentImpl();

		LRESULT WndProcImpl(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

		static Renderer& GetInstance() {
			static Renderer i{};
			return i;
		}

	private:
		bool m_Resizing = false;

		// Shared
		ImFontAtlas m_FontAtlas;

		// DX12
		std::vector<FrameContext> m_FrameContext;

		DXGI_SWAP_CHAIN_DESC m_SwapChainDesc;
		ComPtr<IDXGISwapChain1> m_GameSwapChain;
		ComPtr<IDXGISwapChain3> m_SwapChain;
		ComPtr<ID3D12Device> m_Device;
		ComPtr<ID3D12CommandQueue> m_CommandQueue;
		ComPtr<ID3D12CommandAllocator> m_CommandAllocator;
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;
		ComPtr<ID3D12DescriptorHeap> m_BackbufferDescriptorHeap;
		ComPtr<ID3D12DescriptorHeap> m_DescriptorHeap;

		ComPtr<ID3D12Fence> m_Fence;
		HANDLE m_FenceEvent;
		UINT64 m_FenceLastSignaledValue;
		HANDLE m_SwapChainWaitableObject;
		UINT64 m_FrameIndex;

	private:
		// Other
		std::map<std::uint32_t, RendererCallBack> m_RendererCallBacks;
		std::vector<WindowProcedureCallback> m_WindowProcedureCallbacks;
	};
}

// Make our linker aware of the ImGui WndProcHandler
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
