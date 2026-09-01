#pragma once
#include "engine/engine_common.hpp"

namespace IW8 {
	enum class GamerProfileDataType : int {
		TYPE_INVALID = 0x0,
		TYPE_BYTE = 0x1,
		TYPE_BOOL = 0x2,
		TYPE_SHORT = 0x3,
		TYPE_INT = 0x4,
		TYPE_FLOAT = 0x5,
		TYPE_STRING = 0x6,
		TYPE_BUFFER = 0x7,
		TYPE_FLAG = 0x8
	};
}
