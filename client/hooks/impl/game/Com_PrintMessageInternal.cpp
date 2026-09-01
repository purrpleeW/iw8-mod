#include "common.hpp"
#include "hooks/hook.hpp"

#include <utility/flags.hpp>
#include <utility/strings.hpp>

template <>
void Client::Hook::Hooks::HK_Com_PrintMessageInternal::hkCallback(const int channelAndFlags, const char* msg, char error) {
	static bool dbg = Common::Utility::Flags::HasFlag("dbg");
	static bool dbgIngame = Common::Utility::Flags::HasFlag("dbgingame");

	if (dbg) {
		std::string msgStr = msg;
		if (Common::Utility::Strings::EndsWith(msgStr, "\n")) {
			msgStr = msgStr.substr(0, msgStr.size() - 1);
		}

		msgStr = Common::Utility::Strings::Replace(msgStr, "^1", ANSI_FG_RGB(255, 92, 92));
		msgStr = Common::Utility::Strings::Replace(msgStr, "^2", ANSI_FG_RGB(0, 255, 0));
		msgStr = Common::Utility::Strings::Replace(msgStr, "^3", ANSI_FG_RGB(230, 200, 0));
		msgStr = Common::Utility::Strings::Replace(msgStr, "^4", ANSI_FG_RGB(0, 0, 255));
		msgStr = Common::Utility::Strings::Replace(msgStr, "^5", ANSI_FG_RGB(59, 188, 243));
		msgStr = Common::Utility::Strings::Replace(msgStr, "^6", ANSI_FG_RGB(255, 92, 255));
		msgStr = Common::Utility::Strings::Replace(msgStr, "^7", ANSI_FG_RGB(255, 255, 255));

		if (error == '\x00') {
			LOG("Game/PrintStream", INFO, msgStr);
		}
		else {
			LOG("Game/PrintStream", ERROR, msgStr);
		}
	}
	
	if (dbgIngame) {
		m_Original(channelAndFlags, msg, error);
	}
}
