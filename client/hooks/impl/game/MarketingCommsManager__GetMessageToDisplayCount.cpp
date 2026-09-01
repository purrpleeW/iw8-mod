#include "common.hpp"
#include "game/game.hpp"
#include "hooks/hook.hpp"
#include "hooks/util/hook_util.hpp"

template <>
std::uint64_t Client::Hook::Hooks::HK_MarketingCommsManager__GetMessageToDisplayCount::hkCallback(void* _this, int messageType) {
	std::uint64_t og = m_Original(_this, messageType);

	auto it = Hook::Util::g_PatchedCRM.find(static_cast<IW8::Lua::CRMLocation::Enum>(messageType));
	if (it != Hook::Util::g_PatchedCRM.end()) {
		return it->second.size() - 1;
	}

	return og;
}
