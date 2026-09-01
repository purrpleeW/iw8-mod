#pragma once
#include "common.hpp"
#include <nlohmann/json.hpp>

namespace Client::Network {
	size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
	std::optional<std::string> GetContent(const std::string& url);

	inline std::optional<nlohmann::json> GetContentAsJson(const std::string& url) {
		std::optional<std::string> content = GetContent(url);
		return content.has_value() ? std::optional{ nlohmann::json::parse(content.value()) } : std::nullopt;
	}
}

