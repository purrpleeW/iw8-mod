#include "common.hpp"
#include "game/game.hpp"
#include "ui/renderer.hpp"

#include <backends/imgui_impl_dx12.h>
#include <backends/imgui_impl_win32.h>

namespace Client::UI {
	namespace {
		HWND GetIW8Window() {
			return FindWindow(TEXT("IW8"), nullptr);
		}
	}

	void Renderer::DestroyImpl() {
		ImGui_ImplWin32_Shutdown();
		WaitForLastFrame();
		ImGui_ImplDX12_Shutdown();

		ImGui::DestroyContext();
	}

	bool Renderer::InitDX12() {
		if (!g_Pointers->m_Unk_D3D12_SwapChain) {
			LOG("Renderer", WARN, "SwapChain pointer is invalid!");
			return false;
		}

		if (!g_Pointers->m_Unk_D3D12_CommandQueue) {
			LOG("Renderer", WARN, "CommandQueue pointer is invalid!");
			return false;
		}

		// This is required. In order to stop ComPtr from releasing the original pointer, we create a new ComPtr with the ptr as the intializer.
		// (The '=' operator uses swap which releases the object passed into it)
		if (this->m_GameSwapChain = ComPtr<IDXGISwapChain1>(*g_Pointers->m_Unk_D3D12_SwapChain); !this->m_GameSwapChain.Get()) {
			LOG("Renderer", WARN, "Dereferenced SwapChain pointer is invalid!");
			return false;
		}

		if (this->m_CommandQueue = ComPtr<ID3D12CommandQueue>(*g_Pointers->m_Unk_D3D12_CommandQueue); !this->m_CommandQueue.Get()) {
			LOG("Renderer", WARN, "Dereferenced CommandQueue pointer is invalid!");
			return false;
		}

		this->m_GameSwapChain.As(&this->m_SwapChain); // We need GetCurrentBackBufferIndex from IDXGISwapChain3

		if (const auto result = this->m_SwapChain->GetDevice(IID_PPV_ARGS(this->m_Device.GetAddressOf())); result < 0) {
			LOG("Renderer", WARN, "Failed to get D3D Device with result: [{}]", result);
			return false;
		}

		if (const auto result = this->m_SwapChain->GetDesc(&this->m_SwapChainDesc); result < 0) {
			LOG("Renderer", WARN, "Failed to get SwapChain Description with result: [{}]", result);
			return false;
		}

		if (const auto result = this->m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(this->m_Fence.GetAddressOf())); result < 0) {
			LOG("Renderer", WARN, "Failed to create Fence with result: [{}]", result);
			return false;
		}

		if (const auto result = this->m_FenceEvent = CreateEventA(nullptr, FALSE, FALSE, nullptr); !result) {
			LOG("Renderer", WARN, "Failed to create Fence Event!");
			return false;
		}

		this->m_FrameContext.resize(this->m_SwapChainDesc.BufferCount);

		D3D12_DESCRIPTOR_HEAP_DESC descriptorDesc{
			D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
			this->m_SwapChainDesc.BufferCount,
			D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
		};
		if (const auto result = this->m_Device->CreateDescriptorHeap(&descriptorDesc, IID_PPV_ARGS(this->m_DescriptorHeap.GetAddressOf()));
			result < 0)
		{
			LOG("Renderer", WARN, "Failed to create Descriptor Heap with result: [{}]", result);
			return false;
		}

