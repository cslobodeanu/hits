#ifndef __MORGANA_BASE_MATH_BOX_H__
#define __MORGANA_BASE_MATH_BOX_H__

#include "Vector3.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Math
		{
			class Box
			{
			public:
				static Box Zero;
				static Box Invalid;

				Vector3 center;
				Vector3 offset;

				inline Box() { center=Vector3::Zero; offset = Vector3::Zero; }
				inline Box(const Vector3 &Center, const Vector3 &Offset) : center(Center), offset(Offset) {}
				inline Box(const Vector3 &Center, const float &Offset) : center(Center), offset(Vector3::One * Offset) {}
				inline Box(const float &Offset) : center(Vector3::Zero), offset(Vector3::One * Offset) {}
				inline Box(const Box& other) { center = other.center; offset = other.offset; }
				~Box()	{}

				inline float * Ptr() { return &center.x; }
				inline const float * Ptr() const { return &center.x; }

				bool		Contains(const Vector3 *const pos) const;
				bool		Contains(const Vector3 *const pos, const float radius) const;

				static Box	FromMinMax(const Vector3 &minBB,const Vector3 &maxBB);
				static Box	FromPosSize(const Vector3 &pos, const float size);

				Vector3		RandomPoint(void) const;

				static void Combine(const Box &bb1,const Box &bb2,Box &bbout);
				static bool Intersects(const Box &bb1,const Box &bb2);
				static void TransformBox(const Box &inputBB,const Matrix &mat,Box &outBB);
				static void CombinePrecise(const Box &bb1,const Box &bb2,Box &bbout);

				void		GetCorners(Vector3* c);

				void		Transform(const Matrix* m);
				static void Transform(const Box *const aabb, const Matrix *const mat, Box *const result);

				const float Distance(const Box& other);
				static const float Distance(const Box& b1, const Box& b2);
				static const float Distance2(const Box& b1, const Box& b2);
			};
		}
	}
}

#endif