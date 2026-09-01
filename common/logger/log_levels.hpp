#pragma once
#include "Logger/ANSI.hpp"

namespace Common {
	class LogLevel {
	public:
		LogLevel(const std::string& label, const std::string& ansiColor, int index) {
			this->m_Label = label;
			this->m_AnsiColor = ansiColor;
			this->m_Index = index;
		}

		std::string GetLabel() {
			return this->m_Label;
		}

		std::string GetAnsiColor() {
			return this->m_AnsiColor;
		}

		bool operator==(int index) {
			return this->m_Index == index;
		}
	private:
		std::string m_Label{};
		std::string m_AnsiColor{};
		int m_Index{};
	};

	namespace Levels {
		// Do not create log levels on the same line - it will not look good in code and will break my shortening
#		undef ERROR
#		define CREATE_LOG_LEVEL(name, color) \
		inline int name##_INDEX = __LINE__;\
		inline LogLevel name = LogLevel(ANSI_STRINGIFY(name), color, name##_INDEX);
		CREATE_LOG_LEVEL(INFO, ANSI_FG_RGB(92, 150, 44))
		CREATE_LOG_LEVEL(WARN, ANSI_FG_RGB(166, 138, 13))
		CREATE_LOG_LEVEL(ERROR, ANSI_FG_RGB(240, 82, 79))
		CREATE_LOG_LEVEL(FATAL, ANSI_FG_STYLE_256(124))
		CREATE_LOG_LEVEL(TRACE, ANSI_FG_STYLE_256(214))
		CREATE_LOG_LEVEL(DEBUG, ANSI_FG_STYLE_256(243))
#		undef CREATE_LOG_LEVEL
	}
}
