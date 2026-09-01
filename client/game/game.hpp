#pragma once
#include "common.hpp"
#include "game/function_types.hpp"
#include "memory/scanned_result.hpp"

namespace Client {
	namespace Game {
		class Pointers {
		public:
			explicit Pointers();

			Functions::AddBaseDrawTextCmdT* m_AddBaseDrawTextCmd{};
			Functions::CG_WorldPosToScreenPosRealT* m_CG_WorldPosToScreenPosReal{};
			Functions::CL_GetLocalClientSignInStateT* m_CL_GetLocalClientSignInState{};
			Functions::CL_PlayerData_GetDDLBufferT* m_CL_PlayerData_GetDDLBuffer{};
			Functions::Com_GameInfo_GetGameTypeForInternalNameT* m_Com_GameInfo_GetGameTypeForInternalName{};
			Functions::Com_GameInfo_GetMapInfoForLoadNameT* m_Com_GameInfo_GetMapInfoForLoadName{};
			Functions::Com_ParseNavStringsT* m_Com_ParseNavStrings{};
			Functions::Com_PrintMessageInternalT* m_Com_PrintMessageInternal{};
			Functions::Com_SetErrorMessageT* m_Com_SetErrorMessage{};
			Functions::Content_DoWeHaveContentPackT* m_Content_DoWeHaveContentPack{};
			Functions::DB_FindXAssetHeaderT* m_DB_FindXAssetHeader{};
			Functions::DB_LoadXFileT* m_DB_LoadXFile{};
			Functions::DB_Zones_PerformZoneLoadT* m_DB_Zones_PerformZoneLoad{};
			Functions::DDL_GetEnumT* m_DDL_GetEnum{};
			Functions::DDL_GetIntT* m_DDL_GetInt{};
			Functions::DDL_GetRootStateT* m_DDL_GetRootState{};
			Functions::DDL_GetStringT* m_DDL_GetString{};
			Functions::DDL_MoveToPathT* m_DDL_MoveToPath{};
			Functions::DDL_SetEnumT* m_DDL_SetEnum{};
			Functions::DDL_SetIntT* m_DDL_SetInt{};
			Functions::DDL_SetStringT* m_DDL_SetString{};
			Functions::Dvar_FindVarByNameT* m_Dvar_FindVarByName{};
			Functions::Dvar_GetIntSafeT* m_Dvar_GetIntSafe{};
			Functions::Dvar_GetStringSafeT* m_Dvar_GetStringSafe{};
			Functions::Dvar_RegisterBoolT* m_Dvar_RegisterBool{};
			Functions::dwGetLogOnStatusT* m_dwGetLogOnStatus{};
			Functions::DWServicesAccess__isReadyT* m_DWServicesAccess__isReady{};
			Functions::FS_ReadFileT* m_FS_ReadFile{};
			Functions::GamerProfile_IsProfileLoggedInT* m_GamerProfile_IsProfileLoggedIn{};
			Functions::GamerProfile_GetDataByNameT* m_GamerProfile_GetDataByName{};
			Functions::GamerProfile_SetDataByNameT* m_GamerProfile_SetDataByName{};
			Functions::I_atoui64T* m_I_atoui64{};
			Functions::I_atoui64_hexT* m_I_atoui64_hex{};
			Functions::I_irandT* m_I_irand{};
			Functions::Info_ValueForKeyT* m_Info_ValueForKey{};
			Functions::j_vaT* m_j_va{};
			Functions::Live_GetLocalClientNameT* m_Live_GetLocalClientName{};
			Functions::Live_IsInSystemlinkLobbyT* m_Live_IsInSystemlinkLobby{};
			Functions::Live_IsUserSignedInToDemonwareT* m_Live_IsUserSignedInToDemonware{};
			Functions::LiveStorage_AreStatsFetchedT* m_LiveStorage_AreStatsFetched{};
			Functions::LiveStorage_ReadStatsT* m_LiveStorage_ReadStats{};
			Functions::lua_createtableT* m_lua_createtable{};
			Functions::lua_getfieldT* m_lua_getfield{};
			Functions::lua_pushbooleanT* m_lua_pushboolean{};
			Functions::lua_pushnilT* m_lua_pushnil{};
			Functions::lua_pushstringT* m_lua_pushstring{};
			Functions::lua_removeT* m_lua_remove{};
			Functions::lua_tobooleanT* m_lua_toboolean{};
			Functions::lua_tointeger32T* m_lua_tointeger32{};
			Functions::lua_tolstringT* m_lua_tolstring{};
			Functions::lua_tonumber32T* m_lua_tonumber32{};
			Functions::luaL_openlibT* m_luaL_openlib{};
			Functions::LuaShared_PCallT* m_LuaShared_PCall{};
			Functions::LuaShared_SetTableIntT* m_LuaShared_SetTableInt{};
			Functions::LuaShared_SetTableStringT* m_LuaShared_SetTableString{};
			Functions::LUI_BeginTableT* m_LUI_BeginTable{};
			Functions::LUI_EndTableT* m_LUI_EndTable{};
			Functions::LUI_OpenMenuT* m_LUI_OpenMenu{};
			Functions::MarketingCommsManager__GetMessageToDisplayCountT* m_MarketingCommsManager__GetMessageToDisplayCount{};
			Functions::MSG_ReadInt64T* m_MSG_ReadInt64{};
			Functions::MSG_WriteInt64T* m_MSG_WriteInt64{};
			Functions::PartyHost_StartPrivatePartyT* m_PartyHost_StartPrivateParty{};
			Functions::ProcessScriptFileT* m_ProcessScriptFile{};
			Functions::R_AddCmdDrawTextT* m_R_AddCmdDrawText{};
			Functions::R_EndFrameT* m_R_EndFrame{};
			Functions::SEH_StringEd_GetStringT* m_SEH_StringEd_GetString{};
			Functions::SV_Cmd_ArgvT* m_SV_Cmd_Argv{};
			Functions::SV_UpdateUserinfo_fT* m_SV_UpdateUserinfo_f{};
			Functions::Sys_MicrosecondsT* m_Sys_Microseconds{};
			Functions::UI_ShowKeyboardT* m_UI_ShowKeyboard{};
			Functions::Unk_IsUnsupportedGPUT* m_Unk_IsUnsupportedGPU{};
			Functions::Unk_IsUserSignedInToBnetT* m_Unk_IsUserSignedInToBnet{};

