#include "common.hpp"
#include "game/config.hpp"
#include "game/game.hpp"
#include "hooks/util/hook_util.hpp"

#include <utility/nt.hpp>

namespace Client::Hook {
	void* Util::InitDbgHelp() {
		static class InitDbgHelp_ {
		public:
			InitDbgHelp_() {
				SymInitialize(GetCurrentProcess(), nullptr, FALSE);
			}

			~InitDbgHelp_() {
				SymCleanup(GetCurrentProcess());
			}
		} initDbgHelp;
		return &initDbgHelp;
	}

	LONG WINAPI Util::TopLevelExceptionFilter(EXCEPTION_POINTERS* inf) {
		(void)InitDbgHelp();

		const char* exceptionCodeName = "<unknown>";
		switch (inf->ExceptionRecord->ExceptionCode) {
		case EXCEPTION_ACCESS_VIOLATION:
			exceptionCodeName = "Access violation";
			break;
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
			exceptionCodeName = "Array bounds exceeded";
			break;
		case EXCEPTION_BREAKPOINT:
			exceptionCodeName = "Breakpoint";
			break;
		case EXCEPTION_DATATYPE_MISALIGNMENT:
			exceptionCodeName = "Data type misalignment";
			break;
		case EXCEPTION_FLT_DENORMAL_OPERAND:
			exceptionCodeName = "Float denormal operand";
			break;
		case EXCEPTION_FLT_DIVIDE_BY_ZERO:
			exceptionCodeName = "Float / 0";
			break;
		case EXCEPTION_FLT_INEXACT_RESULT:
			exceptionCodeName = "Float inexact result";
			break;
		case EXCEPTION_FLT_INVALID_OPERATION:
			exceptionCodeName = "Float invalid operation";
			break;
		case EXCEPTION_FLT_OVERFLOW:
			exceptionCodeName = "Float overflow";
			break;
		case EXCEPTION_FLT_STACK_CHECK:
			exceptionCodeName = "Float stack check";
			break;
		case EXCEPTION_FLT_UNDERFLOW:
			exceptionCodeName = "Float underflow";
			break;
		case EXCEPTION_ILLEGAL_INSTRUCTION:
			exceptionCodeName = "Illegal instruction";
			break;
		case EXCEPTION_IN_PAGE_ERROR:
			exceptionCodeName = "In-page error";
			break;
		case EXCEPTION_INT_DIVIDE_BY_ZERO:
			exceptionCodeName = "Int / 0";
			break;
		case EXCEPTION_INT_OVERFLOW:
			exceptionCodeName = "Integer overflow";
			break;
		case EXCEPTION_INVALID_DISPOSITION:
			exceptionCodeName = "Invalid disposition";
			break;
		case EXCEPTION_NONCONTINUABLE_EXCEPTION:
			exceptionCodeName = "Non-continuable exception";
			break;
		case EXCEPTION_PRIV_INSTRUCTION:
			exceptionCodeName = "Private instruction";
			break;
		case EXCEPTION_SINGLE_STEP:
			exceptionCodeName = "Single step";
			break;
		case EXCEPTION_STACK_OVERFLOW:
			exceptionCodeName = "Stack overflow";
			break;
		}

		Common::Utility::NT::Library gameLib = Common::Utility::NT::Library();
		Common::Utility::NT::Library modLib = Common::Utility::NT::Library(Client::g_Module);

		std::string message = "Unhandled exception caught. Instead of giving you absolutely nothing:\n";
		message += std::format("{} region: 0x{:08X} -> 0x{:08X}\n", gameLib.GetName(),
			reinterpret_cast<std::size_t>(gameLib.GetPtr()), reinterpret_cast<std::size_t>(gameLib.GetPtr()) + gameLib.GetOptionalHeader()->SizeOfImage);
		message += std::format("{} region: 0x{:08X} -> 0x{:08X}\n", modLib.GetName(),
			reinterpret_cast<std::size_t>(modLib.GetPtr()), reinterpret_cast<std::size_t>(modLib.GetPtr()) + modLib.GetOptionalHeader()->SizeOfImage);
		message += "---- Exception Record ----\n";
		message += std::format("Exception code: {} (0x{:08X})\n", exceptionCodeName, inf->ExceptionRecord->ExceptionCode);
		message += std::format("Exception flags: 0x{:08X}\n", inf->ExceptionRecord->ExceptionFlags);
		message += std::format("Exception address: {}\n", AndRel(reinterpret_cast<std::size_t>(inf->ExceptionRecord->ExceptionAddress)));
		message += std::format("Number of parameters: {}\n", inf->ExceptionRecord->NumberParameters);
		for (auto x = 0ul; x < std::min(inf->ExceptionRecord->NumberParameters, 15ul /*EXCEPTION_MAXIMUM_PARAMETERS*/); x++) {
			message += std::format(" > Parameter {}: {}\n", x, AndRel(inf->ExceptionRecord->ExceptionInformation[x]));
		}
		message += "----- Context Record -----\n";
		message += std::format("P1Home: {}\n", AndRel(inf->ContextRecord->P1Home));
		message += std::format("P2Home: {}\n", AndRel(inf->ContextRecord->P2Home));
		message += std::format("P3Home: {}\n", AndRel(inf->ContextRecord->P3Home));
		message += std::format("P4Home: {}\n", AndRel(inf->ContextRecord->P4Home));
		message += std::format("P5Home: {}\n", AndRel(inf->ContextRecord->P5Home));
		message += std::format("P6Home: {}\n", AndRel(inf->ContextRecord->P6Home));

		message += std::format("Context flags: 0x{:08X}\n", inf->ContextRecord->ContextFlags);
		message += std::format("MxCsr: 0x{:08X}\n", inf->ContextRecord->MxCsr);

		message += std::format("SegCs: 0x{:04X}\n", inf->ContextRecord->SegCs);
		message += std::format("SegDs: 0x{:04X}\n", inf->ContextRecord->SegDs);
		message += std::format("SegEs: 0x{:04X}\n", inf->ContextRecord->SegEs);
		message += std::format("SegFs: 0x{:04X}\n", inf->ContextRecord->SegFs);
		message += std::format("SegGs: 0x{:04X}\n", inf->ContextRecord->SegGs);
		message += std::format("SegSs: 0x{:04X}\n", inf->ContextRecord->SegSs);
		message += std::format("EFlags: 0x{:08X}\n", inf->ContextRecord->EFlags);

		message += std::format("Dr0: {}\n", AndRel(inf->ContextRecord->Dr0));
		message += std::format("Dr1: {}\n", AndRel(inf->ContextRecord->Dr1));
		message += std::format("Dr2: {}\n", AndRel(inf->ContextRecord->Dr2));
		message += std::format("Dr3: {}\n", AndRel(inf->ContextRecord->Dr3));
		message += std::format("Dr6: {}\n", AndRel(inf->ContextRecord->Dr6));
		message += std::format("Dr7: {}\n", AndRel(inf->ContextRecord->Dr7));

		message += std::format("Rax: {}\n", AndRel(inf->ContextRecord->Rax));
		message += std::format("Rcx: {}\n", AndRel(inf->ContextRecord->Rcx));
		message += std::format("Rdx: {}\n", AndRel(inf->ContextRecord->Rdx));
		message += std::format("Rbx: {}\n", AndRel(inf->ContextRecord->Rbx));
		message += std::format("Rsp: {}\n", AndRel(inf->ContextRecord->Rsp));
		message += std::format("Rbp: {}\n", AndRel(inf->ContextRecord->Rbp));
		message += std::format("Rsi: {}\n", AndRel(inf->ContextRecord->Rsi));
		message += std::format("Rdi: {}\n", AndRel(inf->ContextRecord->Rdi));
		message += std::format("R8: {}\n", AndRel(inf->ContextRecord->R8));
		message += std::format("R9: {}\n", AndRel(inf->ContextRecord->R9));
		message += std::format("R10: {}\n", AndRel(inf->ContextRecord->R10));
		message += std::format("R11: {}\n", AndRel(inf->ContextRecord->R11));
		message += std::format("R12: {}\n", AndRel(inf->ContextRecord->R12));
		message += std::format("R13: {}\n", AndRel(inf->ContextRecord->R13));
		message += std::format("R14: {}\n", AndRel(inf->ContextRecord->R14));
		message += std::format("R15: {}\n", AndRel(inf->ContextRecord->R15));

		message += std::format("Rip: {}\n", AndRel(inf->ContextRecord->Rip));

		message += std::format("VectorControl: {}\n", AndRel(inf->ContextRecord->VectorControl));

		message += std::format("DebugControl: {}\n", AndRel(inf->ContextRecord->DebugControl));
		message += std::format("LastBranchToRip: {}\n", AndRel(inf->ContextRecord->LastBranchToRip));
		message += std::format("LastBranchFromRip: {}\n", AndRel(inf->ContextRecord->LastBranchFromRip));
		message += std::format("LastExceptionToRip: {}\n", AndRel(inf->ContextRecord->LastExceptionToRip));
		message += std::format("LastExceptionFromRip: {}\n", AndRel(inf->ContextRecord->LastExceptionFromRip));

		message += "------- Other Info -------\n";
		STACKFRAME64 stackFrame = { 0 };
		stackFrame.AddrPC.Offset = inf->ContextRecord->Rip;
		stackFrame.AddrPC.Mode = AddrModeFlat;
		stackFrame.AddrFrame.Offset = inf->ContextRecord->Rbp;
		stackFrame.AddrFrame.Mode = AddrModeFlat;
		stackFrame.AddrStack.Offset = inf->ContextRecord->Rsp;
		stackFrame.AddrStack.Mode = AddrModeFlat;

		std::size_t functionCount = 0;
		while (StackWalk64(IMAGE_FILE_MACHINE_AMD64, GetCurrentProcess(), GetCurrentThread(),
			&stackFrame, inf->ContextRecord, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL))
		{
			functionCount++;
			message += std::format("Function address [{}]: {}\n", functionCount, AndRel(stackFrame.AddrPC.Offset));
		}

		MessageBoxA(nullptr, message.c_str(), "iw8-mod exception handler", MB_OK);
		return EXCEPTION_EXECUTE_HANDLER;
	}

