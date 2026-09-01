#include "common.hpp"
#include "hooks/hook.hpp"

template <>
int WSAAPI Client::Hook::Hooks::HK_GetSockName::hkCallback(SOCKET s, sockaddr* name, int* namelen) {
	return getsockname(s, name, namelen);
}
