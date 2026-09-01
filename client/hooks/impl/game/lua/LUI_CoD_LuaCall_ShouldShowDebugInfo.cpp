#include "common.hpp"
#include "game/game.hpp"
#include "hooks/hook.hpp"

int Client::Hook::Hooks::HK_LUI_CoD_LuaCall_ShouldShowDebugInfo::hkCallback(IW8::lua_State* luaVM) {
	if (g_ShowDebugInfo) {
		g_Pointers->m_lua_pushboolean(luaVM, TRUE);
		return 1;
	}
	else {
		return m_Original(luaVM);
	}
}
