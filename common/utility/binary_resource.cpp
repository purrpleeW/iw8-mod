#include "common_core.hpp"
#include "utility/binary_resource.hpp"
#include "utility/io.hpp"
#include "utility/nt.hpp"

namespace Common::Utility {
	namespace {
		std::string GetTempFolder() {
			char path[MAX_PATH] = { 0 };
			if (!GetTempPathA(sizeof(path), path)) {
				throw std::runtime_error("Unable to get temp path");
			}

			return path;
		}

		std::string WriteExistingTempFile(const std::string& file, const std::string& data, const bool fatalIfOverwriteFails) {
			const auto temp = GetTempFolder();
			auto filePath = temp + file;

			std::string currentData;
			if (!IO::ReadFile(filePath, &currentData)) {
				if (!IO::WriteFile(filePath, data)) {
					throw std::runtime_error("Failed to write file: " + filePath);
				}

				return filePath;
			}

			if (currentData == data || IO::WriteFile(filePath, data) || !fatalIfOverwriteFails) {
				return filePath;
			}

			throw std::runtime_error(
				"Temporary file was already written, but differs. It can't be overwritten as it's still in use: " +
				filePath);
		}
	}

	BinaryResource::BinaryResource(const int id, std::string file)
		: m_FileName(std::move(file))
	{
		this->m_Resource = NT::LoadResource(id);

		if (this->m_Resource.empty()) {
			throw std::runtime_error("Unable to load resource: " + std::to_string(id));
		}
	}

	std::string BinaryResource::GetExtractedFile(const bool fatalIfOverwriteFails) {
		if (this->m_Path.empty()) {
			this->m_Path = WriteExistingTempFile(this->m_FileName, this->m_Resource, fatalIfOverwriteFails);
		}

		return this->m_Path;
	}

	const std::string& BinaryResource::GetData() const {
		return this->m_Resource;
	}
}
