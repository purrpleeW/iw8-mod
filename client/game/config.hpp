#pragma once
#include "common.hpp"

#include <nlohmann/json.hpp>
#include <lmcons.h>

#define JSON_FILE_DEF_PROPERTY(type, name) \
	type Get##name() const { \
		return this->m_JsonData[#name].get<type>(); \
	} \
	\
	void Set##name(const type& v) { \
		this->m_JsonData[#name] = v; \
		this->Save(); \
	}

namespace Client {
	namespace Game {
		inline std::filesystem::path GetJsonFilePath(const std::string& name) {
			std::filesystem::path cd = "."s;
			return cd / "iw8-mod" / name;
		}

		class JsonFile {
		public:
			JsonFile(const std::string& path)
				: m_Path(GetJsonFilePath(path))
			{}

			virtual nlohmann::json GetDefault() {
				return {};
			}

			void Load() {
				std::filesystem::path dir = this->m_Path.parent_path();
				if (!std::filesystem::exists(dir)) {
					std::filesystem::create_directories(dir);
				}

				if (!std::filesystem::exists(this->m_Path)) {
					this->m_JsonData = this->GetDefault();
					this->Save();
					return;
				}

				std::ifstream file(this->m_Path);

				if (file.is_open()) {
					try {
						file >> this->m_JsonData;
					}
					catch (const nlohmann::json::parse_error& e) {
						LOG("JsonFile", ERROR, "Parsing error: {}", e.what());
					}
					file.close();
				}

				nlohmann::json def = this->GetDefault();
				for (const auto& [key, val] : def.items()) {
					if (!this->m_JsonData.contains(key)) {
						this->m_JsonData[key] = val;
					}
				}
				this->Save();
			}

			void Save() const {
				std::ofstream file(this->m_Path);
				if (file.is_open()) {
					file << this->m_JsonData.dump(4);
					file.close();
				}
			}

		protected:
			std::filesystem::path m_Path;
			nlohmann::json m_JsonData;
		};

		class Config : public JsonFile {
		public:
			Config(const std::string& path)
				: JsonFile(path)
			{}

			nlohmann::json GetDefault() override {
				return {
					{ "PlayerName", GetDefaultPlayerName() }
				};
			}

			JSON_FILE_DEF_PROPERTY(std::string, PlayerName)
		private:
			std::string GetDefaultPlayerName() {
				char username[UNLEN + 1];
				DWORD usernameLen = UNLEN + 1;

				if (GetUserNameA(username, &usernameLen)) {
					return std::string(username);
				}

				return "UnnamedPlayer";
			}
		};

		class GenericJsonFile : public JsonFile {
		public:
			GenericJsonFile(const std::string& path)
				: JsonFile(path)
			{}

			nlohmann::json GetDefault() override {
				return {};
			}

			void AccessJsonData(std::function<void(nlohmann::json&)> callback) {
				callback(this->m_JsonData);
				this->Save();
			}
		};
	}

	inline Game::Config g_Config{ "config.json" };
	inline Game::GenericJsonFile g_Inventory{ "inventory.json" };
	inline Game::GenericJsonFile g_Settings{ "settings.json" };
}
