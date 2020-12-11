#ifndef __VEC3_H__
#define __VEC3_H__

#include<iostream>
#include<algorithm>
#include<inttypes.h>
#include<cmath>
#include<cassert>

template<class T>
class vec3 {
public:
	static constexpr const size_t dim = 3;
	vec3(void);													///< default constructor
	vec3(const T& a, const T& b, const T& c);					///< initialized constructor
	vec3(const vec3& other);									///< copy constructor
	~vec3(void);												///< destructor
	const T& operator[](size_t n) const;						///< RO component access
	T& operator[](size_t n);									///< RW component access
	const T& operator()(size_t n) const;						///< RO component access
	T& operator()(size_t n);									///< RW component access
	operator const T* (void) const;								///< cast to const pointer
	operator T* (void);											///< cast to pointer
	//T& x, & y, & z;												///< named attributes .x .y .z
	//T& r, & g, & b;												///< named attributes .r .g .b
	vec3& operator= (const vec3& other);						///< assignment operator
	template<class D> vec3<D> as(void) const;					///< cast into vec3<D>
	inline T dot(const vec3<T> & other) const;					///< dot product

	T operator*(const vec3<T>& other) const;					///< vector dot product
	T sqr_length(void) const;									///< squared length
	T length(void) const;										///< length
	void normalize(void);										///< normalize this vector
	vec3<T> normalized(void) const;								///< return normalized vector

	vec3<T> operator+(const vec3<T>& other) const;				///< vector addition
	vec3<T> operator-(const vec3<T>& other) const;				///< vector subtraction	
	vec3<T> operator^(const vec3<T>& other) const;				///< vector cross product
	vec3<T> operator-(void) const;								///< negation
	vec3<T>& operator+=(const vec3<T>& other);					///< cumulative addition
	vec3<T>& operator-=(const vec3<T>& other);					///< cumulative subtraction
	vec3<T>& operator^=(const vec3<T>& other);					///< cumulative cross product

	vec3<T> operator*(const T& val) const;						///< multiplication with rhs scalar
	vec3<T> operator/(const T& val) const;						///< division by rhs scalar
	vec3<T>& operator*=(const T& val);							///< cumulative multiplication with rhs scalar
	vec3<T>& operator/=(const T& val);							///< cumulative division by rhs scalar

	void swap(vec3& other);										///< swaps this vector with other

//protected:
	union {
		struct {
			T x, y, z;
		};
		struct {
			T r, g, b;
		};
		T m_data[dim];
	};
};

using vec3f = vec3<float>;
using vec3d = vec3<double>;
using vec3i = vec3<int32_t>;

template<class T>
vec3<T> operator*(const T& val, vec3<T>& v) {						///< multiplication with lhs scalar
	return vec3<T>(val * v.x, val * v.y, val * v.z);
}

template<class T>
vec3<T> lerp(const vec3<T>& A, const vec3<T>& B, const T& alpha) {	///< linear interpolation of vectors
	return vec3<T>(
		lerp(A.x, B.x, alpha),
		lerp(A.y, B.y, alpha),
		lerp(A.z, B.z, alpha));
}

template<class T>
std::ostream& operator<<(std::ostream& stream, const vec3<T>& v) {	///< debug output of vectors
	stream << "[ " << v.x << " " << v.y << " " << v.z << " ]";
	return stream;
}

template<class T>
vec3<T> clamp(const vec3<T>& val, const vec3<T>& vmin = vec3<T>(T(0), T(0), T(0)), const vec3<T>& vmax = vec3<T>(T(0), T(0), T(0))) {	///< component-wise clamp for vectors
	return vec3<T>(
		std::min<T>(std::max<T>(val.x, vmin.x), vmax.x),
		std::min<T>(std::max<T>(val.y, vmin.y), vmax.y),
		std::min<T>(std::max<T>(val.z, vmin.z), vmax.z)
		);
}

/// extend std::max and std::min to vectors (component-wise)
/// also, define std::swap
namespace std {
	template<class T>
	inline vec3<T> max(const vec3<T>& A, const vec3<T>& B) {
		return vec3<T>(
			std::max(A.x, B.x),
			std::max(A.y, B.y),
			std::max(A.z, B.z)
			);
	}
	template<class T>
	inline vec3<T> min(const vec3<T>& A, const vec3<T>& B) {
		return vec3<T>(
			std::min(A.x, B.x),
			std::min(A.y, B.y),
			std::min(A.z, B.z)
			);
	}
	template<class T>
	inline vec3<T> abs(const vec3<T>& A) {
		return vec3<T>(
			std::abs(A.x),
			std::abs(A.y),
			std::abs(A.z)
			);
	}
};


template<class T>
vec3<T>::vec3(void) {//: x(m_data[0]), y(m_data[1]), z(m_data[2]), r(m_data[0]), g(m_data[1]), b(m_data[2]) {
	m_data[0] = m_data[1] = m_data[2] = T(0);
}

