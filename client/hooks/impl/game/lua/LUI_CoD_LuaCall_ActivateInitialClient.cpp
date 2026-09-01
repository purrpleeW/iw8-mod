#include "common.hpp"
#include "game/game.hpp"
#include "hooks/hook.hpp"

int Client::Hook::Hooks::HK_LUI_CoD_LuaCall_ActivateInitialClient::hkCallback(IW8::lua_State* luaVM) {
	std::size_t reasonSz = 0;
	const char* reason = g_Pointers->m_lua_tolstring(luaVM, 1, &reasonSz);
	std::int64_t controllerIndex = g_Pointers->m_lua_tointeger32(luaVM, 2);
	LOG("Game/LUI_CoD_LuaCall_ActivateInitialClient", INFO, "[{}] Called with reason: {}", controllerIndex, std::string(reason, reasonSz));
	return m_Original(luaVM);
}
