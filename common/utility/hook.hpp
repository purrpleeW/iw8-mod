#pragma once
#include "common_core.hpp"
#include "utility/nt.hpp"

namespace Common::Utility::Hook {
	namespace Detail {
		template <size_t Entries>
		std::vector<size_t(*)()> GetIotaFunctions() {
			if constexpr (Entries == 0) {
				std::vector<size_t(*)()> functions;
				return functions;
			}
			else {
				auto functions = GetIotaFunctions<Entries - 1>();
				functions.emplace_back([]() {
					return Entries - 1;
				});
				return functions;
			}
		}
	}

	// Gets the pointer to the entry in the v-table.
	// It seems otherwise impossible to get this.
	// This is ugly as fuck and only safely works on x64
	// Example:
	//   ID3D11Device* device = ...
	//   auto entry = GetVtableEntry(device, &ID3D11Device::CreateTexture2D);
	template <size_t Entries = 100, typename Class, typename T, typename... Args>
	void** GetVtableEntry(Class* obj, T(Class::* entry)(Args ...)) {
		union {
			decltype(entry) func;
			void* pointer;
		};

		func = entry;

		auto iotaFunctions = Detail::GetIotaFunctions<Entries>();
		auto* object = iotaFunctions.data();

		using FakeFunc = size_t(__thiscall*)(void* self);
		auto index = static_cast<FakeFunc>(pointer)(&object);

		void** objVtable = *reinterpret_cast<void***>(obj);
		return &objVtable[index];
	}

	class Detour {
	public:
		Detour();
		Detour(void* place, void* target);
		Detour(size_t place, void* target);
		~Detour();

		Detour(Detour&& other) noexcept {
			this->operator=(std::move(other));
		}

		Detour& operator=(Detour&& other) noexcept {
			if (this != &other) {
				this->Clear();

				this->m_Place = other.m_Place;
				this->m_Original = other.m_Original;
				this->m_MovedData = other.m_MovedData;

				other.m_Place = nullptr;
				other.m_Original = nullptr;
				other.m_MovedData = {};
			}

			return *this;
		}

		Detour(const Detour&) = delete;
		Detour& operator=(const Detour&) = delete;

		void Enable();
		void Disable();

		void Create(void* place, void* target);
		void Create(size_t place, void* target);
		void Clear();

		void Move();

		void* GetPlace() const;

		template <typename T>
		T* Get() const {
			return static_cast<T*>(this->GetOriginal());
		}

		template <typename T = void, typename... Args>
		T Stub(Args ... args) {
			return static_cast<T(*)(Args ...)>(this->GetOriginal())(args...);
		}

		[[nodiscard]] void* GetOriginal() const;

	private:
		std::vector<uint8_t> m_MovedData{};
		void* m_Place{};
		void* m_Original{};

		void UnMove();
	};

	std::optional<std::pair<void*, void*>> IAT(const NT::Library& library, const std::string& targetLibrary,
		const std::string& process, void* stub);

	void Nop(void* place, size_t length);
	void Nop(size_t place, size_t length);

	void Copy(void* place, const void* data, size_t length);
	void Copy(size_t place, const void* data, size_t length);

	void CopyString(void* place, const char* str);
	void CopyString(size_t place, const char* str);

	bool IsRelativelyFar(const void* pointer, const void* data, int offset = 5);

	void Call(void* pointer, void* data);
	void Call(size_t pointer, void* data);
	void Call(size_t pointer, size_t data);

	void Jump(void* pointer, void* data, bool useFar = false, bool useSafe = false);
	void Jump(size_t pointer, void* data, bool useFar = false, bool useSafe = false);
	void Jump(size_t pointer, size_t data, bool useFar = false, bool useSafe = false);

	void Inject(void* pointer, const void* data);
	void Inject(size_t pointer, const void* data);

	std::vector<uint8_t> MoveHook(void* pointer);
	std::vector<uint8_t> MoveHook(size_t pointer);

	template <typename T>
	T Extract(void* address) {
		auto* const data = static_cast<uint8_t*>(address);
		const auto offset = *reinterpret_cast<int32_t*>(data);
		return reinterpret_cast<T>(data + offset + 4);
	}

	void* FollowBranch(void* address);

	template <typename T>
	static void Set(void* place, T value = false) {
		Copy(place, &value, sizeof(value));
	}

	template <typename T>
	static void Set(const size_t place, T value = false) {
		return Set<T>(reinterpret_cast<void*>(place), value);
	}

	template <typename T, typename... Args>
	static T Invoke(size_t func, Args ... args) {
		return reinterpret_cast<T(*)(Args ...)>(func)(args...);
	}

	template <typename T, typename... Args>
	static T Invoke(void* func, Args ... args) {
		return static_cast<T(*)(Args ...)>(func)(args...);
	}
}
