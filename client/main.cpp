#include "common.hpp"
#include "discord/discord_rpc.hpp"
#include "game/config.hpp"
#include "game/functions.hpp"
#include "game/game.hpp"
#include "game/map_validator.hpp"
#include "game/settings.hpp"
#include "hooks/hook.hpp"
#include "hooks/util/hook_util.hpp"
#include "memory/iat.hpp"
#include "network/updater.hpp"

#include <utility/flags.hpp>
#include <utility/nt.hpp>
#include <utility/memory.hpp>
#include <utility/strings.hpp>

#pragma warning(disable : 4189)
#pragma warning(disable : 4244)
#pragma warning(disable : 4302)

BOOL APIENTRY DllMain(HMODULE hMod, DWORD reason, PVOID) {
	using namespace Client;
	if (reason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(hMod);
		auto game = Common::Utility::NT::Library();

		g_Module = hMod;
		std::uint32_t gameChecksum = game.GetChecksum();
		if (g_GameVersions.contains(gameChecksum)) {
			g_GameIdentifier = g_GameVersions[gameChecksum];
		}

		Common::g_LogService = std::make_unique<Common::LogService>();
		Common::WinAPI::_SetConsoleTitle(std::format("iw8-mod: " GIT_DESCRIBE " - Call of Duty: Modern Warfare v{}", g_GameIdentifier.m_Version));
		g_GameModuleName = game.GetName();
		LOG("MainThread", INFO, "IW8-Mod injected. (base: 0x{:016X})", reinterpret_cast<std::size_t>(game.GetPtr()));

		if (Common::Utility::Flags::HasFlag("updatecheck")) {
			Network::Updater::CheckForUpdates();
		}

		if (remove("Data/data/CASCRepair.mrk") == 0) {
			LOG("MainThread", INFO, "Prevented TACT error E_REPAIR (28) by deleting CASCRepair.mrk.");
		}

		g_Hooks = std::make_unique<Hook::Hooks>();
		LOG("MainThread", INFO, "Hooks initialized.");

		g_Config.Load();

		Common::g_Console.m_OnInput = [](std::string input) {
			if (input == "enable debug info") {
				g_ShowDebugInfo = true;
				LOG("Console/OnInput", INFO, "Enabled debug info.");
			}
			else if (input == "disable debug info") {
				g_ShowDebugInfo = false;
				LOG("Console/OnInput", INFO, "Disabled debug info.");
			}
			else if (Common::Utility::Strings::StartsWith(input, "/")) {
				std::string cbuf = input.substr(1);
				std::vector<std::string> cmdParts = Common::Utility::Strings::Split(cbuf, ' ');
				std::string baseCmd = cmdParts.at(0);

				if (baseCmd == "openmenu") {
					if (cmdParts.size() >= 2) {
						std::string menuName = cmdParts.at(1);
						LOG("Console/OnInput", INFO, "Opening menu: {}", menuName);
						Hook::Util::g_GameThreadQueue.push_back([=]() {
							g_Pointers->m_LUI_OpenMenu(IW8::LocalClientNum_t::LOCAL_CLIENT_0, menuName.c_str(), true, false, false);
						});
					}
					else {
						LOG("Console/OnInput", INFO, "openmenu: A menu name must be supplied.");
					}
				}
				else if (baseCmd == "savesettings") {
					Settings::Save();
				}
				else if (baseCmd == "getdvarstring") {
					if (cmdParts.size() >= 2) {
						std::string dvarName = cmdParts.at(1);
						LOG("Console/OnInput", INFO, "{} = {}", dvarName, g_Pointers->m_Dvar_GetStringSafe(dvarName.c_str()));
					}
					else {
						LOG("Console/OnInput", INFO, "getdvarstring: A dvar name must be supplied.");
					}
				}
				else if (baseCmd == "glcsis") {
					if (cmdParts.size() >= 2) {
						if (cmdParts.at(1) == "true") {
							Hook::Util::g_ForceSignInState = true;
							LOG("Console/OnInput", INFO, "Forcing sign in state.");
						}
						else {
							Hook::Util::g_ForceSignInState = false;
							LOG("Console/OnInput", INFO, "Not forcing sign in state.");
						}
					}
					else {
						LOG("Console/OnInput", INFO, "CL_GetLocalClientSignInState(0) = {}", g_Pointers->m_CL_GetLocalClientSignInState(0));
					}
				}
				else if (baseCmd == "cmdaddr") {
					if (cmdParts.size() >= 2) {
						std::string commandName = cbuf.substr(baseCmd.size() + 1);

						bool foundCmd = false;
						IW8::cmd_function_s* cmd = (*g_Pointers->m_s_cmd_functions);
						while (cmd) {
							if (cmd->m_Name) {
								std::string name = Common::Utility::Strings::ToLower(cmd->m_Name);
								if (name == Common::Utility::Strings::ToLower(commandName)) {
									foundCmd = true;
									break;
								}
							}

							cmd = cmd->m_Next;
						}

						if (foundCmd) {
							LOG("Console/OnInput", INFO, "Address of \"{}\" -> {}", commandName, AndRel(reinterpret_cast<std::uintptr_t>(cmd->m_Function)));
						}
						else {
							LOG("Console/OnInput", INFO, "cmdaddr: Couldn't find command.");
						}
					}
					else {
						LOG("Console/OnInput", INFO, "cmdaddr: A command name must be supplied.");
					}
				}
				else if (baseCmd == "luiglobaladdr") {
					if (cmdParts.size() >= 2) {
						std::string functionName = cbuf.substr(baseCmd.size() + 1);

						bool foundFunc = false;
						IW8::LUIMethod* cmd = (*g_Pointers->m_LUIMethod_LUIGlobalPackage_list);
						while (cmd) {
							if (cmd->m_Name) {
								std::string name = Common::Utility::Strings::ToLower(cmd->m_Name);
								if (name == Common::Utility::Strings::ToLower(functionName)) {
									foundFunc = true;
									break;
								}
							}

							cmd = cmd->m_Next;
						}

						if (foundFunc) {
							LOG("Console/OnInput", INFO, "Address of \"{}\" -> {}", functionName, AndRel(reinterpret_cast<std::uintptr_t>(cmd->m_Function)));
						}
						else {
							LOG("Console/OnInput", INFO, "luiglobaladdr: Couldn't find function.");
						}
					}
					else {
						LOG("Console/OnInput", INFO, "luiglobaladdr: A function name must be supplied.");
					}
				}
				else if (baseCmd == "name") {
					if (cmdParts.size() >= 2) {
						std::string newName = cbuf.substr(baseCmd.size() + 1);
						LOG("Console/OnInput", INFO, "Setting name to \"{}\"", newName);
						g_Config.SetPlayerName(newName);
					}
					else {
						LOG("Console/OnInput", INFO, "name: A name must be supplied.");
					}
				}
				else if (baseCmd == "namesel") {
					Hook::Util::g_GameThreadQueue.push_back([=]() {
						g_Pointers->m_UI_ShowKeyboard(0, "Name", g_Config.GetPlayerName().c_str(), 64, false, false, true,
							IW8::UI_KEYBOARD_TYPE::UI_KEYBOARD_TYPE_NORMAL, Hook::Util::OnPlayerNameInput, true, true);
					});
				}
				else if (baseCmd == "bnetauthtest") {
					auto bn = g_Pointers->m_Unk_BNetClass;

					if (bn && bn->m_FinishedAuth) {
						std::uint8_t bn8 = reinterpret_cast<std::uint8_t>(bn);
						std::uint16_t bn16 = reinterpret_cast<std::uint16_t>(bn);

						auto v4 = bn->m_Var5;

						std::uint8_t v5_0 = (((bn8 - 12) ^ *((std::uint8_t*)bn + 764)) * (((bn8 - 12) ^ *((std::uint8_t*)bn + 764)) + 2)) ^ *((std::uint8_t*)bn + 756) ^ ((unsigned __int16)((((std::uint16_t)bn + 756) ^ *((std::uint16_t*)bn + 382)) * ((((std::uint16_t)bn + 756) ^ *((std::uint16_t*)bn + 382)) + 2)) >> 8);
						std::uint8_t v5_1 = (((bn8 - 11) ^ v4) * (((bn8 - 11) ^ v4) + 2)) ^ *((std::uint8_t*)bn + 757) ^ ((unsigned __int16)((((std::uint16_t)bn + 757) ^ v4) * ((((std::uint16_t)bn + 757) ^ v4) + 2)) >> 8);
						std::uint8_t v5_2 = (((bn8 - 10) ^ v4) * (((bn8 - 10) ^ v4) + 2)) ^ *((std::uint8_t*)bn + 758) ^ ((unsigned __int16)((((std::uint16_t)bn + 758) ^ v4) * ((((std::uint16_t)bn + 758) ^ v4) + 2)) >> 8);
						std::uint8_t v5_3 = (((bn8 - 12 + 3) ^ v4) * (((bn8 - 12 + 3) ^ v4) + 2)) ^ *((std::uint8_t*)bn + 759) ^ ((unsigned __int16)((((std::uint16_t)bn + 759) ^ v4) * ((((std::uint16_t)bn + 759) ^ v4) + 2)) >> 8);

						std::uint32_t v5 = static_cast<std::uint32_t>(v5_0 + (v5_1 << 8) + (v5_2 << 16) + (v5_3 << 24));

						LOG("Console/OnInput", INFO, "[prefix] v5 [{}] >> bn->m_Var4 [{}] == 1 = {} -> {}", v5, (int)bn->m_Var4, v5 >> bn->m_Var4, v5 >> bn->m_Var4 == 1 ? "true" : "false");
						bn->m_Var4 = std::bit_width(v5) - 1;
						LOG("Console/OnInput", INFO, "[postfix] v5 [{}] >> bn->m_Var4 [{}] == 1 = {} -> {}", v5, (int)bn->m_Var4, v5 >> bn->m_Var4, v5 >> bn->m_Var4 == 1 ? "true" : "false");
					}
					else {
						LOG("Console/OnInput", INFO, "bnetauthtest: Cross-auth needs to be finished before calling.");
					}
				}
				else if (baseCmd == "syslinktest") {
					LOG("Console/OnInput", INFO, "> SysLink test");

					Hook::Util::g_GameThreadQueue.push_back([=]() {
						Game::LuaShared_LuaCall_SetDvarBool(*g_Pointers->m_LUI_luaVM, "splitscreen", false);

						// start Lobby.SetupSystemLinkDvars()
						Game::LuaShared_LuaCall_SetDvarBool(*g_Pointers->m_LUI_luaVM, "LTSNLQNRKO" /* onlinegame */, false);
						Game::LuaShared_LuaCall_SetDvarBool(*g_Pointers->m_LUI_luaVM, "LSTLQTSSRM" /* xblive_privatematch */, false);
						Game::LuaShared_LuaCall_SetDvarBool(*g_Pointers->m_LUI_luaVM, "MQNNLTKNTS" /* ui_opensummary */, false);
						Game::LuaShared_LuaCall_SetDvarBool(*g_Pointers->m_LUI_luaVM, "LPSPMQSNPQ" /* systemlink */, true);
						// end Lobby.SetupSystemLinkDvars()

						Game::LUI_CoD_LuaCall_ExecNow(*g_Pointers->m_LUI_luaVM, "xstartprivateparty");

						g_Pointers->m_LUI_OpenMenu(IW8::LocalClientNum_t::LOCAL_CLIENT_0, "MPSystemLinkMenu", FALSE, FALSE, FALSE);
					});
				}
				else if (baseCmd == "unlockall") {
					Game::Cbuf_AddText("seta unlockAllItems 1");
					LOG("Console/OnInput", INFO, "All items should now be unlocked.");
				}
				else {
					Game::Cbuf_AddText(cbuf.c_str());
					LOG("Console/OnInput", INFO, "Executed command in Cbuf.");
				}
			}
			else {
				LOG("Console/OnInput", INFO, "Unknown input received: {}", input);
			}
		};
		Common::Console::StartInputThread();
	}
	else if (reason == DLL_PROCESS_DETACH) {
		Client::Discord::Destroy();
		g_Hooks.reset();
		g_Running = false;
	}

	return TRUE;
}

extern "C" {
	__declspec(dllexport) DWORD XInputGetCapabilities(DWORD dwUserIndex, DWORD dwFlags, struct XINPUT_CAPABILITIES* pCapabilities) {
		static auto func = Client::GetProxyExport<decltype(&XInputGetCapabilities)>("XInput9_1_0.dll", "XInputGetCapabilities");
		return func(dwUserIndex, dwFlags, pCapabilities);
	}

	__declspec(dllexport) DWORD XInputSetState(DWORD dwUserIndex, struct XINPUT_VIBRATION* pVibration) {
		static auto func = Client::GetProxyExport<decltype(&XInputSetState)>("XInput9_1_0.dll", "XInputSetState");
		return func(dwUserIndex, pVibration);
	}

	__declspec(dllexport) DWORD XInputGetState(DWORD dwUserIndex, struct XINPUT_STATE* pState) {
		static auto func = Client::GetProxyExport<decltype(&XInputGetState)>("XInput9_1_0.dll", "XInputGetState");
		return func(dwUserIndex, pState);
	}
}
