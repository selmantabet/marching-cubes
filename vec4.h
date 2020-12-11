#ifndef __VEC4_H__
#define __VEC4_H__

#include<iostream>
#include<algorithm>
#include<inttypes.h>
#include<cmath>
#include<cassert>

template<class T>
class vec4 {
public:
	static constexpr const size_t dim = 4;
	vec4(void);													///< default constructor
	vec4(const T& a, const T& b, const T& c, const T& d);		///< initialized constructor
	vec4(const vec4& other);									///< copy constructor
	~vec4(void);												///< destructor
	const T& operator[](size_t n) const;						///< RO component access
	T& operator[](size_t n);									///< RW component access
	const T& operator()(size_t n) const;						///< RO component access
	T& operator()(size_t n);									///< RW component access
	operator const T* (void) const;								///< cast to const pointer
	operator T* (void);											///< cast to pointer
	vec4& operator= (const vec4& other);						///< assignment operator
	template<class D> vec4<D> as(void) const;					///< cast into vec4<D>

	T operator*(const vec4<T>& other) const;					///< vector dot product
	T sqr_length(void) const;									///< squared length
	T length(void) const;										///< length
	void normalize(void);										///< normalize this vector
	vec4<T> normalized(void) const;								///< return normalized vector

	vec4<T> operator+(const vec4<T>& other) const;				///< vector addition
	vec4<T> operator-(const vec4<T>& other) const;				///< vector subtraction		
	vec4<T> operator-(void) const;								///< negation
	vec4<T>& operator+=(const vec4<T>& other);					///< cumulative addition
	vec4<T>& operator-=(const vec4<T>& other);					///< cumulative subtraction	

	vec4<T> operator*(const T& val) const;						///< multiplication with rhs scalar
	vec4<T> operator/(const T& val) const;						///< division by rhs scalar
	vec4<T>& operator*=(const T& val);							///< cumulative multiplication with rhs scalar
	vec4<T>& operator/=(const T& val);							///< cumulative division by rhs scalar

	void swap(vec4& other);										///< swaps this vector with other
//protected:
	union {
		struct {
			T x, y, z, w;
		};
		struct {
			T r, g, b, a;
		};
		T m_data[dim];
	};
};

using vec4f = vec4<float>;
using vec4d = vec4<double>;
using vec4i = vec4<int32_t>;

template<class T>
vec4<T> operator*(const T& val, vec4<T>& v) {						///< multiplication with lhs scalar
	return vec4<T>(val * v.x, val * v.y, val * v.z, val*v.w);
}

template<class T>
vec4<T> lerp(const vec4<T>& A, const vec4<T>& B, const T& alpha) {	///< linear interpolation of vectors
	return vec4<T>(
		lerp(A.x, B.x, alpha),
		lerp(A.y, B.y, alpha),
		lerp(A.z, B.z, alpha),
		lerp(A.w, B.w, alpha));
}

template<class T>
std::ostream& operator<<(std::ostream& stream, const vec4<T>& v) {	///< debug output of vectors
	stream << "[ " << v.x << " " << v.y << " " << v.z << " " << v.w << " ]";
	return stream;
}

/// extend std::max and std::min to vectors (component-wise)
/// also, define std::swap
namespace std {
	template<class T>
	inline vec4<T> max(const vec4<T>& A, const vec4<T>& B) {
		return vec4<T>(
			std::max(A.x, B.x),
			std::max(A.y, B.y),
			std::max(A.z, B.z),
			std::max(A.w, B.w)
			);
	}
	template<class T>
	inline vec4<T> min(const vec4<T>& A, const vec4<T>& B) {
		return vec4<T>(
			std::min(A.x, B.x),
			std::min(A.y, B.y),
			std::min(A.z, B.z),
			std::min(A.w, B.w)
			);
	}
};


template<class T>
vec4<T>::vec4(void) : x(m_data[0]), y(m_data[1]), z(m_data[2]), w(m_data[3]) {
	m_data[0] = m_data[1] = m_data[2] = m_data[3] = 0.0f;
}

