#include "common.hpp"
#include "game/game.hpp"
#include "game/map_validator.hpp"
#include "hooks/util/hook_util.hpp"

namespace Client::Game {
	std::string s_CurrentMapName{};

	DWORD MapValidator::Thread(PVOID) {
		while (g_Running) {
			if (g_Pointers == nullptr || g_Pointers->m_Com_SetErrorMessage == nullptr || g_Pointers->m_Dvar_FindVarByName == nullptr || g_Pointers->m_j_va == nullptr) {
				std::this_thread::sleep_for(1s);
				continue;
			}

			if (g_Pointers->m_s_luaInFrontend != nullptr && !(*g_Pointers->m_s_luaInFrontend)) {
				std::this_thread::sleep_for(10s);
				continue;
			}

			IW8::dvar_t* ui_mapname = g_Pointers->m_Dvar_FindVarByName("NSQLTTMRMP" /* ui_mapname */);
			if (ui_mapname == nullptr || strlen(ui_mapname->m_Current.m_String) == 0
				//|| s_CurrentMapName == ui_mapname->m_Current.m_String
				) {
				continue;
			}

			bool firstTime = s_CurrentMapName.size() == 0;
			s_CurrentMapName = ui_mapname->m_Current.m_String;

			auto it = g_LoadedFastfiles.find(s_CurrentMapName);
			if (it != g_LoadedFastfiles.end() && !it->second) {
				Hook::Util::g_GameThreadQueue.push_back([=]() {
					ui_mapname->m_Current.m_String = "mp_shipment";
					if (!firstTime) {
						// this can bug out horribly
						//g_Pointers->m_Com_SetErrorMessage(g_Pointers->m_j_va("[iw8-mod] Unavailable map: %s", s_CurrentMapName.c_str()));
					}
					s_CurrentMapName = ui_mapname->m_Current.m_String;
				});
			}

			std::this_thread::sleep_for(500ms);
		}

		return 0;
	}
}
