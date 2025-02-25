#ifndef KoiKoi_H
#define KoiKoi_H
#include "boost/serialization/strong_typedef.hpp"
#include <raylib.h>

namespace KoiKoi {
	BOOST_STRONG_TYPEDEF(float, FloatEnum)
	constexpr float cwidth = 146.0f;
	constexpr float cheight = 240.0f;

	constexpr float ywidth = 79.8f;

	constexpr float nwidth = 54.6f;

	constexpr float swidth = 42.0f;

	constexpr float ckwidth = 189.0f;

	constexpr float theight = 30.0f;

	constexpr float appearenceoffsetX = 4.0f;
	constexpr float appearenceoffsetY = 6.0f;
}

#endif // !KoiKoi_H