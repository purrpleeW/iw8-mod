#pragma once
#include "engine/engine_common.hpp"
#include "engine/iw8/lua_State.hpp"

namespace IW8 {
	class luaL_Reg {
	public:
		const char* m_Name;						// 0x0000
		int(__fastcall* m_Func)(lua_State*);	// 0x0008
	};
	ENGINE_ASSERT_SZ(luaL_Reg, 0x0010);
}
