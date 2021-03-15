#pragma once
#include <cassert>
#include <cstring>
#include <cmath>

template<typename T>
class vec3 {
public:
	union {
		struct {
			T x, y, z;
		};
		T value[3];
	};
public:
	vec3() : x(T(0)), y(T(0)), z(T(0)) {}
	vec3(const vec3& v): x(v[0]), y(v[1]), z(v[2]) {}
	vec3(T _x, T _y, T _z): x(_x), y(_y), z(_z) {}
	/*
	inline T x() { return x; }
	inline const T x() const { return x; }

	inline T y() { return y; }
	inline const T y() const { return y; }

	inline T z() { return z; }
	inline const T z() const { return z; }
	*/
	inline operator T* () { return value; }
	inline const T operator[](int index) const { assert(index < 3); return value[index]; }

	inline const vec3 operator+(const vec3& v) const { return vec3(x + v.x, y + v.y, z + v.z); }
	inline const vec3 operator-(const vec3& v) const { return vec3(x - v.x, y - v.y, z - v.z); }
	inline const vec3 operator-() { return vec3(-x, -y, -z); }
	inline const vec3 operator*(const T f) const { return vec3(x * f, y * f, z * f); }
	inline const vec3 operator/(const T f) const { return vec3(x / f, y / f, z / f); }

	inline vec3& operator+=(const vec3& v) { return *this = *this + v; }
	inline vec3& operator-=(const vec3& v) { return *this = *this - v; }
	inline vec3& operator*=(const T f) { return *this = *this * f; }
	inline vec3& operator/=(const T f) { return *this = *this / f; }

	inline T length() const { return static_cast<T>(sqrt(x*x + y*y + z*z)); }
	inline vec3& normalize() {
		T inv = 1.f / length();
		x *= inv;
		y *= inv;
		z *= inv;
		return *this;
	}
};

template<typename T>
vec3<T>& normalize(vec3<T>& v)  {
	T inv = 1.f / v.length();
	v.x *= inv;
	v.y *= inv;
	v.z *= inv;
	return v;
}

template<typename T>
T dot(const vec3<T>& a, const vec3<T>& b) {
	return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}
template<typename T>
vec3<T> cross(const vec3<T>& a, const vec3<T>& b) {
	return vec3(
		a[1]*b[2] - a[2]*b[1],
	  -(a[0]*b[2] - a[2]*b[0]),
		a[0]*b[1] - a[1]*b[0]);
}

using vec3f = vec3<float>;



template<typename T>
class mat4 {
public:
	union {
		// column-major [Col][Row]
		struct {
			T m11, m21, m31, m41;
			T m12, m22, m32, m42;
			T m13, m23, m33, m43;
			T m14, m24, m34, m44;
		};
		T value[16];
	};
public:
	mat4() {
		memset(&value, 0, 4 * 4 * sizeof(float));
	}
	mat4(float i) {
		memset(&value, 0, 4 * 4 * sizeof(float));
		m11 = m22 = m33 = m44 = i;
	}

	void Identity() {
		memset(&value, 0, 4 * 4 * sizeof(float));
		m11 = m22 = m33 = m44 = 1.0f;
	}
	mat4& operator*(const mat4& m) const {
		mat4 result;
		result.m11 = m11 * m.m11 + m12 * m.m21 + m13 * m.m31 + m14 * m.m41;
		result.m12 = m11 * m.m12 + m12 * m.m22 + m13 * m.m32 + m14 * m.m42;
		result.m13 = m11 * m.m13 + m12 * m.m23 + m13 * m.m33 + m14 * m.m43;
		result.m14 = m11 * m.m14 + m12 * m.m24 + m13 * m.m34 + m14 * m.m44;

		result.m21 = m21 * m.m11 + m22 * m.m21 + m23 * m.m31 + m24 * m.m41;
		result.m22 = m21 * m.m12 + m22 * m.m22 + m23 * m.m32 + m24 * m.m42;
		result.m23 = m21 * m.m13 + m22 * m.m23 + m23 * m.m33 + m24 * m.m43;
		result.m24 = m21 * m.m14 + m22 * m.m24 + m23 * m.m34 + m24 * m.m44;

		result.m31 = m31 * m.m11 + m32 * m.m21 + m33 * m.m31 + m34 * m.m41;
		result.m32 = m31 * m.m12 + m32 * m.m22 + m33 * m.m32 + m34 * m.m42;
		result.m33 = m31 * m.m13 + m32 * m.m23 + m33 * m.m33 + m34 * m.m43;
		result.m34 = m31 * m.m14 + m32 * m.m24 + m33 * m.m34 + m34 * m.m44;

		result.m41 = m41 * m.m11 + m42 * m.m21 + m43 * m.m31 + m44 * m.m41;
		result.m42 = m41 * m.m12 + m42 * m.m22 + m43 * m.m32 + m44 * m.m42;
		result.m43 = m41 * m.m13 + m42 * m.m23 + m43 * m.m33 + m44 * m.m43;
		result.m44 = m41 * m.m14 + m42 * m.m24 + m43 * m.m34 + m44 * m.m44;

		return result;
	}
};

using mat4f = mat4<float>;

static void MatRotateX(mat4f& m, float Angle) {
	float sin_ = std::sin(Angle);
	float cos_ = std::cos(Angle);
	m.m22 = m.m33 = cos_;
	m.m32 = sin_;
	m.m23 = -sin_;
}

static void MatRotateZ(mat4f& m, float Angle) {
	float sin_ = std::sin(Angle);
	float cos_ = std::cos(Angle);
	m.m11 = m.m22 = cos_;
	m.m21 = sin_;
	m.m12 = -sin_;
}

static void MatTranslate(mat4f& m, vec3f& vec) {
	m.Identity();
	m.m14 = vec[0];
	m.m24 = vec[1];
	m.m34 = vec[2];
}

static void MatMakeOrtho2D(mat4f& m, float left, float right, float bottom, float top, float zNear, float zFar)
{

	m.m11 = 2.0f / (right - left);	m.m12 = 0.0f;					m.m13 = 0.0f;					m.m14 = -(right + left) / (right - left);
	m.m21 = 0.0f;					m.m22 = 2.0f / (top - bottom);	m.m23 = 0.0f;					m.m24 = -(top + bottom) / (top - bottom);
	m.m31 = 0.0f;					m.m32 = 0.0f;					m.m33 = -2.0f / (zFar - zNear);	m.m34 = -(zFar + zNear) / (zFar - zNear);
	m.m41 = 0.0f;					m.m42 = 0.0f;					m.m43 = 0.0f;					m.m44 = 1.0f;

}


static void MatMakeLookAt(mat4f& m, const vec3f& CamTo, const vec3f& CamFrom, vec3f& CamUp) {
	/* Gram–Schmidt process */
	vec3f zaxis = CamFrom - CamTo;
	zaxis.normalize();

	vec3f xaxis = cross(CamUp.normalize(), zaxis);
	xaxis.normalize();

	vec3f yaxis = cross(zaxis, xaxis);

	/* Setup matrix */
	m.Identity();
	m.m11 = xaxis[0]; m.m21 = xaxis[1]; m.m31 = xaxis[2]; m.m41 = -dot(xaxis, CamFrom);
	m.m12 = yaxis[0]; m.m22 = yaxis[1]; m.m32 = yaxis[2]; m.m42 = -dot(yaxis, CamFrom);
	m.m13 = zaxis[0]; m.m23 = zaxis[1]; m.m33 = zaxis[2]; m.m43 = -dot(zaxis, CamFrom);
}