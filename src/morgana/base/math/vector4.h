#ifndef __MORGANA_BASE_MATH_VECTOR4_H__
#define __MORGANA_BASE_MATH_VECTOR4_H__

#include "Vector2.h"
#include "vector3.h"
namespace MorganaEngine
{
	namespace Base
	{
		namespace Math
		{
			class Vector4 : public Vector3
			{
			public:
				float w;
				static const Vector4 Zero;
				static const Vector4 One;

				inline Vector4() {}
				inline Vector4(const float X, const float Y, const float Z, const float W): Vector3(X, Y, Z), w(W) {}
				inline Vector4(const float X, const float Y, const float Z): Vector3(X, Y, Z), w(0.f) {}
				inline Vector4(const float X, const float Y): Vector3(X, Y, 0.f), w(0.f) {}
				inline Vector4(const float X): Vector3(X, 0.f, 0.f), w(0.f) {}
				inline Vector4(const Vector3 &v, const float W)	: Vector3(v.x, v.y, v.z), w(W) {}
				inline Vector4(const Vector3 &v) : Vector3(v.x, v.y, v.z), w(0.f) {}
				inline Vector4(const Vector2 &v, const float Z, const float W) : Vector3(v.x, v.y, Z), w(W) {}
				inline Vector4(const Vector2 &v, const float Z) : Vector3(v.x, v.y, Z), w(0.f) {}
				inline Vector4(const Vector2 &v) : Vector3(v.x, v.y, 0.f), w(0.f) {}
				inline Vector4(const float *const xyzw) : Vector3(xyzw[0], xyzw[1], xyzw[2]), w(xyzw[3]) {}
			};
		}
	}
}

#endif