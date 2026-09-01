#pragma once
#include "engine/engine_common.hpp"
#include "engine/iw8/vec2_t.hpp"
#include "engine/iw8/vec4_t.hpp"

namespace IW8 {
	class FontGlowStyle {
	public:
		float m_GlowMinDistance;		// 0x0000
		float m_GlowMaxDistance;		// 0x0004
		vec2_t m_GlowUVOffset;			// 0x0008
		vec4_t m_GlowColor;				// 0x0010
		float m_OutlineGlowMinDistance;	// 0x0020
		float m_OutlineGlowMaxDistance;	// 0x0024
		vec4_t m_OutlineGlowColor;		// 0x0028
	};
	ENGINE_ASSERT_SZ(FontGlowStyle, 0x0038);
}
