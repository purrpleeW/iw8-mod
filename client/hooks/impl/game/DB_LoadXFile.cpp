#include "common.hpp"
#include "game/map_validator.hpp"
#include "hooks/hook.hpp"

template <>
int Client::Hook::Hooks::HK_DB_LoadXFile::hkCallback(const char* zoneName, uintptr_t zoneMem, uintptr_t assetList, int zoneFlags, bool wasPaused, int failureMode, uintptr_t outSignature) {
	LOG("Game/DB_LoadXFile", INFO, "Loading fastfile {}...", zoneName == nullptr ? "<null>" : zoneName);
	int res = m_Original(zoneName, zoneMem, assetList, zoneFlags, wasPaused, failureMode, outSignature);
	if (res != 0) {
		LOG("Game/DB_LoadXFile", WARN, "Loaded fastfile {} with non-zero code {}.", zoneName == nullptr ? "<null>" : zoneName, res);
	}
	Game::MapValidator::g_LoadedFastfiles.emplace(zoneName, res == 0);
	return res;
}
