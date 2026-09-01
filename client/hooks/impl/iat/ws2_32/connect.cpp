#include "common.hpp"
#include "hooks/hook.hpp"

template <>
int WSAAPI Client::Hook::Hooks::HK_Connect::hkCallback(SOCKET s, const sockaddr* name, int namelen) {
	return connect(s, name, namelen);
}
