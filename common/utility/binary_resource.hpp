#pragma once
#include "common_core.hpp"

namespace Common::Utility {
	class BinaryResource {
	public:
		BinaryResource(int id, std::string file);

		std::string GetExtractedFile(bool fatalIfOverwriteFails = false);
		const std::string& GetData() const;
	private:
		std::string m_Resource;
		std::string m_FileName;
		std::string m_Path;
	};
}
