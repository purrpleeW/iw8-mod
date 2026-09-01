#pragma once
#include "engine/engine_common.hpp"
#include "engine/iw8/enums/GamerProfileDataType.hpp"

namespace IW8 {
	class GamerProfileDataField {
	public:
		const char* m_SettingName;		// 0x0000
		std::int64_t m_Offset;			// 0x0008
		GamerProfileDataType m_Type;	// 0x0010
		int m_Size;						// 0x0014
		float m_DefaultVal;				// 0x0018
		float m_MinVal;					// 0x001C
		float m_MaxVal;					// 0x0020
		bool m_IsGameOption;			// 0x0024
	private:
		char pad_0025[0x0003];			// 0x0025
	public:
		int m_BitShift;					// 0x0028
	private:
		char pad_002C[0x0004];			// 0x002C
	};
	ENGINE_ASSERT_SZ(GamerProfileDataField, 0x0030);
}
