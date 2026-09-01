#include "common.hpp"
#include "hooks/hook.hpp"

template <>
int WSAAPI Client::Hook::Hooks::HK_GetAddrInfo::hkCallback(PCSTR pNodeName, PCSTR pServiceName, const ADDRINFOA* pHints, PADDRINFOA* ppResult) {
	return getaddrinfo(pNodeName, pServiceName, pHints, ppResult);
}
