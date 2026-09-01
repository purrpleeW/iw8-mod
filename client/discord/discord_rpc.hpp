#pragma once
#include "common.hpp"
#include <discord_rpc.h>

namespace Client::Discord {
	void Initialize();
	void Destroy();
	void UpdateStatus(std::function<void(DiscordRichPresence*)> func);
	void Update();
	DWORD Thread(PVOID);
}
