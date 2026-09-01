#include "common.hpp"
#include "hooks/hook.hpp"
#include "hooks/util/hook_util.hpp"

#include "resource.h"

template <>
HANDLE WINAPI Client::Hook::Hooks::HK_LoadImageA::hkCallback(HINSTANCE hInst, LPCSTR name, UINT type, int cx, int cy, UINT fuLoad) {
	if (reinterpret_cast<std::uintptr_t>(name) == 0x64) {
		while (!Hook::Util::g_HooksInitialised) {
			Sleep(100);
		}

		int id;
		switch (g_GameIdentifier.m_Checksum) {
		case GameVersion::v1_20_4_7623265_REPLAY:
		case GameVersion::v1_20_4_7623265_SHIP:
			id = IMAGE_SPLASH_v1_20;
			break;
		case GameVersion::v1_44_0_10435696:
			id = IMAGE_SPLASH_v1_44;
			break;
		case GameVersion::v1_38_3_9489393:
			id = IMAGE_SPLASH_v1_38;
			break;
		default:
			id = IMAGE_SPLASH;
			break;
		}
		return LoadImageA(g_Module, MAKEINTRESOURCEA(id), 0, 0, 0, LR_COPYFROMRESOURCE);
	}

	return LoadImageA(hInst, name, type, cx, cy, fuLoad);
}
