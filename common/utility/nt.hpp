#pragma once
#include "common_core.hpp"

namespace Common::Utility::NT {
	class Library final {
	public:
		using TlsCallback = void();

		static Library Load(const std::string& name);
		static Library Load(const std::filesystem::path& path);
		static Library GetByAddress(void* address);

		Library();
		explicit Library(const std::string& name);
		explicit Library(HMODULE handle);

		Library(const Library& library)
			: m_Module(library.m_Module)
		{}

		bool operator!=(const Library& obj) const { return !(*this == obj); };
		bool operator==(const Library& obj) const;

		operator bool() const;
		operator HMODULE() const;

		void Unprotect() const;
		void* GetEntryPoint() const;
		size_t GetRelativeEntryPoint() const;

		bool IsValid() const;
		std::string GetName() const;
		std::string GetPath() const;
		std::string GetFolder() const;
		std::uint8_t* GetPtr() const;
		void Free();

		HMODULE GetHandle() const;
		std::vector<TlsCallback*> GetTlsCallbacks();

		template <typename T>
		T GetProc(const std::string& process) const {
			if (!this->IsValid()) {
				T{};
			}
			return reinterpret_cast<T>(GetProcAddress(this->m_Module, process.data()));
		}

		template <typename T>
		T GetProc(const char* name) const {
			if (!this->IsValid()) {
				T{};
			}
			return reinterpret_cast<T>(GetProcAddress(this->m_Module, name));
		}

		template <typename T>
		std::function<T> Get(const std::string& process) const {
			if (!this->IsValid()) {
				return std::function<T>();
			}
			return static_cast<T*>(this->GetProc<void*>(process));
		}

		template <typename T, typename... Args>
		T Invoke(const std::string& process, Args ... args) const {
			auto method = this->Get<T(__cdecl)(Args ...)>(process);
			if (method) {
				return method(args...);
			}
			return T();
		}

		template <typename T, typename... Args>
		T InvokePascal(const std::string& process, Args ... args) const {
			auto method = this->Get<T(__stdcall)(Args ...)>(process);
			if (method) {
				return method(args...);
			}
			return T();
		}

		template <typename T, typename... Args>
		T InvokeThis(const std::string& process, void* thisPtr, Args ... args) const {
			auto method = this->Get<T(__thiscall)(void*, Args ...)>(thisPtr, process);
			if (method) {
				return method(args...);
			}
			return T();
		}

		std::vector<PIMAGE_SECTION_HEADER> GetSectionHeaders() const;

		PIMAGE_NT_HEADERS GetNTHeaders() const;
		PIMAGE_DOS_HEADER GetDOSHeader() const;
		PIMAGE_OPTIONAL_HEADER GetOptionalHeader() const;

		void** GetIATEntry(const std::string& moduleName, const std::string& procName) const;
		void** GetIATEntry(const std::string& moduleName, const char* name) const;

		std::uint32_t GetChecksum();

	private:
		HMODULE m_Module;
	};

	__declspec(noreturn) void RaiseHardException();
	std::string LoadResource(int id);

	void RelaunchSelf();
	__declspec(noreturn) void Terminate(uint32_t code = 0);
}
