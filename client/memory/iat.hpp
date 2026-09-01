#pragma once
#include "common.hpp"

#include <MinHook.h>

#include "utility/nt.hpp"

namespace Client::Memory {
	class IAT {
	public:
		IAT(Common::Utility::NT::Library library, std::string targetLibrary, std::string function)
			: m_Library(library)
			, m_TargetLibrary(targetLibrary)
			, m_Function(function)
		{}

		IAT(std::string targetLibrary, std::string function)
			: IAT(Common::Utility::NT::Library(), targetLibrary, function)
		{}

		bool Eligible() {
			if (!this->m_Library.IsValid()) {
				return false;
			}
			this->m_IATEntry = this->m_Library.GetIATEntry(this->m_TargetLibrary, this->m_Function);
			if (!this->m_IATEntry) {
				return false;
			}
			return true;
		}

		template <typename T>
		void Hook() {
			if (!this->Eligible()) {
				return;
			}

			DWORD protect;
			VirtualProtect(reinterpret_cast<void*>(this->m_IATEntry), sizeof(*this->m_IATEntry), PAGE_EXECUTE_READWRITE, &protect);
			this->m_Original = *this->m_IATEntry;
			T::m_Original = reinterpret_cast<decltype(T::m_Original)>(this->m_Original);
			*this->m_IATEntry = T::hkCallback;
			VirtualProtect(reinterpret_cast<void*>(this->m_IATEntry), sizeof(*this->m_IATEntry), protect, &protect);

			LOG("IAT", DEBUG, "Hooked {}.", T::GetName());
		}

		void Unhook() {
			if (!this->Eligible()) {
				return;
			}

			DWORD protect;
			VirtualProtect(reinterpret_cast<void*>(this->m_IATEntry), sizeof(*this->m_IATEntry), PAGE_EXECUTE_READWRITE, &protect);
			*this->m_IATEntry = this->m_Original;
			VirtualProtect(reinterpret_cast<void*>(this->m_IATEntry), sizeof(*this->m_IATEntry), protect, &protect);
		}
	private:
		Common::Utility::NT::Library m_Library;
		std::string m_TargetLibrary;
		std::string m_Function;

		void** m_IATEntry = nullptr;
		void* m_Original = nullptr;
	};
}
