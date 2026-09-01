#include "common.hpp"
#include "engine/iw8/lua/crm_content.hpp"
#include "engine/iw8/lua/crm_locations.hpp"
#include "game/game.hpp"
#include "hooks/hook.hpp"
#include "hooks/util/hook_util.hpp"

#include <utility/strings.hpp>

int Client::Hook::Hooks::HK_LUI_CoD_LuaCall_CRMGetMessageContent::hkCallback(IW8::lua_State* luaVM) {
	int controllerIndex = g_Pointers->m_lua_tointeger32(luaVM, 1) & 0xFFFFFFFF;
	int locationID = g_Pointers->m_lua_tointeger32(luaVM, 2) & 0xFFFFFFFF;
	int messageIndex = g_Pointers->m_lua_tointeger32(luaVM, 3) & 0xFFFFFFFF;

	LOG("Game/LUI_CoD_LuaCall_CRMGetMessageContent", DEBUG, "controller: {}, location: {}, messageIndex: {}", controllerIndex, locationID, messageIndex);

	bool pushedData = false;

	auto it = Hook::Util::g_PatchedCRM.find(static_cast<IW8::Lua::CRMLocation::Enum>(locationID));
	if (it != Hook::Util::g_PatchedCRM.end()) {
		auto messages = it->second;
		if (messages.size() > messageIndex) {
			auto& message = messages.at(messageIndex);
			message.AssignJson();

			g_Pointers->m_lua_createtable(luaVM, 0, 0);
			g_Pointers->m_LUI_BeginTable("message", luaVM);
			for (auto& [key, val] : message.m_JsonData.items()) {
				if (val.is_string()) {
					g_Pointers->m_LuaShared_SetTableString(key.c_str(), Common::Utility::Strings::Replace(val.get<std::string>(), "\\n", "\n").c_str(), luaVM);
				}
				else if (val.is_number()) {
					g_Pointers->m_LuaShared_SetTableInt(key.c_str(), val.get<std::int64_t>(), luaVM);
				}
			}
			g_Pointers->m_LUI_EndTable(luaVM);

			pushedData = true;
		}
	}

	if (!pushedData) {
		g_Pointers->m_lua_pushnil(luaVM);
	}

	return 1;
}
