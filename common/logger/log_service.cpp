#include "common_core.hpp"
#include "logger/log_service.hpp"
#include "utility/utility.hpp"

namespace Common {
	LogService::LogService() {
		AllocConsole();
		freopen_s((FILE**)(stdout), "CONOUT$", "w", stdout);
		freopen_s((FILE**)(stdin), "CONIN$", "r", stdin);
		SetConsoleTitleA("iw8-mod");
		SetConsoleOutputCP(CP_UTF8);

		Utility::EnsureVTL();
	}

	LogService::~LogService() {
		fclose(stdin);
		fclose(stdout);
		FreeConsole();
	}

	Logger LogService::GetLogger(std::string name) {
		return Logger(name);
	}
}
