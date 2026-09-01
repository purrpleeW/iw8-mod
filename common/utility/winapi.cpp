#include "common_core.hpp"
#include "utility/winapi.hpp"

namespace Common::WinAPI {
	BOOL _SetConsoleTitle(std::string title) {
		return SetConsoleTitleA(title.c_str());
	}
}