template<class T>
vec3<T>::vec3(const T& a, const T& b, const T& c) {//: x(m_data[0]), y(m_data[1]), z(m_data[2]), r(m_data[0]), g(m_data[1]), b(m_data[2]) {
	m_data[0] = a;
	m_data[1] = b;
	m_data[2] = c;
}

template<class T>
vec3<T>::vec3(const vec3& other) {//: x(m_data[0]), y(m_data[1]), z(m_data[2]), r(m_data[0]), g(m_data[1]), b(m_data[2]) {
	*this = other;
}

template<class T>
vec3<T>::~vec3(void) {
}

template<class T>
const T& vec3<T>::operator[](size_t n) const {
	assert("vec3[] -- invalid argument" && n < dim);
	return m_data[n];
}

template<class T>
T& vec3<T>::operator[](size_t n) {
	assert("vec3[] -- invalid argument" && n < dim);
	return m_data[n];
}

template<class T>
const T& vec3<T>::operator()(size_t n) const {
	assert("vec3() -- invalid argument" && n < dim);
	return m_data[n];
}

template<class T>
T& vec3<T>::operator()(size_t n) {
	assert("vec3() -- invalid argument" && n < dim);
	return m_data[n];
}

template<class T>
vec3<T>::operator const T* (void) const {
	return m_data;
}

template<class T>
vec3<T>::operator T* (void) {
	return m_data;
}
template<class T>
vec3<T>& vec3<T>::operator= (const vec3& other) {
	m_data[0] = other.m_data[0];
	m_data[1] = other.m_data[1];
	m_data[2] = other.m_data[2];
	return *this;
}

template<class T>
template<class D>
vec3<D> vec3<T>::as(void) const {
	return vec3<D>(D(m_data[0]), D(m_data[1]), D(m_data[2]));
}

template<class T>
T vec3<T>::operator*(const vec3<T>& other) const {
	return x * other.x + y * other.y + z * other.z;
}

template<class T>
T vec3<T>::sqr_length(void) const {
	return x * x + y * y + z * z;
}

template<class T>
T vec3<T>::length(void) const {
	return T(sqrt(sqr_length()));
}

template<class T>
void vec3<T>::normalize(void) {
	T ooLen = sqr_length();
	if (ooLen == T(0)) return;
	ooLen = T(1) / sqrt(ooLen);
	x *= ooLen;
	y *= ooLen;
	z *= ooLen;
}

template<class T>
vec3<T> vec3<T>::normalized(void) const {
	T ooLen = sqr_length();
	if (ooLen == T(0)) return *this;
	ooLen = T(1) / sqrt(ooLen);
	return vec3<T>(x * ooLen, y * ooLen, z * ooLen);
}

template<class T>
inline T vec3<T>::dot(const vec3<T>& other) const {
	return x * other.x + y * other.y + z * other.z;
}

template<class T>
vec3<T> vec3<T>::operator+(const vec3<T>& other) const {
	return vec3(x + other.x, y + other.y, z + other.z);
}

template<class T>
vec3<T> vec3<T>::operator-(const vec3<T>& other) const {
	return vec3(x - other.x, y - other.y, z - other.z);
}

template<class T>
vec3<T> vec3<T>::operator^(const vec3<T>& other) const {
	return vec3(
		y * other.z - other.y * z,
		z * other.x - other.z * x,
		x * other.y - other.x * y
	);
}
template<class T>
vec3<T> vec3<T>::operator-(void) const {
	return vec3(-x, -y, -z);
}

template<class T>
vec3<T>& vec3<T>::operator+=(const vec3<T>& other) {
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

template<class T>
vec3<T>& vec3<T>::operator-=(const vec3<T>& other) {
	x -= other.x;
	y -= other.x;
	z -= other.z;
	return *this;
}

template<class T>
vec3<T>& vec3<T>::operator^=(const vec3<T>& other) {
	T tmp1 = y * other.z - other.y * z;
	T tmp2 = z * other.x - other.z * x;
	z = x * other.y - other.x * y;
	x = tmp1;
	y = tmp2;

}
template<class T>
vec3<T> vec3<T>::operator*(const T& val) const {
	return vec3<T>(x * val, y * val, z * val);
}

template<class T>
vec3<T> vec3<T>::operator/(const T& val) const {
	return vec3<T>(x / val, y / val, z / val);
}

template<class T>
vec3<T>& vec3<T>::operator*=(const T& val) {
	x *= val;
	y *= val;
	z *= val;
	return *this;
}

template<class T>
vec3<T>& vec3<T>::operator/=(const T& val) {
	x /= val;
	y /= val;
	z /= val;
	return *this;
}

template<class T>
void vec3<T>::swap(vec3& other) {
	std::swap(x, other.x);
	std::swap(y, other.y);
	std::swap(z, other.z);
}


#endif