	void Util::OnPlayerNameInput(IW8::LocalClientNum_t localClientNum, IW8::UI_KEYBOARD_RESULT res, const char* text) {
		_Unreferenced_parameter_(localClientNum);
		_Unreferenced_parameter_(res);

		if (text == nullptr) {
			LOG("OnPlayerNameInput", DEBUG, "Text was null, cancelling.");
			return;
		}

		g_Config.SetPlayerName(text);
	}

	void Util::InitialiseCRM() {
		g_PatchedCRM[IW8::Lua::CRMLocation::MP_MOTD] = {
			IW8::Lua::CRMContent()
			.SetTitle("^1Welcome to iw8-mod!^7")
			.SetContentLong("This mod is a work-in-progress and should not be demonstrated as a finished product.\\n"
				"\\n"
				"Any bugs or errors found should be reported to ^4@lifix^7 or ^4@xifil^7 on Discord.\\n"
				"\\n"
				"^7https://discord.gg/dPzJajt")
			.SetLayoutType(0)
			.SetLocationID(1)
		};
		
		g_PatchedCRM[IW8::Lua::CRMLocation::MP_MSGS] = {
			IW8::Lua::CRMContent()
			.SetTitle("Connecting with friends")
			.SetContentLong("Radmin VPN or Hamachi is still currently required as there is no working server browser or master server."
				" After connecting with other players through this software, you will see their lobbies in your server list.")
			.SetLayoutType(0)
			.SetLocationID(1),
			IW8::Lua::CRMContent()
			.SetTitle("Help wanted")
			.SetContentLong("iw8-mod is mainly developed by one developer - me, @lifix. If you have any knowledge in C++/Reverse Engineering then "
				"please contribute on GitHub or GitLab, it would help not only me but the greater community too.")
			.SetLayoutType(0)
			.SetLocationID(1)
		};

		g_PatchedCRM[IW8::Lua::CRMLocation::PATCH_NOTES] = {
			IW8::Lua::CRMContent()
			.SetTitle("16/03/25 Changelog")
			.SetContentShort("- added CRM hooks (patch notes, motd, messages, etc.)\\n"
				"^1- known issue: patch notes duplicate")
			.SetLayoutType(0)
			.SetLocationID(1),
			IW8::Lua::CRMContent()
			.SetTitle("15/03/25 Changelog")
			.SetContentShort("- removed invalid map message (it was buggy)\\n"
				"- add /luiglobaladdr to get address of global LUI functions (doesn't work on 1.38 and 1.44 curiously?????)\\n"
				"- removed arbitrary hooking wait\\n"
				"- player name now saved to game directory\\n"
				"- only hook Com_PrintMessageInternal if the -dbg flag is set (can crash the game and only useful sometimes so yea)\\n"
				"- never show \"Go Online\" button in main menu (it was still appearing on replay)\\n"
				"- added /bnetauthtest for testing purposes")
			.SetLayoutType(0)
			.SetLocationID(1),
			IW8::Lua::CRMContent()
			.SetTitle("14/03/25 Changelog")
			.SetContentShort("- added /cmdaddr to get address of commands\\n"
				"- tried to add a dx12 renderer, doesn't work because of AMD GPU bug (someone fix please)\\n"
				"- add logging on replay (Com_PrintMessageInternal)")
			.SetLayoutType(0)
			.SetLocationID(1),
			IW8::Lua::CRMContent()
			.SetTitle("10/03/25 Changelog")
			.SetContentShort("- fixed warnings\\n"
				"- fixed map validator\\n"
				"- added auto copy+launch scripts (iw8-mod/tools/game/)")
			.SetLayoutType(0)
			.SetLocationID(1),
			IW8::Lua::CRMContent()
			.SetTitle("09/03/25 Changelog")
			.SetContentShort("- added watermark to ship builds\\n"
				"- fixed ForceSignInState\\n"
				"- added name input\\n"
				"- trying to connect to online services now doesn't say you aren't signed in\\n"
				"- fixed github workflow\\n"
				"- named some auth pointers")
			.SetLayoutType(0)
			.SetLocationID(1),
			IW8::Lua::CRMContent()
			.SetTitle("08/03/25 Changelog")
			.SetContentShort("- failed to add an auto-updater (again)")
			.SetLayoutType(0)
			.SetLocationID(1),
			IW8::Lua::CRMContent()
			.SetTitle("07/03/25 Changelog")
			.SetContentShort("- improved signature scan speed by over 4x\\n"
				"- failed to add an auto-updater")
			.SetLayoutType(0)
			.SetLocationID(1),
			IW8::Lua::CRMContent()
			.SetTitle("06/03/25 Changelog")
			.SetContentShort("- added -luidump launch flag, this gets the address of every LUI function and throws them into a file called \"luidump.txt\""
				" in your game directory")
			.SetLayoutType(0)
			.SetLocationID(1),
			IW8::Lua::CRMContent()
			.SetTitle("05/03/25 Changelog")
			.SetContentShort("- add _ReturnAddress intrinsic to fix build issues\\n"
				"- standardised AndRel for relative locations\\n"
				"- started work on -luidump launch flag")
			.SetLayoutType(0)
			.SetLocationID(1),
			IW8::Lua::CRMContent()
			.SetTitle("04/03/25 Changelog")
			.SetContentShort("- failed to add an anti-anti-debug\\n"
				"- added a map validator (now doesn't let you launch on an invalid map, autodetected)")
			.SetLayoutType(0)
			.SetLocationID(1),
			IW8::Lua::CRMContent()
			.SetTitle("03/03/25 Changelog")
			.SetContentShort("- add splash screens to 1.38 and 1.44 (1.20 is literally mspaint lol)\\n")
			.SetLayoutType(0)
			.SetLocationID(1),
			IW8::Lua::CRMContent()
			.SetTitle("02/03/25 Changelog")
			.SetContentShort("- add github/gitlab crossover helper scripts\\n"
				"- fix compiler warnings\\n"
				"- remove logging on TLS function calling\\n"
				"- fix translation overrides\\n"
				"- github actions now automatically builds the client")
			.SetLayoutType(0)
			.SetLocationID(1),
			IW8::Lua::CRMContent()
			.SetTitle("01/03/25 Changelog")
			.SetContentShort("^1happy new years! (been a while...)\\n"
				"\\n"
				"- base support for 1.38 and 1.44 (they can now get into the main menu)\\n"
				"- remove Replay/Ship build configs, dll now has multiversion support\\n"
				"- pro tip: you can't patch Arxan by using a GTA 5 signature (idot)\\n"
				"- added mutex to log so it doesn't bug out anymore\\n"
				"- added input in the external console for cbuf (prefixed with /, like minecraft)")
			.SetLayoutType(0)
			.SetLocationID(1),
			IW8::Lua::CRMContent()
			.SetTitle("28/12/24 Changelog")
			.SetContentShort("- added template ws2_32.dll IAT hooks")
			.SetLayoutType(0)
			.SetLocationID(1),
			IW8::Lua::CRMContent()
			.SetTitle("27/12/24 Changelog")
			.SetContentShort("- add bytebuffer from S1")
			.SetLayoutType(0)
			.SetLocationID(1),
			IW8::Lua::CRMContent()
			.SetTitle("23/12/24 Changelog")
			.SetContentShort("- main menu status text fixed\\n"
				"- figured out that hooking DWServicesAccess::isReady gives the player an epileptic seizure")
			.SetLayoutType(0)
			.SetLocationID(1),
			IW8::Lua::CRMContent()
			.SetTitle("22/12/24 Changelog")
			.SetContentShort("- client now renders debug info\\n"
				"- fixed discord rpc player count")
			.SetLayoutType(0)
			.SetLocationID(1),
			IW8::Lua::CRMContent()
			.SetTitle("21/12/24 Changelog")
			.SetContentShort("- add lua hooks by string\\n"
				"- improved load times by hooking IsBattleNetAuthReady and IsPremiumPlayerReady\\n"
				"- now able to get to main menu of 1.20/ship\\n"
				"- ship doesn't crash when trying to go in syslink (it kicks you out)\\n"
				"- added log to ActivateInitialClient\\n"
				"- fix auth on replay\\n"
				"- removed \"[TAB] Go Online\" in main menu\\n"
				"- removed unnecessary hooks that broke 1.20/replay")
			.SetLayoutType(0)
			.SetLocationID(1),
			IW8::Lua::CRMContent()
			.SetTitle("15/12/24 Changelog")
			.SetContentShort("- rebased to new code format\\n"
				"- can now host local games on 1.20/replay\\n"
				"- now able to get to main menu of 1.20/ship\\n"
				"- added inventory loading\\n"
				"- added button to discord rpc\\n"
				"- unlocked ship main menu (can't access anything though)\\n"
				"- added iw8-mod watermark\\n"
				"- added lost connection fix")
			.SetLayoutType(0)
			.SetLocationID(1),
			IW8::Lua::CRMContent()
			.SetTitle("16/11/24 Changelog")
			.SetContentShort("- forked iw8-mod from t9-mod")
			.SetLayoutType(0)
			.SetLocationID(1)
		};

		std::size_t idx = 0;
		for (auto& [key, val] : g_PatchedCRM) {
			IW8::Lua::CRMContent::AutoFillIndexes(&val, idx);
			idx++;
		}

		IW8::dvar_t* crm_max_message_count = g_Pointers->m_Dvar_FindVarByName("NSKOMKOSLO");
		if (crm_max_message_count != nullptr) {
			crm_max_message_count->m_Domain.m_Integer.m_Max = 0x7FFFFFFF;
			crm_max_message_count->m_Current.m_Integer = 0x7FFFFFFF;
		}

		IW8::dvar_t* crm_max_patch_note_count = g_Pointers->m_Dvar_FindVarByName("NNNSQLQO");
		if (crm_max_patch_note_count != nullptr) {
			crm_max_patch_note_count->m_Domain.m_Integer.m_Max = 0x7FFFFFFF;
			crm_max_patch_note_count->m_Current.m_Integer = 0x7FFFFFFF;
		}
	}
}
