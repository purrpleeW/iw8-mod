#pragma once
#include "common_core.hpp"
#include "utility/concurrency.hpp"
#include "utility/hook.hpp"
#include "utility/nt.hpp"
#include <MinHook.h>

#ifdef max
#	undef max
#endif
#ifdef min
#	undef min
#endif

namespace Common::Utility::Hook {
	namespace {
		uint8_t* AllocateSomewhereNear(const void* baseAddress, const size_t size) {
			size_t offset = 0;
			while (true) {
				offset += size;
				auto* target_address = static_cast<const uint8_t*>(baseAddress) - offset;
				if (IsRelativelyFar(baseAddress, target_address)) {
					return nullptr;
				}

				const auto res = VirtualAlloc(const_cast<uint8_t*>(target_address), size, MEM_RESERVE | MEM_COMMIT,
					PAGE_EXECUTE_READWRITE);
				if (res) {
					if (IsRelativelyFar(baseAddress, target_address)) {
						VirtualFree(res, 0, MEM_RELEASE);
						return nullptr;
					}

					return static_cast<uint8_t*>(res);
				}
			}
		}

		class Memory {
		public:
			Memory() = default;

			Memory(const void* ptr)
				: Memory()
			{
				this->m_Length = 0x1000;
				this->m_Buffer = AllocateSomewhereNear(ptr, this->m_Length);
				if (!this->m_Buffer) {
					throw std::runtime_error("Failed to allocate");
				}
			}

			~Memory() {
				if (this->m_Buffer) {
					VirtualFree(this->m_Buffer, 0, MEM_RELEASE);
				}
			}

			Memory(Memory&& obj) noexcept
				: Memory()
			{
				this->operator=(std::move(obj));
			}

			Memory& operator=(Memory&& obj) noexcept {
				if (this != &obj) {
					this->~Memory();
					this->m_Buffer = obj.m_Buffer;
					this->m_Length = obj.m_Length;
					this->m_Offset = obj.m_Offset;

					obj.m_Buffer = nullptr;
					obj.m_Length = 0;
					obj.m_Offset = 0;
				}

				return *this;
			}

			void* Allocate(const size_t length) {
				if (!this->m_Buffer) {
					return nullptr;
				}

				if (this->m_Offset + length > this->m_Length) {
					return nullptr;
				}

				const auto ptr = this->GetPtr();
				this->m_Offset += length;
				return ptr;
			}

			void* GetPtr() const {
				return this->m_Buffer + this->m_Offset;
			}

		private:
			uint8_t* m_Buffer{};
			size_t m_Length{};
			size_t m_Offset{};
		};

		void* GetMemoryNear(const void* address, const size_t size) {
			static Concurrency::Container<std::vector<Memory>> memoryContainer{};

			return memoryContainer.Access<void*>([&](std::vector<Memory>& memories) {
				for (auto& memory : memories) {
					if (!IsRelativelyFar(address, memory.GetPtr())) {
						const auto buffer = memory.Allocate(size);
						if (buffer) {
							return buffer;
						}
					}
				}

				memories.emplace_back(address);
				return memories.back().Allocate(size);
			});
		}

		// this is kinda done by the client anyway...
		void* InitializeMinHook() {
			static class MinHookInit {
			public:
				MinHookInit() {
					//if (MH_Initialize() != MH_OK) {
					//	throw std::runtime_error("Failed to initialize MinHook");
					//}
				}

				~MinHookInit() {
					//MH_Uninitialize();
				}
			} minHookInit;
			return &minHookInit;
		}
	}

	Detour::Detour() {
		(void)InitializeMinHook();
	}

	Detour::Detour(const size_t place, void* target)
		: Detour(reinterpret_cast<void*>(place), target)
	{}

	Detour::Detour(void* place, void* target)
		: Detour()
	{
		this->Create(place, target);
	}

	Detour::~Detour() {
		this->Clear();
	}

	void Detour::Enable() {
		MH_EnableHook(this->m_Place);
		if (!this->m_MovedData.empty()) {
			this->Move();
		}
	}

