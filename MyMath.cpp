#include "MyMath.h"

float ConvertToRadian(float degree) {
	const float PI = 3.14159265f;

	float radian = 0;
	radian = degree * (PI / 180);

	return radian;
}

float AbsoluteValue(float f) {
	f = (f * f) / f;
	return f;
}
