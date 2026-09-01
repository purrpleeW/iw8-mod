#pragma once
#include "common.hpp"

#include <utility/nt.hpp>
#include <MinHook.h>

namespace Client::Memory {
	template <typename P = void>
	class MinHook {
	public:
		MinHook(P* pointer = nullptr)
			: m_DetourPointer(pointer)
		{}

		MinHook(const char* libName, const char* exportName) {
			this->m_DetourPointer = Common::Utility::NT::Library(libName).GetProc<void*>(exportName);
		}

		template <typename C, typename O>
		void Hook(C* callback, O** original) {
			if (this->m_DetourPointer == nullptr) {
				return;
			}

			MH_CreateHook(this->m_DetourPointer, reinterpret_cast<void*>(callback), reinterpret_cast<void**>(original));
			MH_EnableHook(this->m_DetourPointer);
		}

		template <typename T>
		void Hook() {
			if (this->m_DetourPointer == nullptr) {
				return;
			}

			MH_CreateHook(this->m_DetourPointer, &T::hkCallback, (void**)(&T::m_Original));
			MH_EnableHook(this->m_DetourPointer);

			LOG("MinHook", DEBUG, "Hooked {}.", T::GetName());
		}

		template <typename T>
		void Hook(std::uint32_t idx) {
			if (this->m_DetourPointer == nullptr) {
				return;
			}

			MH_CreateHook(VT_GET(this->m_DetourPointer, idx), &T::hkCallback, (void**)(&T::m_Original));
			MH_EnableHook(VT_GET(this->m_DetourPointer, idx));

			LOG("MinHook", DEBUG, "Hooked {}. (VT: {})", T::GetName(), idx);
		}

		void Unhook() {
			if (this->m_DetourPointer == nullptr) {
				return;
			}

			MH_DisableHook(this->m_DetourPointer);
			MH_RemoveHook(this->m_DetourPointer);
		}

		void Unhook(std::uint32_t idx) {
			if (this->m_DetourPointer == nullptr) {
				return;
			}

			MH_DisableHook(VT_GET(this->m_DetourPointer, idx));
			MH_RemoveHook(VT_GET(this->m_DetourPointer, idx));
		}
	private:
		LPVOID m_DetourPointer;
	};
}