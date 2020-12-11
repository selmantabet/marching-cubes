#ifndef __EXT_MATH_H__
#define __EXT_MATH_H__

#include<type_traits>
#include<algorithm>
#include<iostream>

/// Linear interpolation between A and B for scalar data
template<class T>
typename std::enable_if_t<std::is_arithmetic_v<T>, T> lerp(const T& A, const T& B, const T& alpha) {
	return A + alpha * (B - A);
}

/// clamp for scalars
template<class T>
typename std::enable_if_t<std::is_arithmetic_v<T>, T> clamp(const T& val, const T& vmin = T(0), const T& vmax=T(1)) {
	return std::min(std::max(val, vmin), vmax);
}

#include"vec3.h"
#include"vec4.h"

#endif
