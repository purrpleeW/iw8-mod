#pragma once
#include "common_core.hpp"

namespace Common::Registry {
	LONG SetRegValue(HKEY hKeyIn, const char* path, const char* name, DWORD value);
	LONG GetRegValue(HKEY hKeyIn, const char* path, const char* name, DWORD& nValue, DWORD nDefaultValue);
}
