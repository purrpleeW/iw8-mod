#include "common.hpp"
#include "discord/discord_rpc.hpp"
#include "hooks/hook.hpp"
#include "hooks/util/hook_util.hpp"
#include "memory/memory.hpp"
#include "game/game.hpp"
#include "game/map_validator.hpp"
#include "ui/renderer.hpp"

#include <utility/flags.hpp>
#include <utility/hook.hpp>
#include <utility/memory.hpp>
#include <utility/nt.hpp>

namespace Client::Hook {
	Hooks::Hooks() {
		MH_Initialize();

		this->m_SetUnhandledExceptionFilterHK = new Memory::IAT("kernel32.dll", "SetUnhandledExceptionFilter");
		this->m_SetUnhandledExceptionFilterHK->Hook<HK_SetUnhandledExceptionFilter>();

		this->m_AddVectoredExceptionHandlerHK = new Memory::IAT("kernel32.dll", "AddVectoredExceptionHandler");
		this->m_AddVectoredExceptionHandlerHK->Hook<HK_AddVectoredExceptionHandler>();

		this->m_CreateFileAHK = new Memory::IAT("kernel32.dll", "CreateFileA");
		this->m_CreateFileAHK->Hook<HK_CreateFileA>();

		this->m_CheckRemoteDebuggerPresentHK = new Memory::MinHook("kernelbase.dll", "CheckRemoteDebuggerPresent");
		this->m_CheckRemoteDebuggerPresentHK->Hook<HK_CheckRemoteDebuggerPresent>();

		this->m_SetThreadContextHK = new Memory::MinHook("kernelbase.dll", "SetThreadContext");
		this->m_SetThreadContextHK->Hook<HK_SetThreadContext>();

		this->m_LoadImageAHK = new Memory::IAT("user32.dll", "LoadImageA");
		this->m_LoadImageAHK->Hook<HK_LoadImageA>();

		this->m_CloseSocketHK = new Memory::IAT("ws2_32.dll", "closesocket");
		this->m_CloseSocketHK->Hook<HK_CloseSocket>();

		this->m_ConnectHK = new Memory::IAT("ws2_32.dll", "connect");
		this->m_ConnectHK->Hook<HK_Connect>();

		this->m_FreeAddrInfoHK = new Memory::IAT("ws2_32.dll", "freeaddrinfo");
		this->m_FreeAddrInfoHK->Hook<HK_FreeAddrInfo>();

		this->m_GetAddrInfoHK = new Memory::IAT("ws2_32.dll", "getaddrinfo");
		this->m_GetAddrInfoHK->Hook<HK_GetAddrInfo>();

		this->m_GetHostByNameHK = new Memory::IAT("ws2_32.dll", "gethostbyname");
		this->m_GetHostByNameHK->Hook<HK_GetHostByName>();

		this->m_GetPeerNameHK = new Memory::IAT("ws2_32.dll", "getpeername");
		this->m_GetPeerNameHK->Hook<HK_GetPeerName>();

		this->m_GetSockNameHK = new Memory::IAT("ws2_32.dll", "getsockname");
		this->m_GetSockNameHK->Hook<HK_GetSockName>();

		this->m_IoctlSocketHK = new Memory::IAT("ws2_32.dll", "ioctlsocket");
		this->m_IoctlSocketHK->Hook<HK_IoctlSocket>();

		this->m_RecvHK = new Memory::IAT("ws2_32.dll", "recv");
		this->m_RecvHK->Hook<HK_Recv>();

		this->m_RecvFromHK = new Memory::IAT("ws2_32.dll", "recvfrom");
		this->m_RecvFromHK->Hook<HK_RecvFrom>();

		this->m_SelectHK = new Memory::IAT("ws2_32.dll", "select");
		this->m_SelectHK->Hook<HK_Select>();

		this->m_SendHK = new Memory::IAT("ws2_32.dll", "send");
		this->m_SendHK->Hook<HK_Send>();

		this->m_SendToHK = new Memory::IAT("ws2_32.dll", "sendto");
		this->m_SendToHK->Hook<HK_SendTo>();
	}

	void MysteryFunctionDetour() {
		LOG("Game/MysteryFunction", INFO, "Function called, ignoring.");
	}

	void GeneralTLSCallbackDetour() {
		// we don't actually do anything here or the console will be blown
		// up with messages saying the function was called. it's fine to do
		// that with "MysteryFunction" though, because it's only called
		// once (i think?)
		return;
	}

