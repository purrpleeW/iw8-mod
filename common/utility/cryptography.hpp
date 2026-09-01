#pragma once
#include "common_core.hpp"

namespace Common::Utility::Cryptography {
	namespace JenkinsOneAtATime {
		unsigned int Compute(const std::string& data);
		unsigned int Compute(const char* key, size_t len);
	}
}
