#include "common.hpp"
#include "game/game.hpp"
#include "hooks/hook.hpp"
#include "hooks/util/hook_util.hpp"

template <>
int Client::Hook::Hooks::HK_CL_GetLocalClientSignInState::hkCallback(int controllerIndex) {
	static bool signedIn = false;
	if (Hook::Util::g_ForceSignInState && !signedIn) {
		int arrElemSize = 0;

		if (GameVersionIsAny(GameVersion::v1_20_4_7623265_REPLAY, GameVersion::v1_20_4_7623265_SHIP)) {
			arrElemSize = 0x3A;
		}
		else if (GameVersionIsAny(GameVersion::v1_30_0_8403677, GameVersion::v1_38_3_9489393, GameVersion::v1_42_1_10125479, GameVersion::v1_44_0_10435696)) {
			arrElemSize = 0x46;
		}
		else if (GameVersionIsAny(GameVersion::v1_46_0_10750827)) {
			arrElemSize = 0x100;
		}

		if (arrElemSize != 0) {
			g_Pointers->m_Unk_SignInState[arrElemSize * controllerIndex] = 2;
			signedIn = true;
		}
	}

	return m_Original(controllerIndex);
}
