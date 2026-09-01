#pragma once
#include "engine/iw8/CachedAssets_t.hpp"
#include "engine/iw8/clientUIActive_t.hpp"
#include "engine/iw8/cmd_function_s.hpp"
#include "engine/iw8/CmdArgs.hpp"
#include "engine/iw8/DDLContext.hpp"
#include "engine/iw8/DDLState.hpp"
#include "engine/iw8/dvar_t.hpp"
#include "engine/iw8/FontGlowStyle.hpp"
#include "engine/iw8/GamerProfileData.hpp"
#include "engine/iw8/GamerProfileDataField.hpp"
#include "engine/iw8/gameTypeInfo.hpp"
#include "engine/iw8/gentity_s.hpp"
#include "engine/iw8/LocalUserPresenceData.hpp"
#include "engine/iw8/lua_State.hpp"
#include "engine/iw8/luaL_Reg.hpp"
#include "engine/iw8/LUIMethod.hpp"
#include "engine/iw8/mapInfo.hpp"
#include "engine/iw8/msg_t.hpp"
#include "engine/iw8/ScreenPlacement.hpp"
#include "engine/iw8/ScriptFile.hpp"
#include "engine/iw8/vec2_t.hpp"
#include "engine/iw8/vec3_t.hpp"
#include "engine/iw8/XAssetHeader.hpp"
#include "engine/iw8/unknown/BNetClass.hpp"
#include "engine/iw8/enums/DB_FastFileFailureMode.hpp"
#include "engine/iw8/enums/DWOnlineStatus.hpp"
#include "engine/iw8/enums/LocalClientNum_t.hpp"
#include "engine/iw8/enums/PartyHostType.hpp"
#include "engine/iw8/enums/StatsGroup.hpp"
#include "engine/iw8/enums/StatsSource.hpp"
#include "engine/iw8/enums/UI_KEYBOARD_TYPE.hpp"
#include "engine/iw8/enums/UI_KEYBOARD_RESULT.hpp"
#include "engine/iw8/enums/XAssetType.hpp"

