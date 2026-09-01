#pragma once
#include "engine/engine_common.hpp"

namespace IW8 {
	union GamerProfileDataUnion {
		std::uint8_t m_ByteVal;		// 0x0000
		bool m_BoolVal;				// 0x0000
		std::int16_t m_ShortVal;	// 0x0000
		int m_IntVal;				// 0x0000
		float m_FloatVal;			// 0x0000
		const char* m_StringVal;	// 0x0000
	};
	ENGINE_ASSERT_SZ(GamerProfileDataUnion, 0x0008);
}