	void Detour::Disable() {
		this->UnMove();
		MH_DisableHook(this->m_Place);
	}

	void Detour::Create(void* place, void* target) {
		this->Clear();
		this->m_Place = place;

		if (MH_CreateHook(this->m_Place, target, &this->m_Original) != MH_OK) {
			//throw std::runtime_error(string::va("Unable to create hook at location: %p", this->place_));
		}

		this->Enable();
	}

	void Detour::Create(const size_t place, void* target) {
		MH_Initialize();
		this->Create(reinterpret_cast<void*>(place), target);
	}

	void Detour::Clear() {
		if (this->m_Place) {
			this->UnMove();
			MH_RemoveHook(this->m_Place);
		}

		this->m_Place = nullptr;
		this->m_Original = nullptr;
		this->m_MovedData = {};
	}

	void Detour::Move() {
		this->m_MovedData = MoveHook(this->m_Place);
	}

	void* Detour::GetPlace() const {
		return this->m_Place;
	}

	void* Detour::GetOriginal() const {
		return this->m_Original;
	}

	void Detour::UnMove() {
		if (!this->m_MovedData.empty()) {
			Copy(this->m_Place, this->m_MovedData.data(), this->m_MovedData.size());
		}
	}

	std::optional<std::pair<void*, void*>> IAT(const NT::Library& library, const std::string& targetLibrary, const std::string& process, void* stub) {
		if (!library.IsValid()) {
			return {};
		}

		auto* const ptr = library.GetIATEntry(targetLibrary, process);
		if (!ptr) {
			return {};
		}

		DWORD protect;
		VirtualProtect(ptr, sizeof(*ptr), PAGE_EXECUTE_READWRITE, &protect);

		std::swap(*ptr, stub);

		VirtualProtect(ptr, sizeof(*ptr), protect, &protect);
		return {
			{ ptr, stub }
		};
	}

	void Nop(void* place, const size_t length) {
		DWORD oldProtect{};
		VirtualProtect(place, length, PAGE_EXECUTE_READWRITE, &oldProtect);

		std::memset(place, 0x90, length);

		VirtualProtect(place, length, oldProtect, &oldProtect);
		FlushInstructionCache(GetCurrentProcess(), place, length);
	}

	void Nop(const size_t place, const size_t length) {
		Nop(reinterpret_cast<void*>(place), length);
	}

	void Copy(void* place, const void* data, const size_t length) {
		DWORD oldProtect{};
		VirtualProtect(place, length, PAGE_EXECUTE_READWRITE, &oldProtect);

		std::memmove(place, data, length);

		VirtualProtect(place, length, oldProtect, &oldProtect);
		FlushInstructionCache(GetCurrentProcess(), place, length);
	}

	void Copy(const size_t place, const void* data, const size_t length) {
		Copy(reinterpret_cast<void*>(place), data, length);
	}

	void CopyString(void* place, const char* str) {
		Copy(reinterpret_cast<void*>(place), str, strlen(str) + 1);
	}

	void CopyString(const size_t place, const char* str) {
		CopyString(reinterpret_cast<void*>(place), str);
	}

	bool IsRelativelyFar(const void* pointer, const void* data, const int offset) {
		const int64_t diff = size_t(data) - (size_t(pointer) + offset);
		const auto small_diff = int32_t(diff);
		return diff != int64_t(small_diff);
	}

	void Call(void* pointer, void* data) {
		if (IsRelativelyFar(pointer, data)) {
			auto* trampoline = GetMemoryNear(pointer, 14);
			if (!trampoline) {
				throw std::runtime_error("Too far away to create 32bit relative branch");
			}

			Call(pointer, trampoline);
			Jump(trampoline, data, true, true);
			return;
		}

		uint8_t copy_data[5];
		copy_data[0] = 0xE8;
		*reinterpret_cast<int32_t*>(&copy_data[1]) = int32_t(size_t(data) - (size_t(pointer) + 5));

		auto* patch_pointer = PBYTE(pointer);
		Copy(patch_pointer, copy_data, sizeof(copy_data));
	}

