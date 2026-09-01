#pragma once
#include "common.hpp"
#include "hooks/hook_types.hpp"
#include "memory/iat.hpp"
#include "memory/minhook.hpp"
#include "game/function_types.hpp"

namespace Client {
	namespace Hook {
		class Hooks {
		public:
			// Main hook - detects unpack state
			using HK_SetUnhandledExceptionFilter = HookPlate::StdcallHook<"kernel32/SetUnhandledExceptionFilter", LPTOP_LEVEL_EXCEPTION_FILTER,
				LPTOP_LEVEL_EXCEPTION_FILTER>;
			Memory::IAT* m_SetUnhandledExceptionFilterHK;

			// other IAT hooks - these can be called in DllMain for example (that's early!), nothing checks them
			using HK_AddVectoredExceptionHandler = HookPlate::StdcallHook<"kernel32/AddVectoredExceptionHandler", PVOID,
				ULONG, PVECTORED_EXCEPTION_HANDLER>;
			Memory::IAT* m_AddVectoredExceptionHandlerHK;

			using HK_CreateFileA = HookPlate::StdcallHook<"kernel32/CreateFileA", HANDLE,
				LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE>;
			Memory::IAT* m_CreateFileAHK;

			using HK_CheckRemoteDebuggerPresent = HookPlate::StdcallHook<"kernelbase/CheckRemoteDebuggerPresent", BOOL,
				HANDLE, PBOOL>;
			Memory::MinHook<>* m_CheckRemoteDebuggerPresentHK;

			using HK_SetThreadContext = HookPlate::StdcallHook<"kernelbase/SetThreadContext", BOOL,
				HANDLE, const CONTEXT*>;
			Memory::MinHook<>* m_SetThreadContextHK;

			using HK_LoadImageA = HookPlate::StdcallHook<"user32/LoadImageA", HANDLE,
				HINSTANCE, LPCSTR, UINT, int, int, UINT>;
			Memory::IAT* m_LoadImageAHK;

			using HK_CloseSocket = HookPlate::StdcallHook<"ws2_32/closesocket", int,
				SOCKET>;
			Memory::IAT* m_CloseSocketHK;

			using HK_Connect = HookPlate::StdcallHook<"ws2_32/connect", int,
				SOCKET, const sockaddr*, int>;
			Memory::IAT* m_ConnectHK;

			using HK_FreeAddrInfo = HookPlate::StdcallHook<"ws2_32/freeaddrinfo", void,
				PADDRINFOA>;
			Memory::IAT* m_FreeAddrInfoHK;

			using HK_GetAddrInfo = HookPlate::StdcallHook<"ws2_32/getaddrinfo", int,
				PCSTR, PCSTR, const ADDRINFOA*, PADDRINFOA*>;
			Memory::IAT* m_GetAddrInfoHK;

			using HK_GetHostByName = HookPlate::StdcallHook<"ws2_32/gethostbyname", hostent*,
				const char*>;
			Memory::IAT* m_GetHostByNameHK;

			using HK_GetPeerName = HookPlate::StdcallHook<"ws2_32/getpeername", int,
				SOCKET, sockaddr*, int*>;
			Memory::IAT* m_GetPeerNameHK;

			using HK_GetSockName = HookPlate::StdcallHook<"ws2_32/getsockname", int,
				SOCKET, sockaddr*, int*>;
			Memory::IAT* m_GetSockNameHK;

			using HK_IoctlSocket = HookPlate::StdcallHook<"ws2_32/ioctlsocket", int,
				SOCKET, long, u_long*>;
			Memory::IAT* m_IoctlSocketHK;

			using HK_Recv = HookPlate::StdcallHook<"ws2_32/recv", int,
				SOCKET, char*, int, int>;
			Memory::IAT* m_RecvHK;

			using HK_RecvFrom = HookPlate::StdcallHook<"ws2_32/recvfrom", int,
				SOCKET, char*, int, int, sockaddr*, int*>;
			Memory::IAT* m_RecvFromHK;

			using HK_Select = HookPlate::StdcallHook<"ws2_32/select", int,
				int, fd_set*, fd_set*, fd_set*, const timeval*>;
			Memory::IAT* m_SelectHK;

			using HK_Send = HookPlate::StdcallHook<"ws2_32/send", int,
				SOCKET, const char*, int, int>;
			Memory::IAT* m_SendHK;

			using HK_SendTo = HookPlate::StdcallHook<"ws2_32/sendto", int,
				SOCKET, const char*, int, int, sockaddr*, int>;
			Memory::IAT* m_SendToHK;

