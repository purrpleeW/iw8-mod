#pragma once
#include "common_core.hpp"

namespace Common::Utility::IO {
	bool RemoveFile(const std::string& file);
	bool MoveFile(const std::string& src, const std::string& target);
	bool FileExists(const std::string& file);
	bool WriteFile(const std::string& file, const std::string& data, bool append = false);
	bool ReadFile(const std::string& file, std::string* data);
	std::string ReadFile(const std::string& file);
	size_t FileSize(const std::string& file);
	bool CreateDirectory(const std::string& directory);
	bool DirectoryExists(const std::string& directory);
	bool DirectoryIsEmpty(const std::string& directory);
	std::vector<std::string> ListFiles(const std::string& directory);
	void CopyFolder(const std::filesystem::path& src, const std::filesystem::path& target);
}
