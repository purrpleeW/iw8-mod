#pragma once
#include "common.hpp"

#define ENGINE_ASSERT_SZ(cls, sz) static_assert(sizeof(cls) == sz, #cls " is not " #sz " bytes in size.")
