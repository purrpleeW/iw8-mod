#include "common.hpp"
#include "hooks/hook.hpp"

template <>
int WSAAPI Client::Hook::Hooks::HK_Select::hkCallback(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, const timeval* timeout) {
	return select(nfds, readfds, writefds, exceptfds, timeout);
}