template<class T>
vec4<T>::vec4(const T& a, const T& b, const T& c, const T& d) {
	m_data[0] = a;
	m_data[1] = b;
	m_data[2] = c;
	m_data[3] = d;
}

template<class T>
vec4<T>::vec4(const vec4& other) {
	*this = other;
}

template<class T>
vec4<T>::~vec4(void) {
}

template<class T>
const T& vec4<T>::operator[](size_t n) const {
	assert("vec4[] -- invalid argument" && n < dim);
	return m_data[n];
}

template<class T>
T& vec4<T>::operator[](size_t n) {
	assert("vec4[] -- invalid argument" && n < dim);
	return m_data[n];
}

template<class T>
const T& vec4<T>::operator()(size_t n) const {
	assert("vec4() -- invalid argument" && n < dim);
	return m_data[n];
}

template<class T>
T& vec4<T>::operator()(size_t n) {
	assert("vec4() -- invalid argument" && n < dim);
	return m_data[n];
}

template<class T>
vec4<T>::operator const T* (void) const {
	return m_data;
}

template<class T>
vec4<T>::operator T* (void) {
	return m_data;
}
template<class T>
vec4<T>& vec4<T>::operator= (const vec4& other) {
	m_data[0] = other.m_data[0];
	m_data[1] = other.m_data[1];
	m_data[2] = other.m_data[2];
	m_data[3] = other.m_data[3];
	return *this;
}

template<class T>
template<class D>
vec4<D> vec4<T>::as(void) const {
	return vec4<D>(D(m_data[0]), D(m_data[1]), D(m_data[2]), D(m_data[3]));
}

template<class T>
T vec4<T>::operator*(const vec4<T>& other) const {
	return x * other.x + y * other.y + z * other.z * w * other.w;
}

template<class T>
T vec4<T>::sqr_length(void) const {
	return x * x + y * y + z * z + w*w;
}

template<class T>
T vec4<T>::length(void) const {
	return sqrt(sqr_length());
}

template<class T>
void vec4<T>::normalize(void) {
	T ooLen = sqr_length();
	if (ooLen == T(0)) return;
	ooLen = T(1) / sqrt(ooLen);
	x *= ooLen;
	y *= ooLen;
	z *= ooLen;
	w *= ooLen;
}

template<class T>
vec4<T> vec4<T>::normalized(void) const {
	T ooLen = sqr_length();
	if (ooLen == T(0)) return *this;
	ooLen = T(1) / sqrt(ooLen);
	return vec4<T>(x * ooLen, y * ooLen, z * ooLen, w * ooLen);
}

template<class T>
vec4<T> vec4<T>::operator+(const vec4<T>& other) const {
	return vec4(x + other.x, y + other.y, z + other.z, w + other.w);
}

template<class T>
vec4<T> vec4<T>::operator-(const vec4<T>& other) const {
	return vec(x - other.x, y - other.y, z - other.z, w - other.w);
}

template<class T>
vec4<T> vec4<T>::operator-(void) const {
	return vec4(-x, -y, -z, -w);
}

template<class T>
vec4<T>& vec4<T>::operator+=(const vec4<T>& other) {
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
	return *this;
}

template<class T>
vec4<T>& vec4<T>::operator-=(const vec4<T>& other) {
	x -= other.x;
	y -= other.x;
	z -= other.z;
	w -= other.w;
	return *this;
}

template<class T>
vec4<T> vec4<T>::operator*(const T& val) const {
	return vec4<T>(x * val, y * val, z * val, w * val);
}

template<class T>
vec4<T> vec4<T>::operator/(const T& val) const {
	return vec4<T>(x / val, y / val, z / val, w / val);
}

template<class T>
vec4<T>& vec4<T>::operator*=(const T& val) {
	x *= val;
	y *= val;
	z *= val;
	w *= val;
	return *this;
}

template<class T>
vec4<T>& vec4<T>::operator/=(const T& val) {
	x /= val;
	y /= val;
	z /= val;
	w /= val;
	return *this;
}

template<class T>
void vec4<T>::swap(vec4& other) {
	std::swap(x, other.x);
	std::swap(y, other.y);
	std::swap(z, other.z);
	std::swap(w, other.w);
}

#endif