	void* Unk_DebugPrint_og = nullptr;
	__int64 Unk_DebugPrint_Det(IW8::lua_State* a1) {
		std::size_t strSz = 0;
		const char* str = g_Pointers->m_lua_tolstring(a1, 1, &strSz);
		LOG("Game/DebugPrint", DEBUG, "-> {}", std::string(str, strSz));
		return reinterpret_cast<decltype(&Unk_DebugPrint_Det)>(Unk_DebugPrint_og)(a1);
	}

	void Hooks::PostUnpack() {
		static Common::Utility::NT::Library game{};

		switch (g_GameIdentifier.m_Checksum) {
		case GameVersion::v1_20_4_7623265_REPLAY:
			Common::Utility::Hook::Jump(game.GetPtr() + 0x003061A0, MysteryFunctionDetour);
			Common::Utility::Hook::Nop(game.GetPtr() + 0x01504374, 5); // kill "Services aren't ready yet"
			break;
		case GameVersion::v1_20_4_7623265_SHIP:
			Common::Utility::Memory::SafeMemSet(game.GetPtr() + 0x03DF4548, MysteryFunctionDetour);
			break;
		case GameVersion::v1_38_3_9489393:
			//Common::Utility::Memory::SafeMemSet(game.GetPtr() + 0x202E7F55, MysteryFunctionDetour);
			//Common::Utility::Hook::Jump(game.GetPtr() + 0x000119CF, MysteryFunctionDetour);
			break;
		default:
			LOG("Hooks/PostUnpack", WARN, "Version not supported for mystery function detour!!!");
			break;
		}

		g_Pointers = std::make_unique<Game::Pointers>();
		LOG("MainThread", INFO, "Pointers initialized.");

		// I dont have the Arxan integrity signature for MW19, and the GTA one doesn't work (did occur in the binary though)
		// so I guess I'm waiting for someone to be so kind to find that lol
		std::vector<Memory::ScannedResult<void>> arxanChecks =
			//Memory::VectoredSigScan("48 8D 45 ? 48 89 45 ? 48 8D 05 ? ? ? ? 48 89 45", g_GameModuleName, "Arxan Integrity Checks");
			{};
		std::size_t currentCheck = 0;
		for (auto arxanCheck : arxanChecks) {
			currentCheck++;
			LOG("MainThread", DEBUG, "Patching Arxan Integrity Check {}/{}", currentCheck, arxanChecks.size());
			Common::Utility::Hook::Nop(arxanCheck.Add(0x8).Get(), 7);
		}

		/*
		CreateThread(nullptr, 0, Client::Discord::Thread, nullptr, 0, nullptr);
		if (Client::g_GameIdentifier.m_Checksum == Client::GameVersion::v1_20_4_7623265_REPLAY) {
			CreateThread(nullptr, 0, Client::Game::MapValidator::Thread, nullptr, 0, nullptr);
		}
		*/

		CreateThread(nullptr, 0, [](PVOID _thisPtr) -> DWORD {
			Hooks* _this = (Hooks*)_thisPtr;

			while (Hook::Util::g_WaitingToHook) {
				std::this_thread::sleep_for(100ms);
			}

			const auto tlsCallbacks = game.GetTlsCallbacks();
			for (Common::Utility::NT::Library::TlsCallback* callback : tlsCallbacks) {
				void* callbackOg = nullptr;
				(new Memory::MinHook(callback))->Hook(&GeneralTLSCallbackDetour, &callbackOg);
			}
			LOG("HookThread", DEBUG, "Hooked {} TLS callbacks.", tlsCallbacks.size());

			// test hook base+0x19CBDB0 (replay) / base+0x33239C0 (ship)
			if (GameVersionIsAny(GameVersion::v1_20_4_7623265_REPLAY)) {
				(new Memory::MinHook(game.GetPtr() + 0x19CBDB0))->Hook(Unk_DebugPrint_Det, &Unk_DebugPrint_og);
			}
			else if (GameVersionIsAny(GameVersion::v1_20_4_7623265_SHIP)) {
				(new Memory::MinHook(game.GetPtr() + 0x33239C0))->Hook(Unk_DebugPrint_Det, &Unk_DebugPrint_og);
			}

			_this->m_LuaHookStore.Register<HK_LuaShared_LuaCall_IsDemoBuild>();
			_this->m_LuaHookStore.Register<HK_LUI_CoD_LuaCall_ActivateInitialClient>();
			_this->m_LuaHookStore.Register<HK_LUI_CoD_LuaCall_CRMGetMessageContent>();
			_this->m_LuaHookStore.Register<HK_LUI_CoD_LuaCall_IsBattleNetLanOnly>();
			_this->m_LuaHookStore.Register<HK_LUI_CoD_LuaCall_NotifyServer>();
			_this->m_LuaHookStore.Register<HK_LUI_CoD_LuaCall_ShouldShowDebugInfo>();
			if (g_GameIdentifier.m_Ship) {
				_this->m_LuaHookStore.Register<HK_LUI_CoD_LuaCall_IsBattleNetAuthReady>();
				_this->m_LuaHookStore.Register<HK_LUI_CoD_LuaCall_IsConnectedToGameServer>();
				_this->m_LuaHookStore.Register<HK_LUI_CoD_LuaCall_IsGameModeAllowed>();
				_this->m_LuaHookStore.Register<HK_LUI_CoD_LuaCall_IsPremiumPlayer>();
				_this->m_LuaHookStore.Register<HK_LUI_CoD_LuaCall_IsPremiumPlayerReady>();
				_this->m_LuaHookStore.Register<HK_LUI_CoD_LuaCall_OfflineDataFetched>();
			}
			if (GameVersionIsAny(GameVersion::v1_38_3_9489393)) {
				_this->m_LuaHookStore.Register<HK_LUI_CoD_LuaCall_IsGameModeAvailable>();
			}

			_this->m_CL_GetLocalClientSignInStateHK = new Memory::MinHook(g_Pointers->m_CL_GetLocalClientSignInState);
			_this->m_CL_GetLocalClientSignInStateHK->Hook<HK_CL_GetLocalClientSignInState>();

			_this->m_Com_PrintMessageInternalHK = new Memory::MinHook(g_Pointers->m_Com_PrintMessageInternal);
			_this->m_Com_PrintMessageInternalHK->Hook<HK_Com_PrintMessageInternal>();

			_this->m_Content_DoWeHaveContentPackHK = new Memory::MinHook(g_Pointers->m_Content_DoWeHaveContentPack);
			//_this->m_Content_DoWeHaveContentPackHK->Hook<HK_Content_DoWeHaveContentPack>();

			_this->m_DB_FindXAssetHeaderHK = new Memory::MinHook(g_Pointers->m_DB_FindXAssetHeader);
			_this->m_DB_FindXAssetHeaderHK->Hook<HK_DB_FindXAssetHeader>();

			_this->m_DB_LoadXFileHK = new Memory::MinHook(g_Pointers->m_DB_LoadXFile);
			_this->m_DB_LoadXFileHK->Hook<HK_DB_LoadXFile>();

			_this->m_Dvar_RegisterBoolHK = new Memory::MinHook(g_Pointers->m_Dvar_RegisterBool);
			_this->m_Dvar_RegisterBoolHK->Hook<HK_Dvar_RegisterBool>();

			_this->m_dwGetLogOnStatusHK = new Memory::MinHook(g_Pointers->m_dwGetLogOnStatus);
			_this->m_dwGetLogOnStatusHK->Hook<HK_dwGetLogOnStatus>();

			_this->m_DWServicesAccess__isReadyHK = new Memory::MinHook(g_Pointers->m_DWServicesAccess__isReady);
			_this->m_DWServicesAccess__isReadyHK->Hook<HK_DWServicesAccess__isReady>();

			_this->m_GamerProfile_IsProfileLoggedInHK = new Memory::MinHook(g_Pointers->m_GamerProfile_IsProfileLoggedIn);
			//_this->m_GamerProfile_IsProfileLoggedInHK->Hook<HK_GamerProfile_IsProfileLoggedIn>();

			_this->m_Live_GetLocalClientNameHK = new Memory::MinHook(g_Pointers->m_Live_GetLocalClientName);
			_this->m_Live_GetLocalClientNameHK->Hook<HK_Live_GetLocalClientName>();

			_this->m_Live_IsInSystemlinkLobbyHK = new Memory::MinHook(g_Pointers->m_Live_IsInSystemlinkLobby);
			//_this->m_Live_IsInSystemlinkLobbyHK->Hook<HK_Live_IsInSystemlinkLobby>();

			_this->m_Live_IsUserSignedInToDemonwareHK = new Memory::MinHook(g_Pointers->m_Live_IsUserSignedInToDemonware);
			_this->m_Live_IsUserSignedInToDemonwareHK->Hook<HK_Live_IsUserSignedInToDemonware>();

			_this->m_luaL_openlibHK = new Memory::MinHook(g_Pointers->m_luaL_openlib);
			_this->m_luaL_openlibHK->Hook<HK_luaL_openlib>();

			_this->m_MarketingCommsManager__GetMessageToDisplayCountHK = new Memory::MinHook(g_Pointers->m_MarketingCommsManager__GetMessageToDisplayCount);
			_this->m_MarketingCommsManager__GetMessageToDisplayCountHK->Hook<HK_MarketingCommsManager__GetMessageToDisplayCount>();

			_this->m_PartyHost_StartPrivatePartyHK = new Memory::MinHook(g_Pointers->m_PartyHost_StartPrivateParty);
			_this->m_PartyHost_StartPrivatePartyHK->Hook<HK_PartyHost_StartPrivateParty>();

			_this->m_R_EndFrameHK = new Memory::MinHook(g_Pointers->m_R_EndFrame);
			_this->m_R_EndFrameHK->Hook<HK_R_EndFrame>();

			_this->m_SEH_StringEd_GetStringHK = new Memory::MinHook(g_Pointers->m_SEH_StringEd_GetString);
			_this->m_SEH_StringEd_GetStringHK->Hook<HK_SEH_StringEd_GetString>();

			_this->m_SV_UpdateUserinfo_fHK = new Memory::MinHook(g_Pointers->m_SV_UpdateUserinfo_f);
			_this->m_SV_UpdateUserinfo_fHK->Hook<HK_SV_UpdateUserinfo_f>();

			_this->m_UI_ShowKeyboardHK = new Memory::MinHook(g_Pointers->m_UI_ShowKeyboard);
			_this->m_UI_ShowKeyboardHK->Hook<HK_UI_ShowKeyboard>();

			_this->m_Unk_IsUnsupportedGPUHK = new Memory::MinHook(g_Pointers->m_Unk_IsUnsupportedGPU);
			_this->m_Unk_IsUnsupportedGPUHK->Hook<HK_Unk_IsUnsupportedGPU>();

			_this->m_Unk_IsUserSignedInToBnetHK = new Memory::MinHook(g_Pointers->m_Unk_IsUserSignedInToBnet);
			_this->m_Unk_IsUserSignedInToBnetHK->Hook<HK_Unk_IsUserSignedInToBnet>();

			Common::Utility::Hook::Nop(Memory::SigScan("E8 ? ? ? ? 8B 0D ? ? ? ? 8B 15", g_GameModuleName,
				"Lost Connection Fix #1").Get(), 5);
			Common::Utility::Hook::Nop(Memory::SigScan("E8 ? ? ? ? 80 3D ? ? ? ? ? 75 ? C6 05 ? ? ? ? ? E8", g_GameModuleName,
				"Lost Connection Fix #2").Get(), 5);
			Common::Utility::Hook::Nop(Memory::SigScan("E8 ? ? ? ? 33 D2 48 8D 4D ? 41 B8 ? ? ? ? E8 ? ? ? ? E8", g_GameModuleName,
				"Lost Connection Fix #3").Get(), 5);
			Common::Utility::Hook::Nop(Memory::SigScan("E8 ? ? ? ? 4C 8B 77 ? 48 C7 C6", g_GameModuleName,
				"Lost Connection Fix #4").Get(), 5);

			Hook::Util::g_HooksInitialised = true;

#			define FIX_THE_AMD_GPU_BUG FALSE

#			if FIX_THE_AMD_GPU_BUG
				if (!UI::Renderer::Init()) {
					LOG("HookThread", WARN, "DirectX 12 Renderer failed to initialise.");
					return 0;
				}

				_this->m_SwapChainHK = new Memory::MinHook(*g_Pointers->m_Unk_D3D12_SwapChain);
				_this->m_SwapChainHK->Hook<HK_SwapChainPresent>(8);
				_this->m_SwapChainHK->Hook<HK_SwapChainResizeBuffers>(13);

				UI::Renderer::AddRendererCallBack([] {
					// use for a console?
					// TODO: fix amd gpu bug or else this is unusable because i use an amd gpu because i have a severe skill issue if you are reading this you i
					//       am going mentally insane if you think you can help me become un skill issue then that would be greatly appreciated because as of writing
					//       this my gpu is already failing but i already know this is to do with imgui/hooking but i dont know how to fix it also coloured text is broken
					//       i know this because i tried making a console (ported from H1) in imgui and it failed horribly because of the amd gpu bug the amd gpu bug is
					//       going to drive me to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug
					//       to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug
					//       to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug
					//       to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug
					//       to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug
					//       to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug
					//       to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug
					//       to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug to amd gpu bug
					//       FIX THE AMD GPU BUG!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					/*ImGui::SetNextWindowSize({ 200.f, 200.f });
					if (ImGui::Begin("hi")) {
						ImGui::Text("hello from ImGui in DirectX 12!");
						ImGui::Text("res: %g x %g", ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);
						ImGui::Text("fps: %g", ImGui::GetIO().Framerate);
					}
					ImGui::End();*/
				}, 0);
#			endif

			return 0;
		}, this, 0, nullptr);
	}

