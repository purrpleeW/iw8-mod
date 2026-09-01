#include "common.hpp"
#include "game/functions.hpp"
#include "game/game.hpp"
#include "game/inventory.hpp"
#include "hooks/hook.hpp"

int Client::Hook::Hooks::HK_LUI_CoD_LuaCall_NotifyServer::hkCallback(IW8::lua_State* luaVM) {
	std::size_t channelNameSz = 0;
	const char* channelNameBuf = g_Pointers->m_lua_tolstring(luaVM, 1, &channelNameSz);
	std::string channelName = std::string(channelNameBuf, channelNameSz);

	std::int64_t numericData = g_Pointers->m_lua_tointeger32(luaVM, 2);

	LOG("Game/LUI_CoD_LuaCall_NotifyServer", INFO, "{} - {}", channelName, numericData);

	static bool unlockAllExecuted = false;
	if (!unlockAllExecuted) {
		Game::Cbuf_AddText("seta unlockAllItems 1");
		unlockAllExecuted = true;
	}
	if (channelName == "class_edit" || channelName == "loadout_showcase_entered") {
		Inventory::Save();
	}

	return m_Original(luaVM);
}
