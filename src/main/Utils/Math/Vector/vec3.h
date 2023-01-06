#pragma once
template <typename T0, typename T1, typename T2>
struct vec3 {
	template <typename U, typename T, typename Y>
	vec3(U x_, T y_, Y z_) : x((T0)x_), y((T1)y_), z((T2)z_) {};

	vec3() : x(0.0f), y(0.0f), z(0.0f) {};

	vec3& operator+(const vec3& vec3) {
		x += (T0)vec3.x;
		y += (T1)vec3.y;
		z += (T2)vec3.z;
		return *this;
	}
	vec3& operator-(const vec3& vec3) {
		x -= (T0)vec3.x;
		y -= (T1)vec3.y;
		z -= (T2)vec3.z;
		return *this;
	}

	vec3& operator+=(const vec3& vec3) {
		x += (T0)vec3.x;
		y += (T1)vec3.y;
		z += (T2)vec3.z;
		return *this;
	}
	vec3& operator-=(const vec3& vec3) {
		x -= (T0)vec3.x;
		y -= (T1)vec3.y;
		z -= (T2)vec3.z;
		return *this;
	}
	//vec3  num
	template <class T>
	vec3& operator+(T a) {
		x += (T0)a;
		y += (T1)a;
		z += (T2)a;
		return *this;
	}
	template <class T>
	vec3& operator-(T s) {
		x -= (T0)a;
		y -= (T1)a;
		z -= (T2)a;
		return *this;
	}
	template <class T>
	vec3& operator*(T m) {
		x *= (T0)a;
		y *= (T1)a;
		z *= (T2)a;
		return *this;
	}
	template <class T>
	vec3& operator/(T d) {
		x /= (T0)a;
		y /= (T1)a;
		z /= (T2)a;
		return *this;
	}

	template <class T>
	vec3& operator+=(T a) {
		x += a;
		y += a;
		z += a;
		return *this;
	}
	template <class T>
	vec3& operator-=(T s) {
		x -= a;
		y -= a;
		z -= a;
		return *this;
	}
	template <class T>
	vec3& operator*=(T m) {
		x *= a;
		y *= a;
		z *= a;
		return *this;
	}
	template <class T>
	vec3& operator/=(T d) {
		x /= a;
		y /= a;
		z /= a;
		return *this;
	}

	T0 x;
	T1 y;
	T2 z;
};

typedef vec3<int, int, int> ivec3;
typedef vec3<float, float, float> fvec3;