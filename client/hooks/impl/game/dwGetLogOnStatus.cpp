#include "common.hpp"
#include "hooks/hook.hpp"

template <>
IW8::DWOnlineStatus Client::Hook::Hooks::HK_dwGetLogOnStatus::hkCallback(int controllerIndex) {
	_Unreferenced_parameter_(controllerIndex);
	return IW8::DWOnlineStatus::DW_LIVE_CONNECTED;
}
