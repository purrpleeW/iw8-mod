#pragma once
#include "common.hpp"
#include "engine/iw8/enums/LocalClientNum_t.hpp"
#include "engine/iw8/enums/UI_KEYBOARD_RESULT.hpp"
#include "engine/iw8/lua/crm_content.hpp"
#include "engine/iw8/lua/crm_locations.hpp"

#include <nlohmann/json.hpp>

namespace Client::Hook::Util {
	void* InitDbgHelp();
	LONG WINAPI TopLevelExceptionFilter(EXCEPTION_POINTERS* inf);

	void OnPlayerNameInput(IW8::LocalClientNum_t localClientNum, IW8::UI_KEYBOARD_RESULT res, const char* text);

	void InitialiseCRM();

	inline std::vector<std::function<void()>> g_GameThreadQueue{};
	inline bool g_ForceSignInState = false;
	inline bool g_WaitingToHook = true;
	inline bool g_HooksInitialised = false;

	inline std::unordered_map<IW8::Lua::CRMLocation::Enum, std::vector<IW8::Lua::CRMContent>> g_PatchedCRM{};
	/*inline nlohmann::json g_PatchedCRM = nlohmann::json::parse(
R"({
	"1": [
		{
			"message_index": 0,
			"title": "^1Welcome to iw8-mod!^7",
			"content_long": "This mod is a work-in-progress and should not be demonstrated as a finished product.\\n\\nAny bugs or errors found should be reported to ^4@lifix^7 or ^4@xifil^7 on Discord.\\n\\n^7https://discord.gg/dPzJajt",
			"checksum": "motd-30112024",
			"layout_type": 0,
			"location_id": 1,
			"message_id": "1"
		}
	],
	"2": [
		{
			"message_index": 0,
			"title": "Connecting with friends",
			"content_long": "Radmin VPN or Hamachi is still currently required as there is no working server browser or master server. After connecting with other players through this software, you will see their lobbies in your server list.",
			"checksum": "msg-0",
			"layout_type": 0,
			"location_id": 1,
			"message_id": "1"
		}
	],
	"8": [
		{
			"message_index": 0,
			"title": "30/11/24 Changelog",
			"content_short": "- added motd and patch notes\\n- added json parsing for said motd and patch notes\\n- fixed map image on server list",
			"checksum": "cl-30112024",
			"layout_type": 0,
			"location_id": 1,
			"message_id": "1"
		}
	]
})");
*/
}
