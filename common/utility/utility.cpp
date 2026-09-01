#include "common_core.hpp"
#include "utility/registry.hpp"
#include "utility/utility.hpp"
#include <imgui.h>

namespace Common::Utility {
	void EnsureVTL() {
		DWORD virtualTerminalLevelCur = 0;
		Registry::GetRegValue(HKEY_CURRENT_USER, "Console", "VirtualTerminalLevel", virtualTerminalLevelCur, 0);
		if (virtualTerminalLevelCur == 0) {
			Registry::SetRegValue(HKEY_CURRENT_USER, "Console", "VirtualTerminalLevel", 0x0001);
			int mb = MessageBoxA(nullptr, "VirtualTerminalLevel, enabling colours in terminal, was not enabled."
				" The terminal will work when the console is reloaded - would you like to reload the console now?", "game", MB_YESNO);
			if (mb == IDYES) {
				if (g_LogService != nullptr) {
					g_LogService.reset();
				}
				g_LogService = std::make_unique<LogService>();
			}
		}
	}

	MODULEINFO GetModuleInfo(std::string peModule) {
		MODULEINFO mInfo{ 0 };
		HINSTANCE hInst = GetModuleHandleA(peModule.c_str());

		if (!hInst)
			return mInfo;

		GetModuleInformation(GetCurrentProcess(), hInst, &mInfo, sizeof MODULEINFO);
		return mInfo;
	}

	std::vector<std::string> Split(const std::string& str, const std::string& delim) {
		std::vector<std::string> tokens;
		std::string::size_type start = 0;
		std::string::size_type end;

		while ((end = str.find(delim, start)) != std::string::npos) {
			tokens.push_back(str.substr(start, end - start));
			start = end + delim.length();
		}
		tokens.push_back(str.substr(start));

		return tokens;
	}

	bool EndsWith(const std::string& str, const std::string& suffix) {
		if (str.length() >= suffix.length()) {
			return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
		}
		return false;
	}

	std::map<std::size_t, std::uint32_t> GetAnsiMap(const std::string& str) {
		std::regex ansiEscape(R"(\x1B\[[0-?9;]*[m])");
		std::string::const_iterator searchStart(str.cbegin());
		std::smatch match;

		ImU32 defaultColor = ImGui::GetColorU32(ImGui::GetStyle().Colors[ImGuiCol_Text]);
		ImU32 currentColor = defaultColor;
		std::size_t discardedSize = 0;

		std::map<std::size_t, std::uint32_t> map{
			{0, defaultColor}
		};

		while (std::regex_search(searchStart, str.cend(), match, ansiEscape)) {
			std::size_t pos = match.position() - discardedSize;
			discardedSize += match.str().size();

			std::string ansiCode = match.str();
			if (ansiCode == ANSI_RESET || ansiCode == ANSI_FG_DEFAULT) {
				currentColor = defaultColor;
			}
			else if (ansiCode == ANSI_FG_BLACK) {
				currentColor = IM_COL32(12, 12, 12, 255);
			}
			else if (ansiCode == ANSI_FG_RED) {
				currentColor = IM_COL32(197, 15, 31, 255);
			}
			else if (ansiCode == ANSI_FG_GREEN) {
				currentColor = IM_COL32(19, 161, 14, 255);
			}
			else if (ansiCode == ANSI_FG_YELLOW) {
				currentColor = IM_COL32(193, 156, 0, 255);
			}
			else if (ansiCode == ANSI_FG_BLUE) {
				currentColor = IM_COL32(0, 55, 218, 255);
			}
			else if (ansiCode == ANSI_FG_MAGENTA) {
				currentColor = IM_COL32(136, 23, 152, 255);
			}
			else if (ansiCode == ANSI_FG_CYAN) {
				currentColor = IM_COL32(58, 150, 221, 255);
			}
			else if (ansiCode == ANSI_FG_WHITE) {
				currentColor = IM_COL32(204, 204, 204, 204);
			}
			map[pos] = currentColor;

			/*
#define ANSI_FG_RGB(r, g, b)	ANSI_ESCAPE "[38;2;" ANSI_STRINGIFY(r) ";" ANSI_STRINGIFY(g) ";" ANSI_STRINGIFY(b) "m"
			*/

			searchStart += match.length();
		}

		return map;
	}
}
