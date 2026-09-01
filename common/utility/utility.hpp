#pragma once
#include "common_core.hpp"
#include "logger/log_service.hpp"

namespace Common::Utility {
	void EnsureVTL();
	MODULEINFO GetModuleInfo(std::string peModule);
	std::vector<std::string> Split(const std::string& str, const std::string& delim);
	bool EndsWith(const std::string& str, const std::string& suffix);
	std::map<std::size_t, std::uint32_t> GetAnsiMap(const std::string& str);
}
