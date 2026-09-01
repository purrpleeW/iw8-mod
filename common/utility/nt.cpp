#include "common_core.hpp"
#include "utility/nt.hpp"

namespace Common::Utility::NT {
	Library Library::Load(const std::string& name) {
		return Library(LoadLibraryA(name.data()));
	}

	Library Library::Load(const std::filesystem::path& path) {
		return Library::Load(path.generic_string());
	}

	Library Library::GetByAddress(void* address) {
		HMODULE handle = nullptr;
		GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
			static_cast<LPCSTR>(address), &handle);
		return Library(handle);
	}

	Library::Library() {
		this->m_Module = GetModuleHandleA(nullptr);
	}

	Library::Library(const std::string& name) {
		this->m_Module = GetModuleHandleA(name.data());
	}

	Library::Library(const HMODULE handle) {
		this->m_Module = handle;
	}

	bool Library::operator==(const Library& obj) const {
		return this->m_Module == obj.m_Module;
	}

	Library::operator bool() const {
		return this->IsValid();
	}

	Library::operator HMODULE() const {
		return this->GetHandle();
	}

	PIMAGE_NT_HEADERS Library::GetNTHeaders() const {
		if (!this->IsValid()) {
			return nullptr;
		}
		return reinterpret_cast<PIMAGE_NT_HEADERS>(this->GetPtr() + this->GetDOSHeader()->e_lfanew);
	}

	PIMAGE_DOS_HEADER Library::GetDOSHeader() const {
		return reinterpret_cast<PIMAGE_DOS_HEADER>(this->GetPtr());
	}

	PIMAGE_OPTIONAL_HEADER Library::GetOptionalHeader() const {
		if (!this->IsValid()) {
			return nullptr;
		}
		return &this->GetNTHeaders()->OptionalHeader;
	}

	std::vector<PIMAGE_SECTION_HEADER> Library::GetSectionHeaders() const {
		std::vector<PIMAGE_SECTION_HEADER> headers;

		auto ntHeaders = this->GetNTHeaders();
		auto section = IMAGE_FIRST_SECTION(ntHeaders);

		for (uint16_t i = 0; i < ntHeaders->FileHeader.NumberOfSections; ++i, ++section) {
			if (section) {
				headers.push_back(section);
			}
			else {
				OutputDebugStringA("There was an invalid section :O");
			}
		}

		return headers;
	}

	std::uint8_t* Library::GetPtr() const {
		return reinterpret_cast<std::uint8_t*>(this->m_Module);
	}

	void Library::Unprotect() const {
		if (!this->IsValid()) {
			return;
		}

		DWORD protection;
		VirtualProtect(this->GetPtr(), this->GetOptionalHeader()->SizeOfImage, PAGE_EXECUTE_READWRITE, &protection);
	}

	size_t Library::GetRelativeEntryPoint() const {
		if (!this->IsValid()) {
			return 0;
		}

		return this->GetNTHeaders()->OptionalHeader.AddressOfEntryPoint;
	}

	void* Library::GetEntryPoint() const {
		if (!this->IsValid()) {
			return nullptr;
		}

		return this->GetPtr() + this->GetRelativeEntryPoint();
	}

	bool Library::IsValid() const {
		return this->m_Module != nullptr && this->GetDOSHeader()->e_magic == IMAGE_DOS_SIGNATURE;
	}

	std::string Library::GetName() const {
		if (!this->IsValid()) {
			return "";
		}

		auto path = this->GetPath();
		const auto pos = path.find_last_of("/\\");
		if (pos == std::string::npos) return path;

		return path.substr(pos + 1);
	}

	std::string Library::GetPath() const {
		if (!this->IsValid()) {
			return "";
		}

		char name[MAX_PATH] = { 0 };
		GetModuleFileNameA(this->m_Module, name, sizeof name);

		return name;
	}

	std::string Library::GetFolder() const {
		if (!this->IsValid()) {
			return "";
		}

		const auto path = std::filesystem::path(this->GetPath());
		return path.parent_path().generic_string();
	}

	void Library::Free() {
		if (this->IsValid()) {
			FreeLibrary(this->m_Module);
			this->m_Module = nullptr;
		}
	}

	HMODULE Library::GetHandle() const {
		return this->m_Module;
	}

	std::vector<Library::TlsCallback*> Library::GetTlsCallbacks() {
		std::vector<TlsCallback*> callbacks{};
		DWORD virtualAddr = this->GetOptionalHeader()->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress;

		if (virtualAddr == 0) {
			return callbacks;
		}

		PIMAGE_TLS_DIRECTORY tlsDir = reinterpret_cast<PIMAGE_TLS_DIRECTORY>(this->GetPtr() + virtualAddr);
		TlsCallback** current = reinterpret_cast<TlsCallback**>(tlsDir->AddressOfCallBacks);
		while (*current != nullptr) {
			callbacks.push_back(*current);
			current = reinterpret_cast<TlsCallback**>(reinterpret_cast<std::uintptr_t>(current) + sizeof(std::uintptr_t));
		}

		return callbacks;
	}

	void** Library::GetIATEntry(const std::string& moduleName, const std::string& procName) const {
		return this->GetIATEntry(moduleName, procName.data());
	}

	void** Library::GetIATEntry(const std::string& moduleName, const char* procName) const {
		if (!this->IsValid()) {
			return nullptr;
		}

		const Library otherModule(moduleName);
		if (!otherModule.IsValid()) {
			return nullptr;
		}

		auto* const targetFunction = otherModule.GetProc<void*>(procName);
		if (!targetFunction) {
			return nullptr;
		}

		const auto* header = this->GetOptionalHeader();
		if (!header) {
			return nullptr;
		}

		auto* importDescriptor = reinterpret_cast<PIMAGE_IMPORT_DESCRIPTOR>(this->GetPtr() + header->DataDirectory
			[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

		while (importDescriptor->Name) {
			if (!_stricmp(reinterpret_cast<char*>(this->GetPtr() + importDescriptor->Name), moduleName.data())) {
				auto* originalThunkData = reinterpret_cast<PIMAGE_THUNK_DATA>(importDescriptor->OriginalFirstThunk + this->GetPtr());
				auto* thunkData = reinterpret_cast<PIMAGE_THUNK_DATA>(importDescriptor->FirstThunk + this->GetPtr());

				while (originalThunkData->u1.AddressOfData) {
					if (thunkData->u1.Function == reinterpret_cast<uint64_t>(targetFunction)) {
						return reinterpret_cast<void**>(&thunkData->u1.Function);
					}

					const size_t ordinalNumber = originalThunkData->u1.AddressOfData & 0xFFFFFFF;

					if (ordinalNumber <= 0xFFFF) {
						auto* proc = GetProcAddress(otherModule.m_Module, reinterpret_cast<char*>(ordinalNumber));
						if (reinterpret_cast<void*>(proc) == targetFunction) {
							return reinterpret_cast<void**>(&thunkData->u1.Function);
						}
					}

					++originalThunkData;
					++thunkData;
				}

				//break;
			}

			++importDescriptor;
		}

		return nullptr;
	}

	std::uint32_t Library::GetChecksum() {
		std::ifstream file(this->GetPath(), std::ios::binary);
		if (!file.is_open()) {
			return 0;
		}

		uint32_t checksum = 0;
		char byte;
		while (file.get(byte)) {
			checksum += static_cast<uint8_t>(byte);
		}

		return checksum;
	}

	void RaiseHardException() {
		int data = false;
		const Library ntdll("ntdll.dll");
		ntdll.InvokePascal<void>("RtlAdjustPrivilege", 19, true, false, &data);
		ntdll.InvokePascal<void>("NtRaiseHardError", 0xC000007B, 0, nullptr, nullptr, 6, &data);
	}

	std::string LoadResource(const int id) {
		auto* const res = FindResource(Library(), MAKEINTRESOURCE(id), RT_RCDATA);
		if (!res) {
			return {};
		}

		auto* const handle = LoadResource(nullptr, res);
		if (!handle) {
			return {};
		}

		return std::string(LPSTR(LockResource(handle)), SizeofResource(nullptr, res));
	}

	void RelaunchSelf() {
		const Library self;

		STARTUPINFOA startup_info;
		PROCESS_INFORMATION process_info;

		ZeroMemory(&startup_info, sizeof(startup_info));
		ZeroMemory(&process_info, sizeof(process_info));
		startup_info.cb = sizeof(startup_info);

		char current_dir[MAX_PATH];
		GetCurrentDirectoryA(sizeof(current_dir), current_dir);
		auto* const command_line = GetCommandLineA();

		CreateProcessA(self.GetPath().data(), command_line, nullptr, nullptr, false, NULL, nullptr, current_dir,
			&startup_info, &process_info);

		if (process_info.hThread && process_info.hThread != INVALID_HANDLE_VALUE) {
			CloseHandle(process_info.hThread);
		}
		if (process_info.hProcess && process_info.hProcess != INVALID_HANDLE_VALUE) {
			CloseHandle(process_info.hProcess);
		}
	}

	void Terminate(const uint32_t code) {
		TerminateProcess(GetCurrentProcess(), code);
	}
}
