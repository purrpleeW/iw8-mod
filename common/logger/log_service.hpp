#pragma once
#include "common_core.hpp"
#include "logger/logger.hpp"

namespace Common {
	class LogService {
	public:
		LogService();
		~LogService();

		static Logger GetLogger(std::string name);
	};

	inline std::unique_ptr<LogService> g_LogService{};
}

#define LOG_GET(name) Common::g_LogService->GetLogger(name)
#define LOG(name, type, ...) LOG_GET(name).Print(Common::Levels::type, __VA_ARGS__)
