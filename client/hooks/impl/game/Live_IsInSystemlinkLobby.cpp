#include "common.hpp"
#include "game/game.hpp"
#include "hooks/hook.hpp"

template <>
bool Client::Hook::Hooks::HK_Live_IsInSystemlinkLobby::hkCallback() {
	IW8::dvar_t* systemlink = g_Pointers->m_Dvar_FindVarByName("LPSPMQSNPQ");
	return systemlink && systemlink->m_Current.m_Enabled;
}
