#pragma once
#include "common_core.hpp"

namespace Common {
	template<size_t N>
	struct StringLiteral {
		constexpr StringLiteral(const char(&str)[N]) {
			std::copy_n(str, N, this->m_Value);
		}

		char m_Value[N];
	};
}