namespace Client::Game::Functions {
	using AddBaseDrawTextCmdT = void*(const char* text, int maxChars, IW8::GfxFont* font, IW8::Material* fontMaterial, int fontHeight,
		float x, float y, float xScale, float yScale, char tracking, float rotation, const IW8::vec4_t* color, int cursorPos, char cursor, const IW8::FontGlowStyle* glowStyle,
		bool usePost, int allowGPadColor, int allowGPadAnySize, IW8::Material* iconsMaterial, bool requireUniqueCmd);
	using CG_WorldPosToScreenPosRealT = bool(IW8::LocalClientNum_t localClientNum, const IW8::ScreenPlacement* scrPlace, const IW8::vec3_t* worldPos, IW8::vec2_t* outScreenPos);
	using CL_GetLocalClientSignInStateT = int(int controllerIndex);
	using CL_PlayerData_GetDDLBufferT = bool(IW8::DDLContext* context, int controllerIndex, IW8::StatsSource statsSource, IW8::StatsGroup statsGroup);
	using Com_GameInfo_GetGameTypeForInternalNameT = IW8::gameTypeInfo* (const char* gameTypeName);
	using Com_GameInfo_GetMapInfoForLoadNameT = IW8::mapInfo* (const char* mapName);
	using Com_ParseNavStringsT = bool(const char* pStr, const char** navStrings, int navStringMax, int* navStringCount);
	using Com_PrintMessageInternalT = void(const int channelAndFlags, const char* msg, char error);
	using Com_SetErrorMessageT = void(const char* errorMessage);
	using Content_DoWeHaveContentPackT = bool(int contentPack);
	using DB_FindXAssetHeaderT = IW8::XAssetHeader(IW8::XAssetType type, const char* givenName, int allowCreateDefault);
	using DB_LoadXFileT = int(const char* zoneName, uintptr_t zoneMem, uintptr_t assetList, int zoneFlags, bool wasPaused, int failureMode, uintptr_t outSignature);
	using DB_Zones_PerformZoneLoadT = std::int64_t(bool processingPreloadedFiles, bool isBaseMap, bool wasPaused, IW8::DB_FastFileFailureMode failureMode);
	using DDL_GetEnumT = const char*(const IW8::DDLState* state, const IW8::DDLContext* ddlContext);
	using DDL_GetIntT = int(const IW8::DDLState* state, const IW8::DDLContext* ddlContext);
	using DDL_GetRootStateT = IW8::DDLState* (IW8::DDLState* result, const IW8::DDLDef* ddlDef);
	using DDL_GetStringT = const char*(const IW8::DDLState* state, const IW8::DDLContext* ddlContext);
	using DDL_MoveToPathT = bool(const IW8::DDLState* fromState, IW8::DDLState* toState, int depth, const char** path);
	using DDL_SetEnumT = void(IW8::DDLState* state, IW8::DDLContext* ddlContext, const char* val);
	using DDL_SetIntT = void(IW8::DDLState* state, IW8::DDLContext* ddlContext, int val);
	using DDL_SetStringT = void(IW8::DDLState* state, IW8::DDLContext* ddlContext, const char* val);
	using Dvar_FindVarByNameT = IW8::dvar_t*(const char* dvarName);
	using Dvar_GetIntSafeT = int(const char* dvarName);
	using Dvar_GetStringSafeT = const char*(const char* dvarName);
	using Dvar_RegisterBoolT = IW8::dvar_t*(const char* dvarName, bool value, std::uint32_t flags, const char* description);
	using dwGetLogOnStatusT = IW8::DWOnlineStatus(int controllerIndex);
	using DWServicesAccess__isReadyT = bool(void* _this, const int controllerIndex);
	using FS_ReadFileT = std::int64_t(const char* qpath, void** buffer);
	using GamerProfile_IsProfileLoggedInT = bool(int controllerIndex);
	using GamerProfile_GetDataByNameT = IW8::GamerProfileData*(IW8::GamerProfileData* result, int controllerIndex, const char* settingName);
	using GamerProfile_SetDataByNameT = void(int controllerIndex, const char* settingName, float settingValue);
	using I_atoui64T = std::uint64_t(const char* str);
	using I_atoui64_hexT = std::uint64_t(const char* str);
	using I_irandT = std::int64_t(int min, int max);
	using Info_ValueForKeyT = const char*(const char* s, const char* key);
	using j_vaT = const char*(const char* fmt, ...);
	using Live_GetLocalClientNameT = const char*();
	using Live_IsInSystemlinkLobbyT = bool();
	using Live_IsUserSignedInToDemonwareT = bool(int controllerIndex);
	using LiveStorage_AreStatsFetchedT = bool(int controllerIndex, IW8::StatsSource statsSource);
	using LiveStorage_ReadStatsT = void(int controllerIndex);
	using lua_createtableT = void(IW8::lua_State* L, int narray, int nrec);
	using lua_getfieldT = void(IW8::lua_State* L, int idx, const char* k);
	using lua_pushbooleanT = void(IW8::lua_State* L, int b);
	using lua_pushnilT = void(IW8::lua_State* L);
	using lua_pushstringT = void(IW8::lua_State* L, const char* str);
	using lua_removeT = void(IW8::lua_State* L, int idx);
	using lua_tobooleanT = bool(IW8::lua_State* L, int idx);
	using lua_tointeger32T = std::int64_t(IW8::lua_State* L, int idx);
	using lua_tolstringT = const char*(IW8::lua_State* L, int idx, std::size_t* len);
	using lua_tonumber32T = float(IW8::lua_State* L, int idx);
	using luaL_openlibT = void(IW8::lua_State* L, const char* libname, const IW8::luaL_Reg* l, std::uint32_t nup);
	using LuaShared_PCallT = bool(IW8::lua_State* luaVM, int nargs, int nresults);
	using LuaShared_SetTableIntT = void(const char* key, std::int64_t value, IW8::lua_State* luaVM);
	using LuaShared_SetTableStringT = void(const char* key, const char* value, IW8::lua_State* luaVM);
	using LUI_BeginTableT = void(const char* key, IW8::lua_State* luaVM);
	using LUI_EndTableT = void(IW8::lua_State* luaVM);
	using LUI_OpenMenuT = void(IW8::LocalClientNum_t localClientNum, const char* menuName, int isPopup, int isModal, int isExclusive);
	using MarketingCommsManager__GetMessageToDisplayCountT = std::uint64_t(void* _this, int messageType);
	using MSG_ReadInt64T = std::uint64_t(const IW8::msg_t* msg);
	using MSG_WriteInt64T = void(const IW8::msg_t* msg, const std::uint64_t value);
	using PartyHost_StartPrivatePartyT = void(IW8::LocalClientNum_t localClientNum, int localControllerIndex, bool currentlyActive, IW8::PartyHostType hostType);
	using ProcessScriptFileT = void(void* scrContext, IW8::ScriptFile* scriptFile);
	using R_AddCmdDrawTextT = void(const char* text, int maxChars, IW8::GfxFont* font, int fontHeight, float x, float y, float xScale, float yScale, float rotation,
		const IW8::vec4_t* color);
	using R_EndFrameT = void();
	using SEH_StringEd_GetStringT = const char*(const char* pszReference);
	using SV_Cmd_ArgvT = const char*(int argIndex);
	using SV_UpdateUserinfo_fT = void(void* cl);
	using Sys_MicrosecondsT = std::uint64_t();
	using UI_ShowKeyboardT = void(int controllerIndex, const char* title, const char* defaultText, int maxlen, bool verifyString, bool checkProfanity,
		bool allowEmpty, IW8::UI_KEYBOARD_TYPE type, void(__fastcall* callback)(IW8::LocalClientNum_t, IW8::UI_KEYBOARD_RESULT, const char*), bool openAsModal,
		bool useAutoAlign);
	using Unk_IsUnsupportedGPUT = bool();
	using Unk_IsUserSignedInToBnetT = bool();
}
