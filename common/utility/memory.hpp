#pragma once
#include "common_core.hpp"

namespace Common::Utility {
	class Memory final {
	public:
		class Allocator final {
		public:
			~Allocator();

			void Clear();
			void Free(void* data);
			void Free(const void* data);
			void* Allocate(size_t length);

			template <typename T>
			T* Allocate() {
				return this->AllocateArray<T>(1);
			}

			template <typename T>
			T* AllocateArray(const size_t count = 1) {
				return static_cast<T*>(this->Allocate(count * sizeof(T)));
			}

			bool Empty() const;
			char* DuplicateString(const std::string& string);
		private:
			std::mutex m_Mutex;
			std::vector<void*> m_Pool;
		};

		static void* Allocate(size_t length);

		template <typename T>
		static T* Allocate() {
			return AllocateArray<T>(1);
		}

		template <typename T>
		static T* AllocateArray(const size_t count = 1) {
			return static_cast<T*>(Allocate(count * sizeof(T)));
		}

		static char* DuplicateString(const std::string& string);

		static void Free(void* data);
		static void Free(const void* data);

		static bool IsSet(const void* mem, char chr, size_t length);

		static bool IsBadReadPtr(const void* ptr);
		static bool IsBadCodePtr(const void* ptr);
		static bool IsRDataPtr(void* ptr);

		static Allocator* GetAllocator();

		template <typename A, typename V>
		static inline void SafeMemSet(A* address, V value, std::uint32_t size = sizeof(V)) {
			DWORD oldProtect;
			VirtualProtect(static_cast<void*>(address), size, PAGE_EXECUTE_READWRITE, &oldProtect);
			memcpy(static_cast<void*>(address), static_cast<const void*>(&value), size);
			VirtualProtect(static_cast<void*>(address), size, oldProtect, &oldProtect);
		}
	private:
		static inline Allocator m_MemAllocator{};
	};
}