	void Call(const size_t pointer, void* data) {
		return Call(reinterpret_cast<void*>(pointer), data);
	}

	void Call(const size_t pointer, const size_t data) {
		return Call(pointer, reinterpret_cast<void*>(data));
	}

	void Jump(void* pointer, void* data, const bool useFar, const bool useSafe) {
		static const unsigned char jumpData[] = {
			0x48, 0xb8, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0xff, 0xe0
		};

		static const unsigned char jumpDataSafe[] = {
			0xFF, 0x25, 0x00, 0x00, 0x00, 0x00
		};

		if (!useFar && IsRelativelyFar(pointer, data)) {
			auto* trampoline = GetMemoryNear(pointer, 14);
			if (!trampoline) {
				throw std::runtime_error("Too far away to create 32bit relative branch");
			}
			Jump(pointer, trampoline, false, false);
			Jump(trampoline, data, true, true);
			return;
		}

		auto* patchPointer = PBYTE(pointer);

		if (useFar) {
			if (useSafe) {
				uint8_t copyData[sizeof(jumpDataSafe) + sizeof(data)];
				memcpy(copyData, jumpDataSafe, sizeof(jumpDataSafe));
				memcpy(copyData + sizeof(jumpDataSafe), &data, sizeof(data));

				Copy(patchPointer, copyData, sizeof(copyData));
			}
			else {
				uint8_t copyData[sizeof(jumpData)];
				memcpy(copyData, jumpData, sizeof(jumpData));
				memcpy(copyData + 2, &data, sizeof(data));

				Copy(patchPointer, copyData, sizeof(copyData));
			}
		}
		else {
			uint8_t copyData[5];
			copyData[0] = 0xE9;
			*reinterpret_cast<int32_t*>(&copyData[1]) = int32_t(size_t(data) - (size_t(pointer) + 5));

			Copy(patchPointer, copyData, sizeof(copyData));
		}
	}

	void Jump(const size_t pointer, void* data, const bool useFar, const bool useSafe) {
		return Jump(reinterpret_cast<void*>(pointer), data, useFar, useSafe);
	}

	void Jump(const size_t pointer, const size_t data, const bool useFar, const bool useSafe) {
		return Jump(pointer, reinterpret_cast<void*>(data), useFar, useSafe);
	}

	void Inject(void* pointer, const void* data) {
		if (IsRelativelyFar(pointer, data, 4)) {
			throw std::runtime_error("Too far away to create 32bit relative branch");
		}

		Set<int32_t>(pointer, int32_t(size_t(data) - (size_t(pointer) + 4)));
	}

	void Inject(const size_t pointer, const void* data) {
		return Inject(reinterpret_cast<void*>(pointer), data);
	}

	std::vector<uint8_t> MoveHook(void* pointer) {
		std::vector<uint8_t> originalData{};

		auto* data_ptr = static_cast<uint8_t*>(pointer);
		if (data_ptr[0] == 0xE9) {
			originalData.resize(6);
			memmove(originalData.data(), pointer, originalData.size());

			auto* target = FollowBranch(data_ptr);
			Nop(data_ptr, 1);
			Jump(data_ptr + 1, target);
		}
		else if (data_ptr[0] == 0xFF && data_ptr[1] == 0x25) {
			originalData.resize(15);
			memmove(originalData.data(), pointer, originalData.size());

			Copy(data_ptr + 1, data_ptr, 14);
			Nop(data_ptr, 1);
		}
		else {
			throw std::runtime_error("No branch instruction found");
		}

		return originalData;
	}

	std::vector<uint8_t> MoveHook(const size_t pointer) {
		return MoveHook(reinterpret_cast<void*>(pointer));
	}

	void* FollowBranch(void* address) {
		auto* const data = static_cast<uint8_t*>(address);
		if (*data != 0xE8 && *data != 0xE9) {
			throw std::runtime_error("No branch instruction found");
		}

		return Extract<void*>(data + 1);
	}
}
