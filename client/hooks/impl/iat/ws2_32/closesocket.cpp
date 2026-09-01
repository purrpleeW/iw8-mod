#include "common.hpp"
#include "hooks/hook.hpp"

template <>
int WSAAPI Client::Hook::Hooks::HK_CloseSocket::hkCallback(SOCKET s) {
	return closesocket(s);
}
