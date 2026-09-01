#include "common_core.hpp"
#include "utility/cryptography.hpp"

namespace Common::Utility::Cryptography {
	unsigned int JenkinsOneAtATime::Compute(const std::string& data) {
		return Compute(data.data(), data.size());
	}

	unsigned int JenkinsOneAtATime::Compute(const char* key, const size_t len) {
		unsigned int hash, i;
		for (hash = i = 0; i < len; ++i) {
			hash += key[i];
			hash += (hash << 10);
			hash ^= (hash >> 6);
		}
		hash += (hash << 3);
		hash ^= (hash >> 11);
		hash += (hash << 15);
		return hash;
	}
}
