#pragma once
template <typename X, typename Y, typename Z, typename W>
struct vec4 {
	template <typename X_, typename Y_, typename Z_, typename W_>
	vec4(X_ x_, Y_ y_,Z_ z_, W_ w_) : x((X)x_), y((Y)y_), z((Z)z_), w((W)w_) {};

	vec4() : x(0), y(0), z(0), w(0) {};

	vec4& operator+(const vec4& Rvec4) {
		x += (X)Rvec4.x;
		y += (Y)Rvec4.y;
		z += (Z)Rvec4.z;
		w += (W)Rvec4.w;
		return *this;
	}
	vec4& operator-(const vec4& Rvec4) {
		x -= (X)Rvec4.x;
		y -= (Y)Rvec4.y;
		z -= (Z)Rvec4.z;
		w -= (W)Rvec4.w;
		return *this;
	}

	vec4& operator+=(const vec4& Rvec4) {
		x += (X)Rvec4.x;
		y += (Y)Rvec4.y;
		z += (Z)Rvec4.z;
		w += (W)Rvec4.w;
		return *this;
	}
	vec4& operator-=(const vec4& Rvec4) {
		x -= (X)Rvec4.x;
		y -= (Y)Rvec4.y;
		z -= (Z)Rvec4.z;
		w -= (W)Rvec4.w;
		return *this;
	}
	//vec4  num
	template <class T>
	vec4& operator+(T a) {
		x += (X)a;
		y += (Y)a;
		z += (Z)a;
		w += (W)a;
		return *this;
	}
	template <class T>
	vec4& operator-(T s) {
		x -= (X)a;
		y -= (Y)a;
		z -= (Z)a;
		w -= (W)a;
		return *this;
	}
	template <class T>
	vec4& operator*(T m) {
		x *= (X)a;
		y *= (Y)a;
		z *= (Z)a;
		w *= (Z)a;
		return *this;
	}
	template <class T>
	vec4& operator/(T d) {
		x /= (X)a;
		y /= (Y)a;
		z /= (Z)a;
		w /= (W)a;
		return *this;
	}

	template <class T>
	vec4& operator+=(T a) {
		x += a;
		y += a;
		z += a;
		w += a;
		return *this;
	}
	template <class T>
	vec4& operator-=(T s) {
		x -= a;
		y -= a;
		z -= a;
		w -= a;
		return *this;
	}
	template <class T>
	vec4& operator*=(T m) {
		x *= a;
		y *= a;
		z *= a;
		w *= a;
		return *this;
	}
	template <class T>
	vec4& operator/=(T d) {
		x /= a;
		y /= a;
		z /= a;
		w /= a;
		return *this;
	}

	X x;
	Y y;
	Z z;
	W w;
};

typedef vec4<int, int, int, int> ivec4;
typedef vec4<float, float, float, float> fvec4;