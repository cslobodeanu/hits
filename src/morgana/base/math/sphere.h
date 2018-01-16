#ifndef __MORGANA_BASE_MATH_SPHERE_H__
#define __MORGANA_BASE_MATH_SPHERE_H__

#include "Vector3.h"
#include "matrix.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Math
		{
			class Sphere
			{
			public:
				Vector3 center;
				float	radius;

				inline Sphere() { center = Vector3::Zero; radius = 1.0f; }
				inline Sphere(const Vector3& c, const float& r) { center = c; radius = r; }
				inline Sphere(const Sphere& other) { center = other.center; radius = other.radius; }

				inline const bool	Contains(const Vector3& p) const { return Vector3::Distance2(center, p) <= radius * radius; }
				inline const bool	Contains(const Sphere& s) const { return Vector3::Distance2(center, s.center) <= (radius + s.radius) * (radius + s.radius); }

				inline Vector3		RandomPoint() const { return center + Vector3::Random() * radius; }

				inline Sphere		Transform(const Matrix& m) const
				{
					Sphere s;
					s.center = m * center;
					s.radius = radius * Vector3::Transform(&Vector3::Forward, &m).Length();

					return s;
				}
			};
		}
	}
}

#endif