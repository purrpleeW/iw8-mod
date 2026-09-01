#include "common.hpp"
#include "hooks/hook.hpp"

template <>
hostent* WSAAPI Client::Hook::Hooks::HK_GetHostByName::hkCallback(const char* name) {
	LOG("Game/gethostbyname", DEBUG, "hello from GHBN hook: {}", name);
#	pragma warning(push)
#	pragma warning(disable : 4996)
	return gethostbyname(name);
#	pragma warning(pop)
}
