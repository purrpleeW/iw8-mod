#include "common.hpp"
#include "hooks/hook.hpp"

template <>
int WSAAPI Client::Hook::Hooks::HK_GetPeerName::hkCallback(SOCKET s, sockaddr* name, int* namelen) {
	return getpeername(s, name, namelen);
}
