#pragma once
#include "engine/engine_common.hpp"
#include "engine/IW8/lua_State.hpp"

namespace IW8 {
	class LUIMethod {
	public:
		const char* m_Name;							// 0x0000
		int(__fastcall* m_Function)(lua_State*);	// 0x0008
		LUIMethod* m_Next;							// 0x0010
	};
	ENGINE_ASSERT_SZ(LUIMethod, 0x0018);
}
