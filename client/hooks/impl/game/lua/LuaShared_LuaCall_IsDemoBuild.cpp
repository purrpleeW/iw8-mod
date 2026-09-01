#include "common.hpp"
#include "game/game.hpp"
#include "hooks/hook.hpp"

int Client::Hook::Hooks::HK_LuaShared_LuaCall_IsDemoBuild::hkCallback(IW8::lua_State* luaVM) {
	g_Pointers->m_lua_pushboolean(luaVM, TRUE);
	return 1;
}
