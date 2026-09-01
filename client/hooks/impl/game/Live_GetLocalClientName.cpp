#include "common.hpp"
#include "game/config.hpp"
#include "hooks/hook.hpp"

template <>
const char* Client::Hook::Hooks::HK_Live_GetLocalClientName::hkCallback() {
	static std::string playerName = "Unknown Soldier";
	playerName = g_Config.GetPlayerName();

	return playerName.c_str();
}
