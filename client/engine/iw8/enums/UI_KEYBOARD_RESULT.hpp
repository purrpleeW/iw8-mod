#pragma once
#include "engine/engine_common.hpp"

namespace IW8 {
	enum class UI_KEYBOARD_RESULT : int {
		UI_KEYBOARD_RESULT_OK = 0,
		UI_KEYBOARD_RESULT_PROFANITY = 1,
		UI_KEYBOARD_RESULT_INVALID_STRING = 2,
		UI_KEYBOARD_RESULT_CANCELLED = 3,
		UI_KEYBOARD_RESULT_ERROR = 4
	};
}
