#include "common.hpp"
#include "game/asset_replacer.hpp"
#include "game/game.hpp"
#include "hooks/hook.hpp"

template <>
IW8::XAssetHeader Client::Hook::Hooks::HK_DB_FindXAssetHeader::hkCallback(IW8::XAssetType type, const char* givenName, int allowCreateDefault) {
	IW8::XAssetHeader og = m_Original(type, givenName, allowCreateDefault);
	
	if (type == IW8::XAssetType::ASSET_TYPE_TTF) {
		std::filesystem::path thisFontPath = AssetReplacer::GetAssetReplaceDir("ttf") / givenName;

		if (std::filesystem::exists(thisFontPath)) {
			std::lock_guard _(AssetReplacer::g_Mutex);
			std::ifstream font;

			font.open(thisFontPath, std::ios::binary | std::ios::ate);
			if (font.fail()) {
				LOG("Game/CustomFonts", WARN, "Couldn't open local font file: {}", givenName);
				return og;
			}

			int size = static_cast<int>(font.tellg());
			font.seekg(0, std::ios::beg);

			char* allocated = static_cast<char*>(malloc(size));

			font.read(allocated, size);
			font.close();

			og.m_TTFDef->m_FileLen = size;
			og.m_TTFDef->m_File = allocated;
		}
		else {
			std::filesystem::path dir = std::filesystem::path(thisFontPath).parent_path();

			if (!std::filesystem::exists(dir)) {
				std::filesystem::create_directories(dir);
			}

			std::ofstream file(thisFontPath, std::ios::binary);
			if (!file) {
				LOG("Game/CustomFonts", WARN, "Couldn't open local font file for writing: {}", givenName);
				return og;
			}

			file.write(og.m_TTFDef->m_File, og.m_TTFDef->m_FileLen);
			if (!file) {
				LOG("Game/CustomFonts", WARN, "Couldn't write to local font file: {}", givenName);
			}

			file.close();
		}
	}
	
	return og;
}
