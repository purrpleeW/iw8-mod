#include "common.hpp"
#include "hooks/hook.hpp"
#include "hooks/util/hook_util.hpp"

template <>
HANDLE WINAPI Client::Hook::Hooks::HK_CreateFileA::hkCallback(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	Hook::Util::g_WaitingToHook = false;
	return CreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);

	// this is called exactly when checking if the game didn't quit properly on last launch
	// which is always because the game always crashes after 2 minutes on ship and on replay
	// just doesn't quit properly lol
	/*
	auto res = CreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	LOG("MW19/SysCall", DEBUG, "CreateFileA({}, {}, {}, {}, {}, {}, {}) = {}", lpFileName, dwDesiredAccess, dwShareMode,
		AndRel(reinterpret_cast<std::uintptr_t>(lpSecurityAttributes)), dwCreationDisposition, dwFlagsAndAttributes,
		AndRel(reinterpret_cast<std::uintptr_t>(hTemplateFile)),
		AndRel(reinterpret_cast<std::uintptr_t>(res)));
	return res;
	*/
}
