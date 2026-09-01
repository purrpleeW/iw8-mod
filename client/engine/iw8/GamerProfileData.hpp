#pragma once
#include "engine/engine_common.hpp"
#include "engine/iw8/GamerProfileDataUnion.hpp"
#include "engine/iw8/enums/GamerProfileDataType.hpp"

namespace IW8 {
	class GamerProfileData {
	public:
		GamerProfileDataType m_Type;	// 0x0000
	private:
		char pad_0004[0x0004];			// 0x0004
	public:
		GamerProfileDataUnion m_Union;	// 0x0008
	};
	ENGINE_ASSERT_SZ(GamerProfileData, 0x0010);
}
