#include "common.hpp"
#include "hooks/hook.hpp"

template <>
bool Client::Hook::Hooks::HK_Unk_IsUserSignedInToBnet::hkCallback() {
	return true;
}