			IW8::clientUIActive_t* m_clientUIActives{};
			IW8::CmdArgs* m_cmd_args{};
			IW8::gentity_s** m_g_entities{};
			std::uint32_t* m_holdrand{};
			IW8::lua_State** m_LUI_luaVM{};
			IW8::LUIMethod** m_LUIMethod_LUIGlobalPackage_list{};
			IW8::GamerProfileDataField* m_PROFILE_DATA_FIELDS{};
			IW8::cmd_function_s** m_s_cmd_functions{};
			bool* m_s_isContentEnumerationFinished{};
			bool* m_s_luaInFrontend{};
			IW8::LocalUserPresenceData(*m_s_presenceData)[8] {};
			IW8::CachedAssets_t* m_sharedUiInfo_assets{};
			ID3D12CommandQueue** m_Unk_D3D12_CommandQueue{};
			IDXGISwapChain1** m_Unk_D3D12_SwapChain{};
			int* m_Unk_SignInState{};
			std::uint64_t* m_Unk_XUIDCheck1{};
			std::uint64_t* m_Unk_XUIDCheck2{};
			IW8::BNetClass* m_Unk_BNetClass{};

			void R_AddCmdDrawText(const char* text, int maxChars, IW8::GfxFont* font, int fontHeight, float x, float y, float xScale, float yScale, float rotation,
				const IW8::vec4_t* color)
			{
				if (this->m_R_AddCmdDrawText) {
					this->m_R_AddCmdDrawText(text, maxChars, font, fontHeight, x, y, xScale, yScale, rotation, color);
				}
				else if (this->m_AddBaseDrawTextCmd) {
					this->m_AddBaseDrawTextCmd(text, maxChars, font, nullptr, fontHeight, x, y, xScale, yScale, '\0', rotation, color, -1, '\0',
						nullptr, false, 0, 0, nullptr, false);
				}
			}
		};
	}
	inline std::unique_ptr<Game::Pointers> g_Pointers{};
}

#define MATERIAL_WHITE Client::g_Pointers->m_DB_FindXAssetHeader(Client::T9::XAssetType::ASSET_TYPE_MATERIAL, "white", true, -1).material
