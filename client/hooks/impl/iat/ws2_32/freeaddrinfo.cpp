#include "common.hpp"
#include "hooks/hook.hpp"

template <>
void WSAAPI Client::Hook::Hooks::HK_FreeAddrInfo::hkCallback(PADDRINFOA pAddrInfo) {
	return freeaddrinfo(pAddrInfo);
}
