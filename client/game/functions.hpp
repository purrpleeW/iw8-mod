#pragma once
#include "common.hpp"
#include "engine/iw8/lua_State.hpp"

namespace Client::Game {
	void LuaShared_LuaCall_SetDvarBool(IW8::lua_State* luaVM, const char* dvarName, bool dvarValue);
	void LUI_CoD_LuaCall_ExecNow(IW8::lua_State* luaVM, const char* str);
	void Cbuf_AddText(const char* cmd);
}
