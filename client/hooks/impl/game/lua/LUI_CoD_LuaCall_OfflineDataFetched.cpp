#include "common.hpp"
#include "game/game.hpp"
#include "hooks/hook.hpp"

int Client::Hook::Hooks::HK_LUI_CoD_LuaCall_OfflineDataFetched::hkCallback(IW8::lua_State* luaVM) {
	int controllerIndex = g_Pointers->m_lua_tointeger32(luaVM, 1) & 0xFFFFFFFF;
	g_Pointers->m_lua_pushboolean(luaVM, g_Pointers->m_LiveStorage_AreStatsFetched(controllerIndex, IW8::StatsSource::STATS_OFFLINE));
	return 1;
}