			// Lua - we register them, then they actually hook in Game/luaL_openlib
			HookPlate::LuaHookStore m_LuaHookStore{};
			using HK_LuaShared_LuaCall_IsDemoBuild = HookPlate::LuaHook<"LuaShared_LuaCall_IsDemoBuild", "Engine.BGAAHHAGAC">;
			using HK_LUI_CoD_LuaCall_ActivateInitialClient = HookPlate::LuaHook<"LUI_CoD_LuaCall_ActivateInitialClient", "Engine.CDGCBCBAJ">;
			using HK_LUI_CoD_LuaCall_CRMGetMessageContent = HookPlate::LuaHook<"LUI_CoD_LuaCall_CRMGetMessageContent", "Engine.CEGFGEGBEH">;
			using HK_LUI_CoD_LuaCall_IsBattleNetAuthReady = HookPlate::LuaHook<"LUI_CoD_LuaCall_IsBattleNetAuthReady", "Engine.JBIHDJBH">;
			using HK_LUI_CoD_LuaCall_IsBattleNetLanOnly = HookPlate::LuaHook<"LUI_CoD_LuaCall_IsBattleNetLanOnly", "Engine.BJGAADIDFH">;
			using HK_LUI_CoD_LuaCall_IsConnectedToGameServer = HookPlate::LuaHook<"LUI_CoD_LuaCall_IsConnectedToGameServer", "Engine.DHEJECBEE">;
			using HK_LUI_CoD_LuaCall_IsGameModeAllowed = HookPlate::LuaHook<"LUI_CoD_LuaCall_IsGameModeAllowed", "Engine.CEGDBDIIIE">;
			using HK_LUI_CoD_LuaCall_IsGameModeAvailable = HookPlate::LuaHook<"LUI_CoD_LuaCall_IsGameModeAvailable", "Engine.DBEGJIECGB">;
			using HK_LUI_CoD_LuaCall_IsPremiumPlayer = HookPlate::LuaHook<"LUI_CoD_LuaCall_IsPremiumPlayer", "Engine.CFHBIHABCB">;
			using HK_LUI_CoD_LuaCall_IsPremiumPlayerReady = HookPlate::LuaHook<"LUI_CoD_LuaCall_IsPremiumPlayerReady", "Engine.ECFHDAEIDA">;
			using HK_LUI_CoD_LuaCall_NotifyServer = HookPlate::LuaHook<"LUI_CoD_LuaCall_NotifyServer", "Engine.EBHIFJCGBH">;
			using HK_LUI_CoD_LuaCall_OfflineDataFetched = HookPlate::LuaHook<"LUI_CoD_LuaCall_OfflineDataFetched", "Fences.BGCHCGICDB">;
			using HK_LUI_CoD_LuaCall_ShouldShowDebugInfo = HookPlate::LuaHook<"LUI_CoD_LuaCall_ShouldShowDebugInfo", "Debug.CHFBFDCHBA">;

			// Game
			using HK_CL_GetLocalClientSignInState = HookPlate::FastcallHook<"CL_GetLocalClientSignInState", int,
				int>;
			Memory::MinHook<Game::Functions::CL_GetLocalClientSignInStateT>* m_CL_GetLocalClientSignInStateHK;

			using HK_Com_PrintMessageInternal = HookPlate::FastcallHook<"Com_PrintMessageInternal", void,
				const int, const char*, char>;
			Memory::MinHook<Game::Functions::Com_PrintMessageInternalT>* m_Com_PrintMessageInternalHK;

			using HK_Content_DoWeHaveContentPack = HookPlate::FastcallHook<"Content_DoWeHaveContentPack", bool,
				int>;
			Memory::MinHook<Game::Functions::Content_DoWeHaveContentPackT>* m_Content_DoWeHaveContentPackHK;

			using HK_DB_FindXAssetHeader = HookPlate::FastcallHook<"DB_FindXAssetHeader", IW8::XAssetHeader,
				IW8::XAssetType, const char*, int>;
			Memory::MinHook<Game::Functions::DB_FindXAssetHeaderT>* m_DB_FindXAssetHeaderHK;

			using HK_DB_LoadXFile = HookPlate::FastcallHook<"DB_LoadXFile", int,
				const char*, uintptr_t, uintptr_t, int, bool, int, uintptr_t>;
			Memory::MinHook<Game::Functions::DB_LoadXFileT>* m_DB_LoadXFileHK;

			using HK_Dvar_RegisterBool = HookPlate::FastcallHook<"Dvar_RegisterBool", IW8::dvar_t*,
				const char*, bool, std::uint32_t, const char*>;
			Memory::MinHook<Game::Functions::Dvar_RegisterBoolT>* m_Dvar_RegisterBoolHK;

			using HK_dwGetLogOnStatus = HookPlate::FastcallHook<"dwGetLogOnStatus", IW8::DWOnlineStatus,
				int>;
			Memory::MinHook<Game::Functions::dwGetLogOnStatusT>* m_dwGetLogOnStatusHK;

			using HK_DWServicesAccess__isReady = HookPlate::FastcallHook<"DWServicesAccess::isReady", bool,
				void*, const int>;
			Memory::MinHook<Game::Functions::DWServicesAccess__isReadyT>* m_DWServicesAccess__isReadyHK;

			using HK_GamerProfile_IsProfileLoggedIn = HookPlate::FastcallHook<"GamerProfile_IsProfileLoggedIn", bool,
				int>;
			Memory::MinHook<Game::Functions::GamerProfile_IsProfileLoggedInT>* m_GamerProfile_IsProfileLoggedInHK;

