#pragma once
#include "common_core.hpp"
#include "logger/console.hpp"
#include "logger/log_levels.hpp"

#include <format>

namespace Common {
	class Logger {
	public:
		Logger(std::string logName) {
			this->m_LogName = logName;
		}

		template <typename... Args>
		void Print(LogLevel level, const std::string_view& format, Args const &...args) {
			std::string caller = "<unknown module>";

			auto message = std::vformat(format, std::make_format_args(args...));

			struct tm localTime;
			const time_t timeSinceEpoch = std::time(nullptr);
			localtime_s(&localTime, &timeSinceEpoch);

			auto consoleTimestamp = std::format("[{:0>2}:{:0>2}:{:0>2}]", localTime.tm_hour, localTime.tm_min, localTime.tm_sec);
			auto fileTimestamp = std::format("[{}-{}-{} {:0>2}:{:0>2}:{:0>2}]", localTime.tm_year + 1900, localTime.tm_mon + 1, localTime.tm_mday,
				localTime.tm_hour, localTime.tm_min, localTime.tm_sec);

			g_Console.Write(std::format(ANSI_FG_CYAN "{} " ANSI_RESET "{}[{}{}] " ANSI_RESET ANSI_FG_RGB(0, 163, 163) "({}) " ANSI_RESET "{}" ANSI_RESET,
				consoleTimestamp, level.GetAnsiColor(), caller.compare("<unknown module>") ? (caller + "/").c_str() : "", level.GetLabel(), this->m_LogName, message));
		}
	private:
		std::string m_LogName{};
	};
}
