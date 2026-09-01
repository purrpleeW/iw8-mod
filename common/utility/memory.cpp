#include "common_core.hpp"
#include "utility/memory.hpp"
#include "utility/nt.hpp"

namespace Common::Utility {
	Memory::Allocator::~Allocator() {
		this->Clear();
	}

	void Memory::Allocator::Clear() {
		std::lock_guard _(this->m_Mutex);

		for (const auto& data : this->m_Pool) {
			Memory::Free(data);
		}

		this->m_Pool.clear();
	}

	void Memory::Allocator::Free(void* data) {
		std::lock_guard _(this->m_Mutex);

		const auto j = std::find(this->m_Pool.begin(), this->m_Pool.end(), data);
		if (j != this->m_Pool.end()) {
			Memory::Free(data);
			this->m_Pool.erase(j);
		}
	}

	void Memory::Allocator::Free(const void* data) {
		this->Free(const_cast<void*>(data));
	}

	void* Memory::Allocator::Allocate(const size_t length) {
		std::lock_guard _(this->m_Mutex);

		auto* data = Memory::Allocate(length);
		this->m_Pool.push_back(data);
		return data;
	}

	bool Memory::Allocator::Empty() const {
		return this->m_Pool.empty();
	}

	char* Memory::Allocator::DuplicateString(const std::string& string) {
		std::lock_guard _(this->m_Mutex);

		auto* data = Memory::DuplicateString(string);
		this->m_Pool.push_back(data);
		return data;
	}

	void* Memory::Allocate(const size_t length) {
		return std::calloc(length, 1);
	}

	char* Memory::DuplicateString(const std::string& string) {
		auto* newString = AllocateArray<char>(string.size() + 1);
		std::memcpy(newString, string.data(), string.size());
		return newString;
	}

	void Memory::Free(void* data) {
		std::free(data);
	}

	void Memory::Free(const void* data) {
		free(const_cast<void*>(data));
	}

	bool Memory::IsSet(const void* mem, const char chr, const size_t length) {
		const auto memArr = static_cast<const char*>(mem);

		for (size_t i = 0; i < length; ++i) {
			if (memArr[i] != chr) {
				return false;
			}
		}

		return true;
	}

	bool Memory::IsBadReadPtr(const void* ptr) {
		MEMORY_BASIC_INFORMATION mbi = {};
		if (VirtualQuery(ptr, &mbi, sizeof(mbi))) {
			const DWORD mask = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ |
				PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
			auto b = !(mbi.Protect & mask);

			// check the page is not a guard page
			if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS)) {
				b = true;
			}

			return b;
		}
		return true;
	}

	bool Memory::IsBadCodePtr(const void* ptr) {
		MEMORY_BASIC_INFORMATION mbi = {};
		if (VirtualQuery(ptr, &mbi, sizeof(mbi))) {
			const DWORD mask = (PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
			auto b = !(mbi.Protect & mask);

			// check the page is not a guard page
			if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS)) {
				b = true;
			}

			return b;
		}
		return true;
	}

	bool Memory::IsRDataPtr(void* pointer) {
		const std::string rdata = ".rdata";
		const auto pointerLib = NT::Library::GetByAddress(pointer);

		for (const auto& section : pointerLib.GetSectionHeaders()) {
			const auto size = sizeof(section->Name);
			char name[size + 1];
			name[size] = 0;
			std::memcpy(name, section->Name, size);

			if (name == rdata) {
				const auto target = size_t(pointer);
				const size_t sourceStart = size_t(pointerLib.GetPtr()) + section->PointerToRawData;
				const size_t sourceEnd = sourceStart + section->SizeOfRawData;

				return target >= sourceStart && target <= sourceEnd;
			}
		}

		return false;
	}

	Memory::Allocator* Memory::GetAllocator() {
		return &Memory::m_MemAllocator;
	}
}
