#include "common.hpp"
#include "game/game.hpp"
#include "hooks/hook.hpp"
#include "hooks/util/hook_util.hpp"

#define STRINGS_IGNORE_TRANSLATION FALSE
#define STRINGS_COMBINE_TRANSLATION_KEY FALSE

template <>
const char* Client::Hook::Hooks::HK_SEH_StringEd_GetString::hkCallback(const char* pszReference) {
	const char* ret = m_Original(pszReference);

	if (!pszReference[1]) {
		if ((*pszReference & 0x80) != 0) {
			return "t";
		}
		return pszReference;
	}

	static bool forcedSignInState = false;

	g_Pointers->m_GamerProfile_SetDataByName(0, "acceptedEULA", 1);
	g_Pointers->m_GamerProfile_SetDataByName(0, "hasEverPlayed_MainMenu", 1);
	if (!forcedSignInState) {
		// wait until we're at the main menu
		if (strcmp(pszReference, "MENU/STATUS") == 0) {
			std::thread thr([]() {
				std::this_thread::sleep_for(1s);
				Hook::Util::g_ForceSignInState = true;
				LOG("Authentication", INFO, "Sign in state now 2.");
			});
			thr.detach();
			forcedSignInState = true;
		}
	}

	static std::map<std::string, std::function<std::string(std::string)>> replacements;
	static std::map<GameVersion, std::vector<std::string>> unavailableMaps;
#	define REPL_FUNC(v) [=](std::string og) { return v; }

	if (replacements.size() == 0) {
		// loading message
		static std::string loadingMsg = "Welcome to iw8-mod. Loading...";
		replacements.emplace("MENU/POPUP_CONNECTING_OFFLINE", REPL_FUNC(loadingMsg));
		replacements.emplace("XBOXLIVE/POPUP_CONNECTION", REPL_FUNC(loadingMsg));

		// main menu status text
		replacements.emplace("MENU/STATUS", REPL_FUNC(std::format("iw8-mod: " GIT_DESCRIBE " - v{}{}", g_GameIdentifier.m_Version,
			g_GameIdentifier.m_Ship ? "\n^1Warning: Ship executables not fully supported" : "")));

		// local multiplayer -> multiplayer (it's the only one right now)
		replacements.emplace("LUA_MENU/LOCAL_MULTIPLAYER_CAPS", REPL_FUNC("MULTIPLAYER"));

		// protanopia relation
		replacements.emplace("LUA_MENU/PROTANOPIA", REPL_FUNC(std::format("{} (lifix gta 7 mode)", og)));

		// unavailable game modes
		switch (g_GameIdentifier.m_Checksum) {
		case GameVersion::v0_01_2_7089334:
		case GameVersion::v1_20_4_7623265_REPLAY:
		case GameVersion::v1_20_4_7623265_SHIP:
		case GameVersion::v1_44_0_10435696:
			replacements.emplace("MENU_SP/CAMPAIGN", REPL_FUNC(std::format("^1{}", og)));
			replacements.emplace("LUA_MENU/CAMPAIGN_DESC", REPL_FUNC("^3Campaign is not available in this build of the game."));

			replacements.emplace("LUA_MENU/LOCAL_COOP_CAPS", REPL_FUNC(std::format("^1{}", og)));
			replacements.emplace("LUA_MENU/LOCAL_COOP_DESC", REPL_FUNC("^3Co-op is not available in this build of the game."));
		}

		// unavailable maps
		unavailableMaps.emplace(GameVersion::v0_01_2_7089334, std::vector<std::string>{});
		unavailableMaps.emplace(GameVersion::v1_03_0_7209368, std::vector<std::string>{});
		std::vector<std::string> unavailableMaps_v1_20 = {
			"LUA_MENU/MAPNAME_ANIYAH",
			"LUA_MENU/MAPNAME_CAVE_AM",
			"LUA_MENU/MAPNAME_CAVE",
			"LUA_MENU/MAPNAME_CRASH2",
			"LUA_MENU/MAPNAME_DEADZONE",
			"LUA_MENU/MAPNAME_EUPHRATES",
			"LUA_MENU/MAPNAME_HACKNEY_AM",
			"LUA_MENU/MAPNAME_HACKNEY_YARD",
			"LUA_MENU/MAPNAME_M_CAGE",
			"LUA_MENU/MAPNAME_M_CARGO",
			"LUA_MENU/MAPNAME_M_HILL",
			"LUA_MENU/MAPNAME_M_OVERUNDER",
			"LUA_MENU/MAPNAME_M_PINE",
			"LUA_MENU/MAPNAME_M_SHOWERS",
			"LUA_MENU/MAPNAME_M_STACK",
			"LUA_MENU/MAPNAME_PETROGRAD",
			"LUA_MENU/MAPNAME_PICCADILLY",
			"LUA_MENU/MAPNAME_RAID",
			"LUA_MENU/MAPNAME_RUNNER",
			"LUA_MENU/MAPNAME_RUNNER_AM",
			"LUA_MENU/MAPNAME_SPEAR",
			"LUA_MENU/MAPNAME_SPEAR_AM",
			"LUA_MENU/MAPNAME_VACANT"
		};
		unavailableMaps.emplace(GameVersion::v1_20_4_7623265_REPLAY, unavailableMaps_v1_20);
		unavailableMaps.emplace(GameVersion::v1_20_4_7623265_SHIP, unavailableMaps_v1_20);
		unavailableMaps.emplace(GameVersion::v1_38_3_9489393, std::vector<std::string>{});
		unavailableMaps.emplace(GameVersion::v1_44_0_10435696, std::vector<std::string>{});
	}

#	if STRINGS_IGNORE_TRANSLATION
		return pszReference;
#	elif STRINGS_COMBINE_TRANSLATION_KEY
		return g_Pointers->m_j_va("%s (%s)", ret, pszReference);
#	else
		auto it = replacements.find(pszReference);
		if (it != replacements.end()) {
			return g_Pointers->m_j_va("%s", it->second(ret).c_str());
		}
		std::vector<std::string> maps = unavailableMaps[g_GameIdentifier.m_Checksum];
		if (std::find(maps.begin(), maps.end(), std::string(pszReference)) != maps.end()) {
			return g_Pointers->m_j_va("%s ^1(Unavailable)", ret);
		}

		return ret;
#	endif
}
