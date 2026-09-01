#include "common.hpp"
#include "game/game.hpp"
#include "game/signature_store.hpp"
#include "memory/memory.hpp"
#include <utility/memory.hpp>
#include <utility/nt.hpp>

#define SETUP_POINTER(name) #name, (void**)&this->m_##name
#define SETUP_MOD(mod) [](Memory::ScannedResult<void> r) { return r.##mod##; }

namespace Client::Game {
	Pointers::Pointers() {
		SignatureStore batch;

		if (GameVersionIsAny(GameVersion::v1_20_4_7623265_SHIP)) {
			batch.Add(SETUP_POINTER(AddBaseDrawTextCmd), "E8 ? ? ? ? 48 8B D0 48 85 C0 74 ? 48 8B 8C 24", SETUP_MOD(Add(1).Rip()));
		}
		else if (GameVersionIsAny(GameVersion::v1_30_0_8403677, GameVersion::v1_38_3_9489393, GameVersion::v1_42_1_10125479, GameVersion::v1_44_0_10435696, GameVersion::v1_46_0_10750827)) {
			batch.Add(SETUP_POINTER(AddBaseDrawTextCmd), "E8 ? ? ? ? 48 8B D0 48 85 C0 0F 84 ? ? ? ? 48 8B 8C 24", SETUP_MOD(Add(1).Rip()));
		}

		batch.Add(SETUP_POINTER(CG_WorldPosToScreenPosReal), "E8 ? ? ? ? 4C 8D 4D ? 49 8B D6 4C 8D 45 E7", SETUP_MOD(Add(1).Rip()));

		batch.Add(SETUP_POINTER(CL_GetLocalClientSignInState), "E8 ? ? ? ? 85 C0 7F ? 8B CB", SETUP_MOD(Add(1).Rip()));

		batch.Add(SETUP_POINTER(CL_PlayerData_GetDDLBuffer), "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B E9 49 63 F8");

		if (!GameVersionIsNewerThan(GameVersion::v1_44_0_10435696)) {
			batch.Add(SETUP_POINTER(Com_GameInfo_GetGameTypeForInternalName), "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 8B 35 ? ? ? ? 45 33"
				" DB 48 8B E9");
		}
		else {
			batch.Add(SETUP_POINTER(Com_GameInfo_GetGameTypeForInternalName), "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 8B 35 ? ? ? ? 33 DB");
		}

		if (!GameVersionIsNewerThan(GameVersion::v1_44_0_10435696)) {
			batch.Add(SETUP_POINTER(Com_GameInfo_GetMapInfoForLoadName), "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 8B 3D ? ? ? ? 45 33 DB"
				" 48 8B E9");
		}
		else {
			batch.Add(SETUP_POINTER(Com_GameInfo_GetMapInfoForLoadName), "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 8B 2D");
		}

		batch.Add(SETUP_POINTER(Com_ParseNavStrings), "E8 ? ? ? ? 44 8B 44 24 ? 33 F6 45 85 C0 7E", SETUP_MOD(Add(1).Rip()));

		if (GameVersionIsAny(GameVersion::v1_20_4_7623265_REPLAY)) {
			batch.Add(SETUP_POINTER(Com_PrintMessageInternal), "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 41 8B F0 0F B7 F9");
		}

		batch.Add(SETUP_POINTER(Com_SetErrorMessage), "E8 ? ? ? ? EB ? 40 38 2D", SETUP_MOD(Add(1).Rip()));

		if (GameVersionIsAny(GameVersion::v1_30_0_8403677, GameVersion::v1_38_3_9489393, GameVersion::v1_42_1_10125479, GameVersion::v1_44_0_10435696, GameVersion::v1_46_0_10750827)) {
			batch.Add(SETUP_POINTER(Content_DoWeHaveContentPack), "8B D1 83 F9 ? 75");
		}

		batch.Add(SETUP_POINTER(DB_FindXAssetHeader), "E8 ? ? ? ? 44 8B C5 8D 4D", SETUP_MOD(Add(1).Rip()));

		if (GameVersionIsAny(GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_30_0_8403677)) {
			batch.Add(SETUP_POINTER(DB_LoadXFile), "E8 ? ? ? ? 8B F8 44 38 AB", SETUP_MOD(Add(1).Rip()));
		}
		else if (GameVersionIsAny(GameVersion::v1_38_3_9489393, GameVersion::v1_42_1_10125479, GameVersion::v1_44_0_10435696, GameVersion::v1_46_0_10750827)) {
			batch.Add(SETUP_POINTER(DB_LoadXFile), "E8 ? ? ? ? 8B F8 33 ED 40 38 B3", SETUP_MOD(Add(1).Rip()));
		}

		if (GameVersionIsAny(GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_30_0_8403677)) {
			batch.Add(SETUP_POINTER(DB_Zones_PerformZoneLoad), "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 45 8B F9 41 0F"
				" B6 F0");
		}
		else if (GameVersionIsAny(GameVersion::v1_38_3_9489393, GameVersion::v1_42_1_10125479, GameVersion::v1_44_0_10435696, GameVersion::v1_46_0_10750827)) {
			batch.Add(SETUP_POINTER(DB_Zones_PerformZoneLoad), "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 45 8B E1 41 0F"
				" B6 E8");
		}

		if (!GameVersionIsNewerThan(GameVersion::v1_44_0_10435696)) {
			batch.Add(SETUP_POINTER(DDL_GetEnum), "48 89 5C 24 ? 57 48 83 EC ? 48 8B FA 48 8B D9 E8 ? ? ? ? 84 C0 74 ? 4C 8B 4B ? 48 8B CF 41 8B 41 ? 99 41 F7"
				" 79 ? 45 8B 49 ? 8B 53 ? 44 8B C0 E8 ? ? ? ? EB ? 33 C0 8B D0 48 8B CB 48 8B 5C 24");
		}
		else {
			batch.Add(SETUP_POINTER(DDL_GetEnum), "E8 ? ? ? ? 48 63 CB 4C 8B C0", SETUP_MOD(Add(1).Rip()));
		}

		if (!GameVersionIsNewerThan(GameVersion::v1_44_0_10435696)) {
			batch.Add(SETUP_POINTER(DDL_GetInt), "48 89 5C 24 ? 57 48 83 EC ? 48 8B FA 48 8B D9 E8 ? ? ? ? 84 C0 74 ? 4C 8B 4B ? 48 8B CF 41 8B 41 ? 99 41 F7 79"
				" ? 45 8B 49 ? 8B 53 ? 44 8B C0 E8 ? ? ? ? EB ? 33 C0 48 8B 4B");
		}
		else {
			batch.Add(SETUP_POINTER(DDL_GetInt), "E8 ? ? ? ? 44 85 E3", SETUP_MOD(Add(1).Rip()));
		}

		batch.Add(SETUP_POINTER(DDL_GetRootState), "33 C0 C7 41 ? ? ? ? ? 48 89 41 ? 89 41 ? 48 8B C1 48 89 51");

		batch.Add(SETUP_POINTER(DDL_GetString), "40 55 56 48 83 EC ? 48 8B EA");

		if (!GameVersionIsNewerThan(GameVersion::v1_44_0_10435696)) {
			batch.Add(SETUP_POINTER(DDL_MoveToPath), "48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 4D 8B F1 48 8B FA 48 85 C9 74 ? 48 85 D2 74 ? 48 8B 41 ? 48"
				" 89 42 ? 48 8B 41 ? 48 89 42 ? 8B 41 ? 89 42 ? 8B 41 ? 89 42 ? 0F B6 01 88 02 48 89 5C 24 ? 49 63 F0 45 85 C0 7E ? 33 DB 48 8B CF E8 ? ? ? ? 4D"
				" 8B 04 DE");
		}
		else {
			batch.Add(SETUP_POINTER(DDL_MoveToPath), "E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 48 8B D7 48 8B CD", SETUP_MOD(Add(1).Rip()));
		}

		batch.Add(SETUP_POINTER(DDL_SetEnum), "48 89 5C 24 ? 57 48 83 EC ? 48 8B FA 48 8B D9 33 D2 49 8B C8");

		batch.Add(SETUP_POINTER(DDL_SetInt), "E9 ? ? ? ? 85 C9 75 ? 8D 42", SETUP_MOD(Add(1).Rip()));

		if (!GameVersionIsNewerThan(GameVersion::v1_44_0_10435696)) {
			batch.Add(SETUP_POINTER(DDL_SetString), "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 49 8B D8 48 8B F2 48 8B F9 E8");
		}
		else {
			batch.Add(SETUP_POINTER(DDL_SetString), "E8 ? ? ? ? 8B CD E8 ? ? ? ? 48 81 C4", SETUP_MOD(Add(1).Rip()));
		}

		batch.Add(SETUP_POINTER(Dvar_FindVarByName), "E8 ? ? ? ? 48 8B CB 48 63 50", SETUP_MOD(Add(1).Rip()));

		batch.Add(SETUP_POINTER(Dvar_GetIntSafe), "E8 ? ? ? ? 8B D0 85 C0 75 ? 38 05", SETUP_MOD(Add(1).Rip()));

		batch.Add(SETUP_POINTER(Dvar_GetStringSafe), "48 83 EC ? E8 ? ? ? ? 8B C8 E8 ? ? ? ? 48 85 C0 75 ? 48 8D 05 ? ? ? ? 48 83 C4 ? C3 80 78");

		batch.Add(SETUP_POINTER(Dvar_RegisterBool), "E8 ? ? ? ? 48 8B F0 F6 46", SETUP_MOD(Add(1).Rip()));

		if (GameVersionIsAny(GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_30_0_8403677, GameVersion::v1_38_3_9489393)) {
			batch.Add(SETUP_POINTER(dwGetLogOnStatus), "40 53 48 83 EC ? 48 63 C1 BA");
		}
		else if (GameVersionIsAny(GameVersion::v1_42_1_10125479, GameVersion::v1_44_0_10435696, GameVersion::v1_46_0_10750827)) {
			batch.Add(SETUP_POINTER(dwGetLogOnStatus), "40 53 48 83 EC ? 48 63 C1 BA ? ? ? ? 48 69 D8");
		}

		batch.Add(SETUP_POINTER(DWServicesAccess__isReady), "48 83 EC ? 85 D2 78 ? 48 63 C2 48 3B 81 ? ? ? ? 73 ? 48 69 C0 ? ? ? ? 48 81 C1 ? ? ? ? 48 89 5C"
			" 24 ? 48 03 C8 32 DB 48 8B 01");

		batch.Add(SETUP_POINTER(FS_ReadFile), "40 53 57 41 57 48 83 EC ? 48 8B FA");

		if (GameVersionIsAny(GameVersion::v1_20_4_7623265_REPLAY)) {
			batch.Add(SETUP_POINTER(GamerProfile_IsProfileLoggedIn), "E8 ? ? ? ? 84 C0 75 ? E8 ? ? ? ? 84 C0 75 ? 33 C0 48 83 C4", SETUP_MOD(Add(1).Rip()));
		}
		else if (GameVersionIsAny(GameVersion::v1_38_3_9489393)) {
			batch.Add(SETUP_POINTER(GamerProfile_IsProfileLoggedIn), "E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 38 1D ? ? ? ? 4C 89 A4 24", SETUP_MOD(Add(1).Rip()));
		}

		batch.Add(SETUP_POINTER(GamerProfile_GetDataByName), "40 53 48 83 EC ? 48 63 C2 48 8B D9");

		batch.Add(SETUP_POINTER(GamerProfile_SetDataByName), "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 8B F1 0F 29 74 24");

		batch.Add(SETUP_POINTER(I_atoui64), "E8 ? ? ? ? 48 8D 4C 24 ? 48 89 84 24", SETUP_MOD(Add(1).Rip()));

		batch.Add(SETUP_POINTER(I_atoui64_hex), "E8 ? ? ? ? BE ? ? ? ? 48 8B E8", SETUP_MOD(Add(1).Rip()));

		batch.Add(SETUP_POINTER(I_irand), "69 05 ? ? ? ? ? ? ? ? 2B D1 48 63 D2");

		batch.Add(SETUP_POINTER(Info_ValueForKey), "48 89 74 24 ? 57 48 83 EC ? 48 8B FA 48 8B F1 48 85 C9 74");

		batch.Add(SETUP_POINTER(j_va), "48 89 4C 24 ? 48 89 54 24 ? 4C 89 44 24 ? 4C 89 4C 24 ? 53 48 83 EC ? B9");

		batch.Add(SETUP_POINTER(Live_GetLocalClientName), "E8 ? ? ? ? 4C 8B 4B ? 48 8D 0D", SETUP_MOD(Add(1).Rip()));

		if (GameVersionIsAny(GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_30_0_8403677, GameVersion::v1_38_3_9489393, GameVersion::v1_42_1_10125479, GameVersion::v1_44_0_10435696, GameVersion::v1_46_0_10750827)) {
			batch.Add(SETUP_POINTER(Live_IsInSystemlinkLobby), "E8 ? ? ? ? 84 C0 75 ? 45 84 FF", SETUP_MOD(Add(1).Rip()));
		}

		if (!GameVersionIsNewerThan(GameVersion::v1_44_0_10435696)) {
			batch.Add(SETUP_POINTER(Live_IsUserSignedInToDemonware), "E8 ? ? ? ? 84 C0 74 ? 4C 8D 43 ? 8B D7", SETUP_MOD(Add(1).Rip()));
		}
		else {
			batch.Add(SETUP_POINTER(Live_IsUserSignedInToDemonware), "E8 ? ? ? ? 83 4F ? ? 48 8D 0D", SETUP_MOD(Add(1).Rip()));
		}

		batch.Add(SETUP_POINTER(LiveStorage_AreStatsFetched), "E8 ? ? ? ? 84 C0 75 ? 8B CB E8 ? ? ? ? 8B CB E8 ? ? ? ? 84 C0 74", SETUP_MOD(Add(1).Rip()));

		if (GameVersionIsAny(GameVersion::v1_20_4_7623265_REPLAY)) {
			// this is an offline only function
			// todo: reimplement this entire function
			batch.Add(SETUP_POINTER(LiveStorage_ReadStats), "40 53 48 83 EC ? 48 63 D9 8B CB E8 ? ? ? ? B9");
		}

		batch.Add(SETUP_POINTER(lua_createtable), "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 4C 8B 49 ? 41 8B F8");

		batch.Add(SETUP_POINTER(lua_getfield), "48 89 5C 24 ? 57 48 83 EC ? 4D 8B D0 48 8B D9 E8 ? ? ? ? 48 8B F8 49 C7 C0 ? ? ? ? 90 49 FF C0 43 80 3C 02"
			" ? 75 ? 49 8B D2 48 8B CB E8 ? ? ? ? 48 B9 ? ? ? ? ? ? ? ? 4C 8D 44 24 ? 48 0B C1 48 8B D7 48 8B CB 48 89 44 24 ? E8 ? ? ? ? 48 85 C0");

		batch.Add(SETUP_POINTER(lua_pushboolean), "E8 ? ? ? ? EB ? 85 D2 78", SETUP_MOD(Add(1).Rip()));

		batch.Add(SETUP_POINTER(lua_pushnil), "48 8B 41 ? 48 C7 00");

		batch.Add(SETUP_POINTER(lua_pushstring), "48 89 5C 24 ? 57 48 83 EC ? 48 8B FA 48 8B D9 48 85 D2 75 ? 48 8B 41");

		if (!GameVersionIsNewerThan(GameVersion::v1_44_0_10435696)) {
			batch.Add(SETUP_POINTER(lua_remove), "4C 8B C1 85 D2 7E ? 8D 42 ? 48 63 D0 48 8B 41 ? 48 8B 49");
		}
		else {
			batch.Add(SETUP_POINTER(lua_remove), "4C 8B C1 85 D2 7E ? 48 8B 41 ? 48 8B 49");
		}

		batch.Add(SETUP_POINTER(lua_toboolean), "48 83 EC ? E8 ? ? ? ? 48 8B 08 33 C0");

		batch.Add(SETUP_POINTER(lua_tointeger32), "E8 ? ? ? ? 8B F0 80 FB", SETUP_MOD(Add(1).Rip()));

		batch.Add(SETUP_POINTER(lua_tolstring), "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 49 8B F8 8B DA 48 8B F1");

		if (!GameVersionIsNewerThan(GameVersion::v1_44_0_10435696)) {
			batch.Add(SETUP_POINTER(lua_tonumber32), "40 53 48 83 EC ? 0F 29 74 24 ? 48 8B D9 E8 ? ? ? ? 48 8D 4C 24");
		}
		else {
			batch.Add(SETUP_POINTER(lua_tonumber32), "E8 ? ? ? ? 0F 57 F6 8B D3", SETUP_MOD(Add(1).Rip()));
		}

		batch.Add(SETUP_POINTER(luaL_openlib), "48 89 5C 24 ? 55 56 41 56 48 83 EC ? 48 8B 41");

		if (GameVersionIsAny(GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP)) {
			batch.Add(SETUP_POINTER(LuaShared_PCall), "E8 ? ? ? ? 8B E8 F7 D6", SETUP_MOD(Add(1).Rip()));
		}
		else if (GameVersionIsAny(GameVersion::v1_30_0_8403677, GameVersion::v1_38_3_9489393, GameVersion::v1_42_1_10125479, GameVersion::v1_44_0_10435696, GameVersion::v1_46_0_10750827)) {
			batch.Add(SETUP_POINTER(LuaShared_PCall), "E8 ? ? ? ? 8B F8 85 C0 74 ? 4C 8D 44 24", SETUP_MOD(Add(1).Rip()));
		}

		batch.Add(SETUP_POINTER(LuaShared_SetTableInt), "E8 ? ? ? ? 45 85 F6 74 ? 4C 8B C3", SETUP_MOD(Add(1).Rip()));

		batch.Add(SETUP_POINTER(LuaShared_SetTableString), "E8 ? ? ? ? B0 ? EB ? BA", SETUP_MOD(Add(1).Rip()));

		batch.Add(SETUP_POINTER(LUI_BeginTable), "E8 ? ? ? ? 85 ED 74 ? BB", SETUP_MOD(Add(1).Rip()));

		if (GameVersionIsAny(GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP)) {
			batch.Add(SETUP_POINTER(LUI_EndTable), "E8 ? ? ? ? 44 3B 76", SETUP_MOD(Add(1).Rip()));
		}
		else if (GameVersionIsAny(GameVersion::v1_30_0_8403677, GameVersion::v1_38_3_9489393, GameVersion::v1_42_1_10125479, GameVersion::v1_44_0_10435696, GameVersion::v1_46_0_10750827)) {
			batch.Add(SETUP_POINTER(LUI_EndTable), "E8 ? ? ? ? 8B DF 3B BE", SETUP_MOD(Add(1).Rip()));
		}

		batch.Add(SETUP_POINTER(LUI_OpenMenu), "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 41 8B F1 41 8B D8");

		batch.Add(SETUP_POINTER(MarketingCommsManager__GetMessageToDisplayCount), "40 57 4C 8B 91");

		batch.Add(SETUP_POINTER(MSG_ReadInt64), "40 56 57 41 56 48 83 EC ? 8B 51 ? 48 8B F9 8B 71 ? 8B 41 ? 03 C6 44 8D 72 ? C1 E0 ? 44 3B F0 7E ? C7 01 ?"
			" ? ? ? 33 C0");

		batch.Add(SETUP_POINTER(MSG_WriteInt64), "4C 8B C9 8B 49 ? 45 8B 41 ? 8D 41 ? 41 C1 E0 ? 41 3B C0 7E ? 41 C7 01 ? ? ? ? C3 41 83 39 ? 75 ? F6 C1 ?"
			" 74 ? 4C 8B C2");

		batch.Add(SETUP_POINTER(PartyHost_StartPrivateParty), "E8 ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ? 33 C9 85 C0", SETUP_MOD(Add(1).Rip()));

		if (!GameVersionIsNewerThan(GameVersion::v1_44_0_10435696)) {
			batch.Add(SETUP_POINTER(ProcessScriptFile), "48 89 5C 24 ? 48 89 74 24 ? 55 57 41 54 41 56 41 57 48 8B EC 48 83 EC ? 8B 5A");
		}
		else {
			batch.Add(SETUP_POINTER(ProcessScriptFile), "40 53 55 56 57 41 55 41 56 41 57 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 8B 5A");
		}

		if (GameVersionIsAny(GameVersion::v1_20_4_7623265_REPLAY)) {
			batch.Add(SETUP_POINTER(R_AddCmdDrawText), "E8 ? ? ? ? 48 8B 05 ? ? ? ? 0F 28 DF", SETUP_MOD(Add(1).Rip()));
		}

		if (GameVersionIsAny(GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_30_0_8403677)) {
			batch.Add(SETUP_POINTER(R_EndFrame), "48 8B 15 ? ? ? ? 45 33 D2 4C 8B 0D");
		}
		else if (GameVersionIsAny(GameVersion::v1_38_3_9489393, GameVersion::v1_42_1_10125479, GameVersion::v1_44_0_10435696, GameVersion::v1_46_0_10750827)) {
			batch.Add(SETUP_POINTER(R_EndFrame), "48 83 EC ? E8 ? ? ? ? 48 8B 15 ? ? ? ? 45 33 D2");
		}

		if (GameVersionIsAny(GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_30_0_8403677)) {
			batch.Add(SETUP_POINTER(SEH_StringEd_GetString), "E8 ? ? ? ? 48 8B CB 48 8B F8 E8 ? ? ? ? 48 8B D0 48 8B CF E8 ? ? ? ? 4C 8B 05 ? ? ? ? 48 8D"
				" 0D ? ? ? ? 48 8B D0 E8 ? ? ? ? 48 8B 0D", SETUP_MOD(Add(1).Rip()));
		}
		else if (GameVersionIsAny(GameVersion::v1_38_3_9489393, GameVersion::v1_42_1_10125479, GameVersion::v1_44_0_10435696)) {
			batch.Add(SETUP_POINTER(SEH_StringEd_GetString), "E8 ? ? ? ? 48 8B D0 80 FB", SETUP_MOD(Add(1).Rip()));
		}
		else if (GameVersionIsAny(GameVersion::v1_46_0_10750827)) {
//#			error scan for SEH_StringEd_GetString [1.46]
		}

		if (GameVersionIsAny(GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP)) {
			batch.Add(SETUP_POINTER(SV_Cmd_Argv), "E8 ? ? ? ? 4C 8B D0 4C 2B D3", SETUP_MOD(Add(1).Rip()));
		}
		else if (GameVersionIsAny(GameVersion::v1_30_0_8403677, GameVersion::v1_38_3_9489393, GameVersion::v1_42_1_10125479, GameVersion::v1_44_0_10435696)) {
			batch.Add(SETUP_POINTER(SV_Cmd_Argv), "E8 ? ? ? ? 48 89 44 24 ? 33 F6 0F", SETUP_MOD(Add(1).Rip()));
		}
		else if (GameVersionIsAny(GameVersion::v1_46_0_10750827)) {
			batch.Add(SETUP_POINTER(SV_Cmd_Argv), "E8 ? ? ? ? 48 8B 1D ? ? ? ? 48 8D 3D ? ? ? ? 48 8B E8", SETUP_MOD(Add(1).Rip()));
		}

		batch.Add(SETUP_POINTER(SV_UpdateUserinfo_f), "40 53 48 83 EC ? 48 8B D9 B9 ? ? ? ? E8 ? ? ? ? 48 8D 8B");

		if (!GameVersionIsNewerThan(GameVersion::v1_44_0_10435696)) {
			batch.Add(SETUP_POINTER(Sys_Microseconds), "83 3D ? ? ? ? ? 49 B8 ? ? ? ? ? ? ? ? F2 0F 10 15");
		}
		else {
			batch.Add(SETUP_POINTER(Sys_Microseconds), "E8 ? ? ? ? 48 2B C3 48 8B C8", SETUP_MOD(Add(1).Rip()));
		}

		batch.Add(SETUP_POINTER(UI_ShowKeyboard), "48 8B 44 24 ? 48 89 05 ? ? ? ? 0F B6 44 24");

		if (GameVersionIsAny(GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP)) {
			batch.Add(SETUP_POINTER(Unk_IsUnsupportedGPU), "48 83 EC ? 0F B6 05 ? ? ? ? 85 C0 75");
		}

		if (GameVersionIsAny(GameVersion::v1_42_1_10125479, GameVersion::v1_44_0_10435696)) {
			batch.Add(SETUP_POINTER(Unk_IsUserSignedInToBnet), "40 53 48 83 EC ? 48 8B DA E8 ? ? ? ? 83 38 ? 75 ? E8 ? ? ? ? 84 C0");
		}

		batch.Add(SETUP_POINTER(clientUIActives), "48 8D 15 ? ? ? ? 8B 4C 10", SETUP_MOD(Add(3).Rip()));

		if (!GameVersionIsNewerThan(GameVersion::v1_44_0_10435696)) {
			batch.Add(SETUP_POINTER(cmd_args), "48 63 05 ? ? ? ? 33 C9 41 89 AC 87", SETUP_MOD(Add(3).Rip()));
		}
		else {
			batch.Add(SETUP_POINTER(cmd_args), "48 63 05 ? ? ? ? 33 C9 45 89 B4 84", SETUP_MOD(Add(3).Rip()));
		}

		batch.Add(SETUP_POINTER(g_entities), "48 8B 0D ? ? ? ? 8B F2 4C 69 E0", SETUP_MOD(Add(3).Rip()));

		batch.Add(SETUP_POINTER(holdrand), "89 05 ? ? ? ? C1 E8 ? F3 48 0F 2A C0", SETUP_MOD(Add(2).Rip()));

		if (GameVersionIsAny(GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP)) {
			batch.Add(SETUP_POINTER(LUI_luaVM), "4C 8B 05 ? ? ? ? 48 8D 0D ? ? ? ? 48 8B D0 E8 ? ? ? ? BA ? ? ? ? 48 8B CE E8 ? ? ? ? 4D 8D 7F",
				SETUP_MOD(Add(3).Rip()));
		}
		else if (GameVersionIsAny(GameVersion::v1_30_0_8403677, GameVersion::v1_38_3_9489393, GameVersion::v1_42_1_10125479, GameVersion::v1_44_0_10435696, GameVersion::v1_46_0_10750827)) {
			batch.Add(SETUP_POINTER(LUI_luaVM), "48 8B 05 ? ? ? ? 45 33 C0 44 8B 4C 24 ? 48 89 44 24 ? 48 8B 5C 24", SETUP_MOD(Add(3).Rip()));
		}

		if (GameVersionIsAny(GameVersion::v1_20_4_7623265_REPLAY)) {
			batch.Add(SETUP_POINTER(LUIMethod_LUIGlobalPackage_list), "48 8B 1D ? ? ? ? 48 8B 3D ? ? ? ? 48 85 DB", SETUP_MOD(Add(3).Rip()));
		}
		else if (GameVersionIsAny(GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_30_0_8403677, GameVersion::v1_38_3_9489393, GameVersion::v1_42_1_10125479, GameVersion::v1_44_0_10435696, GameVersion::v1_46_0_10750827)) {
			batch.Add(SETUP_POINTER(LUIMethod_LUIGlobalPackage_list), "48 8B 1D ? ? ? ? 48 8B 3D ? ? ? ? 89 05", SETUP_MOD(Add(3).Rip()));
		}

		batch.Add(SETUP_POINTER(PROFILE_DATA_FIELDS), "48 8D 0D ? ? ? ? 80 7C D1", SETUP_MOD(Add(3).Rip()));

		if (!GameVersionIsNewerThan(GameVersion::v1_44_0_10435696)) {
			batch.Add(SETUP_POINTER(s_cmd_functions), "48 89 0D ? ? ? ? 48 8B 53", SETUP_MOD(Add(3).Rip()));
		}
		else {
			batch.Add(SETUP_POINTER(s_cmd_functions), "48 8B 3D ? ? ? ? 48 8D 35 ? ? ? ? 4C 8B F0 48 85 FF 0F 84 ? ? ? ? 66 0F 1F 84 00 ? ? ? ? 4C 8B 4F",
				SETUP_MOD(Add(3).Rip()));
		}

		batch.Add(SETUP_POINTER(s_isContentEnumerationFinished), "80 3D ? ? ? ? ? 74 ? 48 89 7C 24", SETUP_MOD(Add(2).Rip().Add(1)));

		batch.Add(SETUP_POINTER(s_luaInFrontend), "0F B6 05 ? ? ? ? 75", SETUP_MOD(Add(3).Rip()));

		if (!GameVersionIsNewerThan(GameVersion::v1_44_0_10435696)) {
			batch.Add(SETUP_POINTER(s_presenceData), "48 8D 05 ? ? ? ? 4C 8D 05 ? ? ? ? 48 39 08 74 ? FF C2 48 05", SETUP_MOD(Add(3).Rip()));
		}
		else {
//#			error scan for s_presenceData [1.46]
		}

		if (GameVersionIsAny(GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP, GameVersion::v1_30_0_8403677)) {
			batch.Add(SETUP_POINTER(sharedUiInfo_assets), "48 89 05 ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ? B2 ? 48 89 05 ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ? 33 D2",
				SETUP_MOD(Add(3).Rip()));
		}
		else if (GameVersionIsAny(GameVersion::v1_38_3_9489393, GameVersion::v1_42_1_10125479, GameVersion::v1_44_0_10435696, GameVersion::v1_46_0_10750827)) {
			batch.Add(SETUP_POINTER(sharedUiInfo_assets), "48 89 05 ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ? B2 ? 48 89 05 ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ? 80 3D",
				SETUP_MOD(Add(3).Rip()));
		}

		batch.Add(SETUP_POINTER(Unk_D3D12_CommandQueue), "48 8B 0D ? ? ? ? 4C 8B 44 24", SETUP_MOD(Add(3).Rip()));

		batch.Add(SETUP_POINTER(Unk_D3D12_SwapChain), "48 8B 1D ? ? ? ? 44 8B C7", SETUP_MOD(Add(3).Rip()));

		if (!GameVersionIsNewerThan(GameVersion::v1_44_0_10435696)) {
			batch.Add(SETUP_POINTER(Unk_SignInState), "83 3D ? ? ? ? ? 7E ? 33 C9", SETUP_MOD(Add(2).Rip().Add(1)));
		}
		else {
			batch.Add(SETUP_POINTER(Unk_SignInState), "E8 ? ? ? ? 85 C0 7F ? 8B CB", SETUP_MOD(Add(1).Rip().Add(14).Rip()));
		}

		// 1-game_test -> xenonUserData.m_guardedUserData[0].xuid
		if (GameVersionIsAny(GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP)) {
			batch.Add(SETUP_POINTER(Unk_XUIDCheck1), "48 8D 3D ? ? ? ? 0F 1F 44 00 ? 48 63 C3", SETUP_MOD(Add(3).Rip()));
		}
		else if (GameVersionIsAny(GameVersion::v1_30_0_8403677, GameVersion::v1_38_3_9489393)) {
			batch.Add(SETUP_POINTER(Unk_XUIDCheck1), "48 8D 15 ? ? ? ? 48 FF C6 81 FF", SETUP_MOD(Add(3).Rip()));
		}
		else if (GameVersionIsAny(GameVersion::v1_42_1_10125479, GameVersion::v1_44_0_10435696, GameVersion::v1_46_0_10750827)) {
			batch.Add(SETUP_POINTER(Unk_XUIDCheck1), "48 8D 1D ? ? ? ? 40 88 35", SETUP_MOD(Add(3).Rip()));
		}

		if (GameVersionIsAny(GameVersion::v1_20_4_7623265_REPLAY)) {
			batch.Add(SETUP_POINTER(Unk_XUIDCheck2), "48 8D 0D ? ? ? ? E8 ? ? ? ? 4C 8D 0D ? ? ? ? 48 89 44 24", SETUP_MOD(Add(3).Rip()));
		}

		batch.Add(SETUP_POINTER(Unk_BNetClass), "83 3D ? ? ? ? ? 74 ? B8 ? ? ? ? C3", SETUP_MOD(Add(2).Rip().Add(1)));
		
		batch.ScanAll();
	}
}