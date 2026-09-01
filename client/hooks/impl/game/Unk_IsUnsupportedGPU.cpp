#include "common.hpp"
#include "hooks/hook.hpp"

template <>
bool Client::Hook::Hooks::HK_Unk_IsUnsupportedGPU::hkCallback() {
	return false;
}
