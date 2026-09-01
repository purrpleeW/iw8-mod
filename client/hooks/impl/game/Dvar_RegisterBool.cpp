#include "common.hpp"
#include "game/game.hpp"
#include "hooks/hook.hpp"

std::map<std::pair<const char*, const char*>, bool> s_PatchedBooleanDvars = {
	{ { "cg_drawBuildName", "LSSSQMQPNL" }, true },
	{ { "cg_drawFastfileDebugInfo", "MOSSSSTTNL" }, true },
	{ { "cg_drawFPS", "OLNTNRTPPL" }, true },
	{ { "cg_drawFrontendSceneDebugInfo", "OMPMKKTORN" }, true },
	{ { "cg_viewedSplashScreen", "MLNMPQOON" }, true },
	{ { "cl_waterMarkEnabled", "LRKNROSQPM" }, true },
	{ { "com_checkIfGameModeInstalled", "RLSPOOTTT" }, false },
	{ { "com_force_premium", "MROLPRPTPO" }, true },
	{ { "com_lan_lobby_enabled", "LPNMMPKRL" }, true },
	{ { "con_bindableGrave", "OKLQKPPKPQ" }, false },
	{ { "con_minicon", "LMSLLSMONN" }, true },
	{ { "force_offline_enabled", "MPSSOTQQPM" }, true },
	{ { "force_offline_menus", "LSTQOKLTRN" }, true },
	{ { "lui_cod_points_enabled", "LNTOKPTKS" }, false },
	{ { "lui_enable_magma_blade_layout", "LRKPTLNQTT" }, false },
	{ { "lui_force_online_menus", "LMMRONPQMO" }, false },
	{ { "lui_wz_tutorial_optional", "LSPSKLPNQT" }, true },
	{ { "online_lan_cross_play", "LTOQRQMMLQ" }, true },
	{ { "ui_onlineRequired", "MTSTMKPMRM" }, false }
};

template <>
IW8::dvar_t* Client::Hook::Hooks::HK_Dvar_RegisterBool::hkCallback(const char* dvarName, bool value, std::uint32_t flags, const char* description) {
	bool valuePatched = value;
	std::uint32_t flagsPatched = flags;
	for (const auto& [names, val] : s_PatchedBooleanDvars) {
		if (strcmp(dvarName, names.second) == 0) {
			const char* disclaimer = "";
			if (valuePatched == val) {
				disclaimer = " - unnecessary";
			}
			LOG("Game/Dvar_RegisterBool", INFO, "Patched '{}' -> {}{}", names.first, val ? "true" : "false", disclaimer);
			valuePatched = val;
		}
	}
	if (strcmp(dvarName, "LPSPMQSNPQ") == 0) {
		LOG("Game/Dvar_RegisterBool", INFO, "Patched systemlink");
		flagsPatched = 0x80;
	}
	return m_Original(dvarName, valuePatched, flagsPatched, description);
}