			using HK_Live_GetLocalClientName = HookPlate::FastcallHook<"Live_GetLocalClientName", const char*>;
			Memory::MinHook<Game::Functions::Live_GetLocalClientNameT>* m_Live_GetLocalClientNameHK;

			using HK_Live_IsInSystemlinkLobby = HookPlate::FastcallHook<"Live_IsInSystemlinkLobby", bool>;
			Memory::MinHook<Game::Functions::Live_IsInSystemlinkLobbyT>* m_Live_IsInSystemlinkLobbyHK;

			using HK_Live_IsUserSignedInToDemonware = HookPlate::FastcallHook<"Live_IsUserSignedInToDemonware", bool,
				int>;
			Memory::MinHook<Game::Functions::Live_IsUserSignedInToDemonwareT>* m_Live_IsUserSignedInToDemonwareHK;

			using HK_luaL_openlib = HookPlate::FastcallHook<"luaL_openlib", void,
				IW8::lua_State*, const char*, const IW8::luaL_Reg*, std::uint32_t>;
			Memory::MinHook<Game::Functions::luaL_openlibT>* m_luaL_openlibHK;

			using HK_MarketingCommsManager__GetMessageToDisplayCount = HookPlate::FastcallHook<"MarketingCommsManager::GetMessageToDisplayCount", std::uint64_t,
				void*, int>;
			Memory::MinHook<Game::Functions::MarketingCommsManager__GetMessageToDisplayCountT>* m_MarketingCommsManager__GetMessageToDisplayCountHK;

			using HK_PartyHost_StartPrivateParty = HookPlate::FastcallHook<"PartyHost_StartPrivateParty", void,
				int, int, bool, int>;
			Memory::MinHook<Game::Functions::PartyHost_StartPrivatePartyT>* m_PartyHost_StartPrivatePartyHK;

			using HK_R_EndFrame = HookPlate::FastcallHook<"R_EndFrame", void>;
			Memory::MinHook<Game::Functions::R_EndFrameT>* m_R_EndFrameHK;

			using HK_SEH_StringEd_GetString = HookPlate::FastcallHook<"SEH_StringEd_GetString", const char*,
				const char*>;
			Memory::MinHook<Game::Functions::SEH_StringEd_GetStringT>* m_SEH_StringEd_GetStringHK;

			using HK_SV_UpdateUserinfo_f = HookPlate::FastcallHook<"SV_UpdateUserinfo_f", void,
				void*>;
			Memory::MinHook<Game::Functions::SV_UpdateUserinfo_fT>* m_SV_UpdateUserinfo_fHK;

			using HK_UI_ShowKeyboard = HookPlate::FastcallHook<"UI_ShowKeyboard", void,
				int, const char*, const char*, int, bool, bool, bool, IW8::UI_KEYBOARD_TYPE,
				void(__fastcall*)(IW8::LocalClientNum_t, IW8::UI_KEYBOARD_RESULT, const char*), bool, bool>;
			Memory::MinHook<Game::Functions::UI_ShowKeyboardT>* m_UI_ShowKeyboardHK;

			using HK_Unk_IsUnsupportedGPU = HookPlate::FastcallHook<"Unk_IsUnsupportedGPU", bool>;
			Memory::MinHook<Game::Functions::Unk_IsUnsupportedGPUT>* m_Unk_IsUnsupportedGPUHK;

			using HK_Unk_IsUserSignedInToBnet = HookPlate::FastcallHook<"Unk_IsUserSignedInToBnet", bool>;
			Memory::MinHook<Game::Functions::Unk_IsUserSignedInToBnetT>* m_Unk_IsUserSignedInToBnetHK;

			// DirectX 12
			using HK_SwapChainPresent = HookPlate::FastcallHook<"SwapChain::Present", HRESULT,
				IDXGISwapChain1*, UINT, UINT>;
			using HK_SwapChainResizeBuffers = HookPlate::FastcallHook<"SwapChain::ResizeBuffers", HRESULT,
				IDXGISwapChain1*, UINT, UINT, UINT, DXGI_FORMAT, UINT>;
			Memory::MinHook<IDXGISwapChain1>* m_SwapChainHK;

			explicit Hooks();
			void PostUnpack();
			~Hooks();

			template <typename T>
			void DeleteHook(Memory::MinHook<T>** hook, std::vector<int> indexes = {}) {
				if (!hook || !*hook) {
					return;
				}

				if (indexes.empty()) {
					(*hook)->Unhook();
				}
				else {
					for (int index : indexes) {
						(*hook)->Unhook(index);
					}
				}

				delete *hook;
				*hook = nullptr;
			}

			void DeleteHook(Memory::IAT** hook) {
				if (!hook || !*hook) {
					return;
				}

				(*hook)->Unhook();

				delete *hook;
				*hook = nullptr;
			}
		};
	}

	inline std::unique_ptr<Hook::Hooks> g_Hooks{};
}