	Hooks::~Hooks() {
		this->DeleteHook(&this->m_SwapChainHK, { 8, 13 });
		this->DeleteHook(&this->m_Unk_IsUserSignedInToBnetHK);
		this->DeleteHook(&this->m_Unk_IsUnsupportedGPUHK);
		this->DeleteHook(&this->m_UI_ShowKeyboardHK);
		this->DeleteHook(&this->m_SV_UpdateUserinfo_fHK);
		this->DeleteHook(&this->m_SEH_StringEd_GetStringHK);
		this->DeleteHook(&this->m_R_EndFrameHK);
		this->DeleteHook(&this->m_PartyHost_StartPrivatePartyHK);
		this->DeleteHook(&this->m_MarketingCommsManager__GetMessageToDisplayCountHK);
		this->DeleteHook(&this->m_luaL_openlibHK);
		this->DeleteHook(&this->m_Live_IsUserSignedInToDemonwareHK);
		this->DeleteHook(&this->m_Live_IsInSystemlinkLobbyHK);
		this->DeleteHook(&this->m_Live_GetLocalClientNameHK);
		this->DeleteHook(&this->m_GamerProfile_IsProfileLoggedInHK);
		this->DeleteHook(&this->m_DWServicesAccess__isReadyHK);
		this->DeleteHook(&this->m_dwGetLogOnStatusHK);
		this->DeleteHook(&this->m_Dvar_RegisterBoolHK);
		this->DeleteHook(&this->m_DB_LoadXFileHK);
		this->DeleteHook(&this->m_DB_FindXAssetHeaderHK);
		this->DeleteHook(&this->m_Content_DoWeHaveContentPackHK);
		this->DeleteHook(&this->m_Com_PrintMessageInternalHK);
		this->DeleteHook(&this->m_CL_GetLocalClientSignInStateHK);
		this->DeleteHook(&this->m_SendToHK);
		this->DeleteHook(&this->m_SendHK);
		this->DeleteHook(&this->m_SelectHK);
		this->DeleteHook(&this->m_RecvFromHK);
		this->DeleteHook(&this->m_RecvHK);
		this->DeleteHook(&this->m_IoctlSocketHK);
		this->DeleteHook(&this->m_GetSockNameHK);
		this->DeleteHook(&this->m_GetPeerNameHK);
		this->DeleteHook(&this->m_GetHostByNameHK);
		this->DeleteHook(&this->m_GetAddrInfoHK);
		this->DeleteHook(&this->m_FreeAddrInfoHK);
		this->DeleteHook(&this->m_ConnectHK);
		this->DeleteHook(&this->m_CloseSocketHK);
		this->DeleteHook(&this->m_LoadImageAHK);
		this->DeleteHook(&this->m_SetThreadContextHK);
		this->DeleteHook(&this->m_CheckRemoteDebuggerPresentHK);
		this->DeleteHook(&this->m_CreateFileAHK);
		this->DeleteHook(&this->m_AddVectoredExceptionHandlerHK);
		this->DeleteHook(&this->m_SetUnhandledExceptionFilterHK);

		g_Pointers.reset();
		LOG("MainThread", INFO, "Pointers uninitialized.");
	}
}
