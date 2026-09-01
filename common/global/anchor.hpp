#pragma once
#include "global/vector2.hpp"

namespace Common {
	enum class Anchor : int {
		TOP_LEFT		= 0x00,
		TOP_CENTER		= 0x01,
		TOP_RIGHT		= 0x02,
		CENTER_LEFT		= 0x10,
		CENTER			= 0x11,
		CENTER_RIGHT	= 0x12,
		BOTTOM_LEFT		= 0x20,
		BOTTOM_CENTER	= 0x21,
		BOTTOM_RIGHT	= 0x22
	};

	template <typename T>
	inline Vector2<T> GetAnchorOffset(Anchor anchor, Vector2<T> size) {
		T xOffset = 0.f;
		T yOffset = 0.f;
		int anchorData = static_cast<int>(anchor);
		Vector2<T> correctedSize = size.Div((T) 2);
		while (anchorData >= 0x10) {
			yOffset -= correctedSize.GetY();
			anchorData -= 0x10;
		}
		while (anchorData >= 0x01) {
			xOffset -= correctedSize.GetX();
			anchorData -= 0x01;
		}
		return Vector2<T>(xOffset, yOffset);
	}
}
