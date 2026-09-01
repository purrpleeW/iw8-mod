#include "common.hpp"
#include "discord/discord_rpc.hpp"
#include "game/game.hpp"

#define IS_IN_FRONTEND (g_Pointers->m_s_luaInFrontend == nullptr || *g_Pointers->m_s_luaInFrontend)

namespace Client {
	bool s_RPCInitalized = false;
	time_t s_RPCStartTime = 0;

	const char* GetMapName(const char* mapName) {
		IW8::mapInfo* info = g_Pointers->m_Com_GameInfo_GetMapInfoForLoadName(mapName);
		if (!info) {
			return "Unknown Map";
		}
		if (info->m_MapName[0] == '\x1F') {
			return "error";
		}
		return g_Pointers->m_SEH_StringEd_GetString(info->m_MapName);
	}

	const char* GetGameTypeName(const char* gameType) {
		IW8::gameTypeInfo* info = g_Pointers->m_Com_GameInfo_GetGameTypeForInternalName(gameType);
		if (!info) {
			return "Unknown Mode";
		}
		if (info->m_GameTypeName[0] == '\x1F') {
			return "error";
		}
		return g_Pointers->m_SEH_StringEd_GetString(info->m_GameTypeName);
	}

	void Discord::Initialize() {
		if (s_RPCInitalized) {
			return;
		}

		DiscordEventHandlers handlers;
		memset(&handlers, 0, sizeof(handlers));
		Discord_Initialize("1315310787010498592", &handlers, true, nullptr);
		s_RPCInitalized = true;
	}

	void Discord::Destroy() {
		if (!s_RPCInitalized) {
			return;
		}

		Discord_Shutdown();
		s_RPCInitalized = false;
	}
	
	void Discord::UpdateStatus(std::function<void(DiscordRichPresence*)> func) {
		DiscordRichPresence rpc;
		memset(&rpc, 0, sizeof(rpc));
		rpc.startTimestamp = s_RPCStartTime;
		func(&rpc);
		Discord_UpdatePresence(&rpc);
	}

	void Discord::Update() {
		if (!s_RPCInitalized) {
			return;
		}

		s_RPCStartTime = std::time(nullptr);

		UpdateStatus([](DiscordRichPresence* rpc) {
			rpc->partySize = g_Pointers->m_Dvar_GetIntSafe("NKSQNMMRRQ" /* party_partyPlayerCountNum */);
			rpc->partyMax = g_Pointers->m_Dvar_GetIntSafe("OOTQKOTRM" /* party_maxplayers */);

			rpc->button1name = "Discord";
			rpc->button1link = "https://discord.gg/dPzJajt";

			if (IS_IN_FRONTEND) {
				rpc->details = rpc->largeImageText = "In Menus";
				rpc->state = rpc->smallImageText = "Waiting";
				rpc->largeImageKey = "mw";
				rpc->smallImageKey = "";
			}
			else {
				const char* map = g_Pointers->m_Dvar_GetStringSafe("NSQLTTMRMP" /* ui_mapname */);
				const char* gameType = g_Pointers->m_Dvar_GetStringSafe("MOLPOSLOMO" /* ui_gametype */);

				rpc->details = rpc->largeImageText = GetMapName(map);
				rpc->state = rpc->smallImageText = GetGameTypeName(gameType);
				rpc->largeImageKey = map;
				rpc->smallImageKey = gameType;
			}
		});

		LOG("Discord", INFO, "Updated status.");
	}

	DWORD Discord::Thread(PVOID) {
		LOG("Discord", INFO, "Initializing Discord RPC...");
		Initialize();

		if (s_RPCInitalized == false) {
			LOG("Discord", INFO, "Failed to initialize Discord RPC.");
		}
		else {
			LOG("Discord", INFO, "Successfully initialized Discord RPC.");
			Update();
		}

		bool wasInFrontend = IS_IN_FRONTEND;
		while (s_RPCInitalized) {
			std::this_thread::sleep_for(1s);
			bool inFrontend = IS_IN_FRONTEND;
			if (wasInFrontend != inFrontend) {
				Update();
				wasInFrontend = inFrontend;
			}
		}

		return 0;
	}
}
