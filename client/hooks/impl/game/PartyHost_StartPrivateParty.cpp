#include "common.hpp"
#include "game/functions.hpp"
#include "game/game.hpp"
#include "game/inventory.hpp"
#include "hooks/hook.hpp"

template <>
void Client::Hook::Hooks::HK_PartyHost_StartPrivateParty::hkCallback(int localClientNum, int localControllerIndex, bool currentlyActive, int hostType) {
	LOG("Game/PartyHost_StartPrivateParty", DEBUG, "hello");
	m_Original(localClientNum, localControllerIndex, currentlyActive, hostType);

	static bool autoExec = false;
	if (!autoExec) {
		Game::Cbuf_AddText("exec autoexec.cfg");
		g_Pointers->m_LUI_OpenMenu(IW8::LocalClientNum_t::LOCAL_CLIENT_0, "CRMContainer", FALSE, FALSE, FALSE);
		Inventory::Load();

		// skips Tutorial stuff
		Game::Cbuf_AddText("setRankedPlayerData mpTutorialLevel -2147463171");
		Game::Cbuf_AddText("setRankedPlayerData mpTutorialLevel2 28671");
		Game::Cbuf_AddText("setRankedPlayerData mpTutorialLevel - 2147454979");
		Game::Cbuf_AddText("setRankedPlayerData mpTutorialLevel2 61439");

		autoExec = true;
	}
}
