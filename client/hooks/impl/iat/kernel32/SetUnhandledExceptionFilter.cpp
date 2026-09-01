#include "common.hpp"
#include "hooks/hook.hpp"
#include "hooks/util/hook_util.hpp"

bool s_CalledPostUnpack = false;

template <>
LPTOP_LEVEL_EXCEPTION_FILTER WINAPI Client::Hook::Hooks::HK_SetUnhandledExceptionFilter::hkCallback(LPTOP_LEVEL_EXCEPTION_FILTER filter) {
	_Unreferenced_parameter_(filter);
	auto res = SetUnhandledExceptionFilter(Hook::Util::TopLevelExceptionFilter);
	if (!s_CalledPostUnpack) {
		g_Hooks->PostUnpack();
		s_CalledPostUnpack = true;
	}
	return res;
}