		if (const auto result = this->m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(this->m_CommandAllocator.GetAddressOf()));
			result < 0)
		{
			LOG("Renderer", WARN, "Failed to create Command Allocator with result: [{}]", result);
			return false;
		}

		for (size_t i{}; i < this->m_SwapChainDesc.BufferCount; ++i) {
			this->m_FrameContext[i].m_CommandAllocator = this->m_CommandAllocator.Get();
		}

		if (const auto result = this->m_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, this->m_CommandAllocator.Get(), NULL,
			IID_PPV_ARGS(this->m_CommandList.GetAddressOf())); result < 0)
		{
			LOG("Renderer", WARN, "Failed to create Command List with result: [{}]", result);
			return false;
		}

		if (const auto result = this->m_CommandList->Close(); result < 0) {
			LOG("Renderer", WARN, "Failed to finalize the creation of Command List with result: [{}]", result);
			return false;
		}

		D3D12_DESCRIPTOR_HEAP_DESC descriptorBackbufferDesc{
			D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
			this->m_SwapChainDesc.BufferCount,
			D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
			1
		};

		if (const auto result = this->m_Device->CreateDescriptorHeap(&descriptorBackbufferDesc, IID_PPV_ARGS(this->m_BackbufferDescriptorHeap.GetAddressOf()));
			result < 0)
		{
			LOG("Renderer", WARN, "Failed to create Backbuffer Descriptor Heap with result: [{}]", result);
			return false;
		}

		const auto RTVDescriptorSize{ this->m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV) };
		D3D12_CPU_DESCRIPTOR_HANDLE RTVHandle{ this->m_BackbufferDescriptorHeap->GetCPUDescriptorHandleForHeapStart() };
		for (UINT i{}; i != this->m_SwapChainDesc.BufferCount; ++i) {
			ComPtr<ID3D12Resource> BackBuffer{};
			this->m_FrameContext[i].m_Descriptor = RTVHandle;
			this->m_SwapChain->GetBuffer(i, IID_PPV_ARGS(BackBuffer.GetAddressOf()));
			this->m_Device->CreateRenderTargetView(BackBuffer.Get(), nullptr, RTVHandle);
			this->m_FrameContext[i].m_Resource = BackBuffer.Get();
			RTVHandle.ptr += RTVDescriptorSize;
		}

		// never returns false, useless to check return
		ImGui::CreateContext(&GetInstance().m_FontAtlas);
		ImGui_ImplWin32_Init(GetIW8Window());
		ImGui_ImplDX12_Init(this->m_Device.Get(), this->m_SwapChainDesc.BufferCount, DXGI_FORMAT_R8G8B8A8_UNORM, this->m_DescriptorHeap.Get(),
			this->m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart(), this->m_DescriptorHeap->GetGPUDescriptorHandleForHeapStart());

		ImGui::StyleColorsDark();

		LOG("Renderer", INFO, "DirectX 12 renderer has finished initializing.");
		return true;
	}

	void Renderer::ResizeImpl(float scale) {
		DX12PreResize();

		if (scale != 1.0f) {
			ImGui::GetStyle().ScaleAllSizes(scale);
		}

		ImGui::GetStyle().MouseCursorScale = 1.0f;
		ImGui::GetIO().FontGlobalScale = scale;
		DX12PostResize();
	}

	bool Renderer::InitImpl() {
		while (!GetIW8Window()) {
			std::this_thread::sleep_for(1s);
		}

		LOG("Renderer", INFO, "Using DX12, clear shader cache if you're having issues.");
		LOG("Renderer", INFO, "Waiting...");
		std::this_thread::sleep_for(5s); // Early injection could result in errors.
		return InitDX12();
	}

	bool Renderer::AddRendererCallBackImpl(RendererCallBack&& callback, std::uint32_t priority) {
		return this->m_RendererCallBacks.insert({ priority, callback }).second;
	}

	void Renderer::AddWindowProcedureCallbackImpl(WindowProcedureCallback&& callback) {
		return this->m_WindowProcedureCallbacks.push_back(callback);
	}

	void Renderer::DX12OnPresentImpl() {
		Renderer::DX12NewFrame();
		for (const auto& callback : this->m_RendererCallBacks | std::views::values) {
			callback();
		}
		Renderer::DX12EndFrame();
	}

	LRESULT Renderer::WndProcImpl(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) 	{
		for (const auto& callback : this->m_WindowProcedureCallbacks) {
			callback(hwnd, msg, wparam, lparam);
		}

		return ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
	}

	void Renderer::WaitForLastFrame() {
		FrameContext frameCtx = GetInstance().m_FrameContext[GetInstance().m_FrameIndex % GetInstance().m_SwapChainDesc.BufferCount];
		UINT64 fenceValue = frameCtx.m_FenceValue;

		if (fenceValue == 0) {
			return;
		}

		frameCtx.m_FenceValue = 0;

		if (GetInstance().m_Fence->GetCompletedValue() >= fenceValue) {
			return;
		}

		GetInstance().m_Fence->SetEventOnCompletion(fenceValue, GetInstance().m_FenceEvent);
		WaitForSingleObject(GetInstance().m_FenceEvent, INFINITE);
	}

	void Renderer::WaitForNextFrame() {
		UINT64 nextFrameIndex = GetInstance().m_FrameIndex + 1;
		GetInstance().m_FrameIndex = nextFrameIndex;

		HANDLE waitableObjects[] = { GetInstance().m_SwapChainWaitableObject, nullptr };
		DWORD numWaitableObjects = 1;

		FrameContext frameCtx = GetInstance().m_FrameContext[nextFrameIndex % GetInstance().m_SwapChainDesc.BufferCount];
		UINT64 fenceValue = frameCtx.m_FenceValue;
		if (fenceValue != 0) { // means no fence was signaled
			frameCtx.m_FenceValue = 0;
			GetInstance().m_Fence->SetEventOnCompletion(fenceValue, GetInstance().m_FenceEvent);
			waitableObjects[1] = GetInstance().m_FenceEvent;
			numWaitableObjects = 2;
		}

		WaitForMultipleObjects(numWaitableObjects, waitableObjects, TRUE, INFINITE);
	}

	void Renderer::DX12PreResize() {
		SetResizing(true);
		WaitForLastFrame();
		ImGui_ImplDX12_InvalidateDeviceObjects();

		for (size_t i{}; i != GetInstance().m_SwapChainDesc.BufferCount; ++i) {
			REL(GetInstance().m_FrameContext[i].m_Resource);
		}
	}

	void Renderer::DX12PostResize() {
		// Recreate our pointers and ImGui's
		ImGui_ImplDX12_CreateDeviceObjects();
		const auto rtvDescriptorSize{ GetInstance().m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV) };
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle{ GetInstance().m_BackbufferDescriptorHeap->GetCPUDescriptorHandleForHeapStart() };

		for (UINT i{}; i != GetInstance().m_SwapChainDesc.BufferCount; ++i) {
			ComPtr<ID3D12Resource> backBuffer{};
			GetInstance().m_FrameContext[i].m_Descriptor = rtvHandle;
			GetInstance().m_SwapChain->GetBuffer(i, IID_PPV_ARGS(backBuffer.GetAddressOf()));
			GetInstance().m_Device->CreateRenderTargetView(backBuffer.Get(), nullptr, rtvHandle);
			GetInstance().m_FrameContext[i].m_Resource = backBuffer.Get();
			rtvHandle.ptr += rtvDescriptorSize;
		}

		SetResizing(false);
	}

	void Renderer::DX12NewFrame() {
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void Renderer::DX12EndFrame() {
		WaitForNextFrame();

		FrameContext& currentFrameContext{
			GetInstance().m_FrameContext[GetInstance().m_SwapChain->GetCurrentBackBufferIndex()]
		};
		currentFrameContext.m_CommandAllocator->Reset();

		D3D12_RESOURCE_BARRIER barrier{
			D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
			D3D12_RESOURCE_BARRIER_FLAG_NONE,
			{{
				currentFrameContext.m_Resource,
				D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
				D3D12_RESOURCE_STATE_PRESENT,
				D3D12_RESOURCE_STATE_RENDER_TARGET
			}}
		};
		GetInstance().m_CommandList->Reset(currentFrameContext.m_CommandAllocator, nullptr);
		GetInstance().m_CommandList->ResourceBarrier(1, &barrier);
		GetInstance().m_CommandList->OMSetRenderTargets(1, &currentFrameContext.m_Descriptor, FALSE, nullptr);
		GetInstance().m_CommandList->SetDescriptorHeaps(1, GetInstance().m_DescriptorHeap.GetAddressOf());

		ImGui::Render();
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), GetInstance().m_CommandList.Get());

		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		GetInstance().m_CommandList->ResourceBarrier(1, &barrier);
		GetInstance().m_CommandList->Close();

		ID3D12CommandList* commandLists[]{
			GetInstance().m_CommandList.Get()
		};
		GetInstance().m_CommandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

		UINT64 fenceValue = GetInstance().m_FenceLastSignaledValue + 1;
		GetInstance().m_CommandQueue->Signal(GetInstance().m_Fence.Get(), fenceValue);
		GetInstance().m_FenceLastSignaledValue = fenceValue;
		currentFrameContext.m_FenceValue = fenceValue;
	}
}
