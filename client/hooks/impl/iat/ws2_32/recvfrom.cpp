#include "common.hpp"
#include "hooks/hook.hpp"

template <>
int WSAAPI Client::Hook::Hooks::HK_RecvFrom::hkCallback(SOCKET s, char* buf, int len, int flags, sockaddr* from, int* fromlen) {
	return recvfrom(s, buf, len, flags, from, fromlen);
}
