#include "common_core.hpp"
#include "utility/io.hpp"

namespace Common::Utility::IO {
	bool RemoveFile(const std::string& file) {
		return DeleteFileA(file.data()) == TRUE;
	}

	bool MoveFile(const std::string& src, const std::string& target) {
		return MoveFileA(src.data(), target.data()) == TRUE;
	}

	bool FileExists(const std::string& file) {
		return std::ifstream(file).good();
	}

	bool WriteFile(const std::string& file, const std::string& data, const bool append) {
		const auto pos = file.find_last_of("/\\");
		if (pos != std::string::npos) {
			CreateDirectory(file.substr(0, pos));
		}

		std::ofstream stream(file, std::ios::binary | std::ofstream::out | (append ? std::ofstream::app : 0));

		if (stream.is_open()) {
			stream.write(data.data(), static_cast<std::streamsize>(data.size()));
			stream.close();
			return true;
		}

		return false;
	}

	std::string ReadFile(const std::string& file) {
		std::string data;
		ReadFile(file, &data);
		return data;
	}

	bool ReadFile(const std::string& file, std::string* data) {
		if (!data) {
			return false;
		}
		data->clear();

		if (FileExists(file)) {
			std::ifstream stream(file, std::ios::binary);
			if (!stream.is_open()) {
				return false;
			}

			stream.seekg(0, std::ios::end);
			const std::streamsize size = stream.tellg();
			stream.seekg(0, std::ios::beg);

			if (size > -1) {
				data->resize(static_cast<uint32_t>(size));
				stream.read(data->data(), size);
				stream.close();
				return true;
			}
		}

		return false;
	}

	size_t FileSize(const std::string& file) {
		if (FileExists(file)) {
			std::ifstream stream(file, std::ios::binary);

			if (stream.good()) {
				stream.seekg(0, std::ios::end);
				return static_cast<size_t>(stream.tellg());
			}
		}

		return 0;
	}

	bool CreateDirectory(const std::string& directory) {
		return std::filesystem::create_directories(directory);
	}

	bool DirectoryExists(const std::string& directory) {
		return std::filesystem::is_directory(directory);
	}

	bool DirectoryIsEmpty(const std::string& directory) {
		return std::filesystem::is_empty(directory);
	}

	std::vector<std::string> ListFiles(const std::string& directory) {
		std::vector<std::string> files;

		for (auto& file : std::filesystem::directory_iterator(directory)) {
			files.push_back(file.path().generic_string());
		}

		return files;
	}

	void CopyFolder(const std::filesystem::path& src, const std::filesystem::path& target) {
		using namespace std::filesystem;
		copy(src, target, copy_options::overwrite_existing | copy_options::recursive);
	}
}
