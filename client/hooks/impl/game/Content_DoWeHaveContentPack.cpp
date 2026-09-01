#include "common.hpp"
#include "hooks/hook.hpp"

template <>
bool Client::Hook::Hooks::HK_Content_DoWeHaveContentPack::hkCallback(int contentPack) {
	static std::vector<int> scannedAlready{};
	bool og = m_Original(contentPack);

	if (std::find(scannedAlready.begin(), scannedAlready.end(), contentPack) == scannedAlready.end()) {
		LOG("Game/Content_DoWeHaveContentPack", DEBUG, "{} -> {}", contentPack, og ? "Yes!" : "No.");
		scannedAlready.push_back(contentPack);
	}

	return og;
}
