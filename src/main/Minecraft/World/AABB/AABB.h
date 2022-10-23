#pragma once
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
class AABB_BOX {
	double x = 0.0;
	double y = 0.0;
	double z = 0.0;

	double y_size = 0;
	double x_size = 0;
	double z_size = 0;

public:
	template <class T, class Y, class U> void SetPos(T x1, Y y1, U z1) {
		x = (double)x1;
		y = (double)y1;
		z = (double)z1;
	}

	template <class T, class Y, class U> void SetSize(T x1, Y y1, U z1) {
		x_size = (double)x1;
		y_size = (double)y1;
		z_size = (double)z1;
	}

	template <class T, class Y, class U> bool TestIntersect(T x1, Y y1, U z1) {
		double xmin = x - x_size;
		double xmax = x + x_size;

		double ymin = y - y_size;
		double ymax = y + y_size;

		double zmin = z - z_size;
		double zmax = z + z_size;

		if (
			((xmin <= (double)x1) && (xmax >= (double)x1)) &&
			((ymin <= (double)y1) && (ymax >= (double)y1)) &&
			((zmin <= (double)z1) && (zmax >= (double)z1))
			) {
			return true;
		}
		return false;
	}
};