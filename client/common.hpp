#pragma once
#include <common_core.hpp>
#include <logger/log_service.hpp>
#include <utility/nt.hpp>
#include <utility/winapi.hpp>

#define VT_GET(ptr, idx) (*(void***)(ptr))[idx]
#define HIGH_ORDER_LOG_HOOK 0

#define ENUM_UNDER(val) static_cast<std::underlying_type<decltype(val)>::type>(val)

namespace Client {
	using namespace std::chrono_literals;
	using namespace std::string_literals;

	enum class GameVersion {
		NONE,
		v0_01_2_7089334,
		v1_03_0_7209368,
		v1_20_4_7623265_REPLAY,
		v1_20_4_7623265_SHIP,
		v1_30_0_8403677,
		v1_38_3_9489393,
		v1_42_1_10125479,
		v1_44_0_10435696,
		v1_46_0_10750827
	};

	struct GameIdentifier {
		std::string m_Version = "Unknown version";
		GameVersion m_Checksum = GameVersion::NONE;
		bool m_Ship = true;
	};

	inline std::unordered_map<std::uint32_t, GameIdentifier> g_GameVersions{
		{ 0x37CCF2A5, { "0.01.2.7089334", GameVersion::v0_01_2_7089334, true } },
		{ 0x641EA3EB, { "1.03.0.7209368", GameVersion::v1_03_0_7209368, true } },
		{ 0xFF380251, { "1.20.4.7623265-replay", GameVersion::v1_20_4_7623265_REPLAY, false } },
		{ 0x6E9C74C8, { "1.20.4.7623265-ship", GameVersion::v1_20_4_7623265_SHIP, true } },
		{ 0x7E37B0B9, { "1.30.0.8403677", GameVersion::v1_30_0_8403677, true } },
		{ 0xCE698CA4, { "1.38.3.9489393", GameVersion::v1_38_3_9489393, true } },
		{ 0x54FBBFD1, { "1.42.1.10125479", GameVersion::v1_42_1_10125479, true } },
		{ 0xD6C4C029, { "1.44.0.10435696", GameVersion::v1_44_0_10435696, true } },
		{ 0x5910A608, { "1.46.0.10750827", GameVersion::v1_46_0_10750827, true } }
	};

	inline HMODULE g_Module{};
	inline HANDLE g_MainThread{};
	inline DWORD g_MainThreadId{};
	inline std::atomic_bool g_Running{ true };
	inline std::string g_GameModuleName = "?";

	inline GameIdentifier g_GameIdentifier{};

	template<typename... Args>
	bool GameVersionIsAny(Args... args) {
		return ((args == g_GameIdentifier.m_Checksum) || ...);
	}

	inline bool GameVersionIsNewerThan(GameVersion version) {
		return static_cast<int>(version) < static_cast<int>(g_GameIdentifier.m_Checksum);
	}

	inline bool GameVersionIsOlderThan(GameVersion version) {
		return static_cast<int>(version) > static_cast<int>(g_GameIdentifier.m_Checksum);
	}

	// runtime settings
	inline bool g_ShowDebugInfo = false;

	// other things
	std::string AndRel(std::uintptr_t address);

	template <typename T>
	inline T GetProxyExport(const std::string& libName, const std::string& exportName) {
		char dir[MAX_PATH]{ 0 };
		GetSystemDirectoryA(dir, sizeof(dir));

		const auto lib = Common::Utility::NT::Library::Load(dir + "/"s + libName);
		return lib.GetProc<T>(exportName.c_str());
	}
}
