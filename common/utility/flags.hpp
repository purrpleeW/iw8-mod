#pragma once
#include "common_core.hpp"

namespace Common::Utility::Flags {
	void ParseFlags(std::vector<std::string>& flags);
	bool HasFlag(const std::string& flag);
}
