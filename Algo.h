#pragma once

namespace algo {
	template<class T>
	constexpr const T& clamp(const T& v, const T& lo, const T& hi)
	{
		return std::min(std::max(v, lo), hi);
	}
}