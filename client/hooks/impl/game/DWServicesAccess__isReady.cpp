#include "common.hpp"
#include "hooks/hook.hpp"

bool flipSwitch = false;
std::int64_t startTime = 0;

template <>
bool Client::Hook::Hooks::HK_DWServicesAccess__isReady::hkCallback(void* _this, const int controllerIndex) {
	/*
		SEIZURE WARNING: DO NOT JUST RETURN TRUE
	*/
	return m_Original(_this, controllerIndex);

	/*if (startTime == 0) {
		startTime = std::time(nullptr);
	}

	if (!flipSwitch && std::time(nullptr) > startTime + 20) {
		flipSwitch = true;
		LOG("Game/DWServicesAccess::isReady", INFO, "Now returning true. WCGW?");
	}

	return flipSwitch;*/
}
