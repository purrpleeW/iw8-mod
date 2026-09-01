#include "common.hpp"
#include "game/game.hpp"
#include "hooks/hook.hpp"

template <>
void Client::Hook::Hooks::HK_SV_UpdateUserinfo_f::hkCallback(void* cl) {
	/*
		More checks can be added here

		It's patched in:
			- Modern Warfare (2019) (Current version)
			- Vanguard
			- Modern Warfare II

		Could probably find the actual fix there
	*/
	if (!strlen(g_Pointers->m_Info_ValueForKey(g_Pointers->m_SV_Cmd_Argv(1), "platform"))) {
		return;
	}

	return m_Original(cl);
}
