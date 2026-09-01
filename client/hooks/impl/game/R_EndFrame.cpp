#include "common.hpp"
#include "engine/iw8/XUID.hpp"
#include "game/game.hpp"
#include "game/map_validator.hpp"
#include "game/settings.hpp"
#include "hooks/hook.hpp"
#include "hooks/util/hook_util.hpp"
#include "memory/memory.hpp"

bool s_PatchedAuth = false;
int s_FramesPassed = 0;

template <>
void Client::Hook::Hooks::HK_R_EndFrame::hkCallback() {
	static std::string watermarkText = std::format("iw8-mod: " GIT_DESCRIBE " [v{}]", g_GameIdentifier.m_Version);

	if (g_Pointers->m_sharedUiInfo_assets) {
		IW8::GfxFont* font = g_Pointers->m_sharedUiInfo_assets->m_SubtitleFont;
		if (font) {
			IW8::vec4_t color = { .666f, .666f, .666f, .666f };
			g_Pointers->R_AddCmdDrawText(watermarkText.c_str(),
				0x7FFFFFFF, font, font->m_PixelHeight, 4.f, 4.f + static_cast<float>(font->m_PixelHeight), 1.f, 1.f, 0.f, &color);
		}
	}

	if (!s_PatchedAuth) {
		if (s_FramesPassed > 500) {
			IW8::XUID xuid;
			xuid.RandomXUID();
			std::uint64_t xuidMagic = 0x11CB1243B8D7C31E;
			std::uint64_t xuidId = xuid.m_ID * xuid.m_ID;

			LOG("Game/R_EndFrame", INFO, "Patching auth...");
			g_Pointers->m_Unk_BNetClass->m_FinishedAuth = true;

			if (g_Pointers->m_Unk_XUIDCheck1 != nullptr) {
				*g_Pointers->m_Unk_XUIDCheck1 = xuidMagic | xuidId;
			}

			// the below patch (0x14F05ACE8 @ 1.20.4.7623265-replay) seems to be replay only, with this being xref 5 of 6, however only 5 exist
			// in ship, and this is the one excluded.
			if (g_Pointers->m_Unk_XUIDCheck2 != nullptr) {
				*g_Pointers->m_Unk_XUIDCheck2 = xuidMagic | xuidId;
			}

			if (g_Pointers->m_s_presenceData != nullptr) {
				(*g_Pointers->m_s_presenceData)[0].m_Current.m_CrossTitlePresenceData.m_PlatformID = xuidMagic | xuidId / 6;
			}

			*g_Pointers->m_s_isContentEnumerationFinished = true;
			g_Pointers->m_Unk_BNetClass->m_State = 2;
			g_Pointers->m_Unk_BNetClass->m_FinishedAuth = true;

			IW8::dvar_t* xp_dec_dc = g_Pointers->m_Dvar_FindVarByName("NTTRLOPQKS");
			if (xp_dec_dc != nullptr) {
				xp_dec_dc->m_Current.m_Enabled = false;
			}

			IW8::dvar_t* r_hudOutlineVRScopeThermalDarkColorEnemy = g_Pointers->m_Dvar_FindVarByName("MRMMQQKQKN");
			if (r_hudOutlineVRScopeThermalDarkColorEnemy != nullptr) {
				/*
					ok so this is imported from... somewhere- but this isn't being left in.
					as you can clearly see:
						v156 = Dvar_RegisterVec4("MRMMQQKQKN", *(float *)&_XMM1, *(float *)&_XMM2, *(float *)&_XMM3,
							v1381, descriptiono, maxo, 0x44u, "The dark color for enemies.");
					this dvar is a vec4, not a bool, and why would we even need to change this? no idea.
				*/
				//r_hudOutlineVRScopeThermalDarkColorEnemy->m_Current.m_Enabled = false;

				/*
					update: i was wrong. we weren't changing the value... but the name to "\x00"?
						that makes even less sense i'm sorry
				*/
			}

			//Hook::Util::g_ForceSignInState = true;
			g_Pointers->m_Unk_BNetClass->m_Var3 = 0x795230F0;
			g_Pointers->m_Unk_BNetClass->m_Var4 = 0x1F;
			g_Pointers->m_Unk_BNetClass->m_Var5 = 0x00000000;

			LOG("Game/R_EndFrame", INFO, "Patched auth.");
			Settings::Load();
			Hook::Util::InitialiseCRM();
			s_PatchedAuth = true;
		}
		else {
			s_FramesPassed++;
		}
	}

	if (Hook::Util::g_GameThreadQueue.size() > 0) {
		for (auto func : Hook::Util::g_GameThreadQueue) {
			func();
		}
		Hook::Util::g_GameThreadQueue.clear();
	}

	if (g_GameIdentifier.m_Ship && g_Pointers->m_Live_IsInSystemlinkLobby) {
		IW8::dvar_t* systemlink = g_Pointers->m_Dvar_FindVarByName("LPSPMQSNPQ");
		std::uintptr_t funcAddr = reinterpret_cast<std::uintptr_t>(g_Pointers->m_Live_IsInSystemlinkLobby);

		static bool wasSystemlink = false;
		bool isSystemlink = systemlink && systemlink->m_Current.m_Enabled;

		if (isSystemlink) {
			*reinterpret_cast<char*>(funcAddr + 0) = '\xB0';
			*reinterpret_cast<char*>(funcAddr + 1) = '\x01';
		}
		else {
			*reinterpret_cast<char*>(funcAddr + 0) = '\x32';
			*reinterpret_cast<char*>(funcAddr + 1) = '\xC0';
		}

		if (wasSystemlink != isSystemlink) {
			LOG("Game/R_EndFrame", DEBUG, "systemlink = {}", isSystemlink ? "yes" : "no");
			wasSystemlink = isSystemlink;
		}
	}

	return m_Original();
}
