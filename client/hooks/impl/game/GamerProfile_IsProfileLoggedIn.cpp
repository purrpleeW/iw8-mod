#include "common.hpp"
#include "hooks/hook.hpp"

template <>
bool Client::Hook::Hooks::HK_GamerProfile_IsProfileLoggedIn::hkCallback(int controllerIndex) {
	_Unreferenced_parameter_(controllerIndex);
	return true;
}
