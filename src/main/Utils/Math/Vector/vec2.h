#pragma once
#include "../FastMath.h"
template <typename T0, typename T1>
struct vec2 {
	template <typename U, typename T>
	vec2(U x_, T y_) : x((T0)x_), y((T1)y_)) {};

	vec2() : x(0.0f), y(0.0f), z(0.0f) {};

	vec2& operator+(const vec2& vec2) {
		x += (T0)vec2.x;
		y += (T1)vec2.y;
		return *this;
	}
	vec2& operator-(const vec2& vec2) {
		x -= (T0)vec2.x;
		y -= (T1)vec2.y;
		return *this;
	}

	vec2& operator+=(const vec2& vec2) {
		x += (T0)vec2.x;
		y += (T1)vec2.y;
		return *this;
	}
	vec2& operator-=(const vec2& vec2) {
		x -= (T0)vec2.x;
		y -= (T1)vec2.y;
		return *this;
	}
	//vec2  num
	template <class T>
	vec2& operator+(T a) {
		x += (T0)a;
		y += (T1)a;
		return *this;
	}
	template <class T>
	vec2& operator-(T s) {
		x -= (T0)a;
		y -= (T1)a;
		return *this;
	}
	template <class T>
	vec2& operator*(T m) {
		x *= (T0)a;
		y *= (T1)a;
		return *this;
	}
	template <class T>
	vec2& operator/(T d) {
		x /= (T0)a;
		y /= (T1)a;
		return *this;
	}

	template <class T>
	vec2& operator+=(T a) {
		x += a;
		y += a;
		return *this;
	}
	template <class T>
	vec2& operator-=(T s) {
		x -= a;
		y -= a;
		return *this;
	}
	template <class T>
	vec2& operator*=(T m) {
		x *= a;
		y *= a;
		return *this;
	}
	template <class T>
	vec2& operator/=(T d) {
		x /= a;
		y /= a;
		return *this;
	}

	T0 x;
	T1 y;
};

typedef vec2<int, int> ivec2;
typedef vec2<float, float> fvec2;