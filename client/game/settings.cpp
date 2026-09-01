#include "common.hpp"
#include "game/config.hpp"
#include "game/game.hpp"
#include "game/settings.hpp"

namespace Client {
	int Settings::GetSettingsCount() {
		switch (g_GameIdentifier.m_Checksum) {
		case GameVersion::v0_01_2_7089334:
		case GameVersion::v1_03_0_7209368:
			// idk yet
			return 1;
		case GameVersion::v1_20_4_7623265_REPLAY:
		case GameVersion::v1_20_4_7623265_SHIP:
			return 135;
		case GameVersion::v1_38_3_9489393:
			return 135;	// temp
		case GameVersion::v1_44_0_10435696:
			return 135;	// temp
		}

		return 1;
	}

	void Settings::Save() {
		g_Settings.AccessJsonData([](nlohmann::json& json) {
			json.clear();

			IW8::GamerProfileData profile;
			for (int i = 0; i < GetSettingsCount(); ++i) {
				std::string settingName = g_Pointers->m_PROFILE_DATA_FIELDS[i].m_SettingName;
				if (g_Pointers->m_GamerProfile_GetDataByName(&profile, 0, settingName.c_str())) {
					switch (profile.m_Type) {
					case IW8::GamerProfileDataType::TYPE_INVALID:
						LOG("GameSettings", DEBUG, "Can't save setting \"{}\" as it has an invalid type.", settingName);
						break;
					case IW8::GamerProfileDataType::TYPE_BYTE:
						json[settingName] = profile.m_Union.m_ByteVal;
						break;
					case IW8::GamerProfileDataType::TYPE_BOOL:
						json[settingName] = profile.m_Union.m_BoolVal;
						break;
					case IW8::GamerProfileDataType::TYPE_SHORT:
						json[settingName] = profile.m_Union.m_ShortVal;
						break;
					case IW8::GamerProfileDataType::TYPE_INT:
						json[settingName] = profile.m_Union.m_IntVal;
						break;
					case IW8::GamerProfileDataType::TYPE_FLOAT:
						json[settingName] = profile.m_Union.m_FloatVal;
						break;
					case IW8::GamerProfileDataType::TYPE_STRING:
						json[settingName] = profile.m_Union.m_StringVal;
						break;
					case IW8::GamerProfileDataType::TYPE_BUFFER:
						LOG("GameSettings", DEBUG, "Can't save setting \"{}\" as the 'BUFFER' type is not implemented.", settingName);
						break;
					case IW8::GamerProfileDataType::TYPE_FLAG:
						json[settingName] = profile.m_Union.m_BoolVal;
						break;
					default:
						LOG("GameSettings", DEBUG, "Can't find setting \"{}\".", settingName);
					}
				}
			}
		});
	}

	void Settings::Load() {
		g_Settings.Load();
		g_Settings.AccessJsonData([](nlohmann::json& json) {
			if (json.size() == 0) {
				return;
			}

			IW8::GamerProfileData profile;
			for (int i = 0; i < GetSettingsCount(); ++i) {
				std::string settingName = g_Pointers->m_PROFILE_DATA_FIELDS[i].m_SettingName;
				if (g_Pointers->m_GamerProfile_GetDataByName(&profile, 0, settingName.c_str())) {
					switch (profile.m_Type) {
					case IW8::GamerProfileDataType::TYPE_INVALID:
						LOG("GameSettings", DEBUG, "Can't load setting \"{}\" as it has an invalid type.", settingName);
						break;
					case IW8::GamerProfileDataType::TYPE_BYTE:
						LOG("GameSettings", DEBUG, "Can't load setting \"{}\" as the 'BYTE' type is not implemented.", settingName);
						break;
					case IW8::GamerProfileDataType::TYPE_BOOL:
						g_Pointers->m_GamerProfile_SetDataByName(0, settingName.c_str(), json[settingName]);
						break;
					case IW8::GamerProfileDataType::TYPE_SHORT:
						LOG("GameSettings", DEBUG, "Can't load setting \"{}\" as the 'SHORT' type is not implemented.", settingName);
						break;
					case IW8::GamerProfileDataType::TYPE_INT:
						g_Pointers->m_GamerProfile_SetDataByName(0, settingName.c_str(), json[settingName]);
						break;
					case IW8::GamerProfileDataType::TYPE_FLOAT:
						g_Pointers->m_GamerProfile_SetDataByName(0, settingName.c_str(), json[settingName].get<float>());
						break;
					case IW8::GamerProfileDataType::TYPE_STRING:
						LOG("GameSettings", DEBUG, "Can't load setting \"{}\" as the 'STRING' type is not implemented.", settingName);
						break;
					case IW8::GamerProfileDataType::TYPE_BUFFER:
						LOG("GameSettings", DEBUG, "Can't load setting \"{}\" as the 'BUFFER' type is not implemented.", settingName);
						break;
					case IW8::GamerProfileDataType::TYPE_FLAG:
						g_Pointers->m_GamerProfile_SetDataByName(0, settingName.c_str(), json[settingName]);
						break;
					default:
						LOG("GameSettings", DEBUG, "Can't find setting \"{}\".", settingName);
					}
				}
			}
		});
	}
}
