#pragma once
#include "common.hpp"

namespace Client::Game::MapValidator {
	inline std::map<std::string, bool> g_LoadedFastfiles{};
	DWORD Thread(PVOID);
}
