#include "common.hpp"
#include "hooks/hook.hpp"

template <>
int WSAAPI Client::Hook::Hooks::HK_Send::hkCallback(SOCKET s, const char* buf, int len, int flags) {
	return send(s, buf, len, flags);
}
