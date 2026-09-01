#include "common.hpp"
#include "hooks/hook.hpp"

template <>
int WSAAPI Client::Hook::Hooks::HK_IoctlSocket::hkCallback(SOCKET s, long cmd, u_long* argp) {
	return ioctlsocket(s, cmd, argp);
}
