#include "common.hpp"
#include "game/functions.hpp"
#include "game/game.hpp"

namespace Client {
	void Game::LuaShared_LuaCall_SetDvarBool(IW8::lua_State* luaVM, const char* dvarName, bool dvarValue) {
		g_Pointers->m_lua_getfield(luaVM, -10002, "Dvar");
		g_Pointers->m_lua_getfield(luaVM, -1, "DHEGHJJJHI");
		g_Pointers->m_lua_remove(luaVM, -2);
		g_Pointers->m_lua_pushstring(luaVM, dvarName);
		g_Pointers->m_lua_pushboolean(luaVM, dvarValue);
		g_Pointers->m_LuaShared_PCall(luaVM, 1, 1);
	}

	void Game::LUI_CoD_LuaCall_ExecNow(IW8::lua_State* luaVM, const char* str) {
		g_Pointers->m_lua_getfield(luaVM, -10002, "Engine");
		g_Pointers->m_lua_getfield(luaVM, -1, "DAGFFDGFII");
		g_Pointers->m_lua_remove(luaVM, -2);
		g_Pointers->m_lua_pushstring(luaVM, str);
		g_Pointers->m_LuaShared_PCall(luaVM, 1, 1);
	}

	void Game::Cbuf_AddText(const char* cmd) {
		if (g_Pointers->m_LUI_luaVM && *g_Pointers->m_LUI_luaVM) {
			LUI_CoD_LuaCall_ExecNow(*g_Pointers->m_LUI_luaVM, cmd);
		}
	}
}
