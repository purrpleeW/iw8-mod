#include "common.hpp"
#include "hooks/hook.hpp"

template <>
int WSAAPI Client::Hook::Hooks::HK_Recv::hkCallback(SOCKET s, char* buf, int len, int flags) {
	return recv(s, buf, len, flags);
}
