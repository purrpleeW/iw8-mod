#include "common.hpp"
#include "hooks/hook.hpp"

template <>
int WSAAPI Client::Hook::Hooks::HK_SendTo::hkCallback(SOCKET s, const char* buf, int len, int flags, sockaddr* to, int tolen) {
	return sendto(s, buf, len, flags, to, tolen);
}
