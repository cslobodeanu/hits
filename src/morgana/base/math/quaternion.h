#ifndef __MORGANA_BASE_MATH_QUATERNION_H__
#define __MORGANA_BASE_MATH_QUATERNION_H__

#include "vector3.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Math
		{
			class Matrix;
			class Quaternion
			{
			public:
				float x;
				float y;
				float z;
				float w;
	
				inline Quaternion() { /*this = Identity;*/ }
				inline Quaternion(const float X, const float Y, const float Z, const float W) : x(X), y(Y), z(Z), w(W) {}
				~Quaternion() {}
	
				inline float * Ptr() { return &x; }
				inline const float * Ptr() const { return &x; }
	
				//Member functions - In-place rotations
				Quaternion &		RotateX(const float angle);
				Quaternion &		RotateY(const float angle);
				Quaternion &		RotateZ(const float angle);
				Quaternion &		Rotate(const Vector3 *const axis, const float angle);
				void operator *=	(const Quaternion &q);
	
				inline void			Conjugate() { x = -x; y = -y; z = -z; }
				void				Normalize();
	
				float				Yaw() const;
				float				Pitch() const;
				float				Roll()	const;

				Vector3				Forward() const;
				Vector3				RightDir() const;
				Vector3				UpDir() const;

				static const Quaternion Identity;
				static const Quaternion Left;
				static const Quaternion Right;
				static const Quaternion Up;
				static const Quaternion Down;
				static const Quaternion Backward;
	
				static Quaternion	CreateFromAxisAngle(const Vector3 *const axis, const float angle);
				static void			CreateFromAxisAngle(const Vector3 *const axis, const float angle, Quaternion *const result);
				static Quaternion	CreateFromYawPitchRoll(const float yaw, const float pitch, const float roll);
				static void			CreateFromYawPitchRoll(const float yaw, const float pitch, const float roll, Quaternion *const result);
				static Quaternion	CreateFromRotationMatrix(const Matrix *const matrix);
				static void			CreateFromRotationMatrix(const Matrix *const matrix, Quaternion *const result);
				static Quaternion	CreateFromDirection(const Vector3 *const direction);
				static void			CreateFromDirection(const Vector3 *const direction, Quaternion *const result);
				static Quaternion	CreateRotationX(const float angle);
				static void			CreateRotationX(const float angle, Quaternion *const result);
				static Quaternion	CreateRotationY(const float angle);
				static void			CreateRotationY(const float angle, Quaternion *const result);
				static Quaternion	CreateRotationZ(const float angle);
				static void			CreateRotationZ(const float angle, Quaternion *const result);
	
				static Quaternion	RotateX(const Quaternion *const q, const float angle);
				static void			RotateX(const Quaternion *const q, const float angle, Quaternion *const result);
				static Quaternion	RotateY(const Quaternion *const q, const float angle);
				static void			RotateY(const Quaternion *const q, const float angle, Quaternion *const result);
				static Quaternion	RotateZ(const Quaternion *const q, const float angle);
				static void			RotateZ(const Quaternion *const q, const float angle, Quaternion *const result);
				static Quaternion 	Rotate(const Quaternion *const q, const Vector3 *const axis, const float angle);
				static void			Rotate(const Quaternion *const q, const Vector3 *const axis, const float angle, Quaternion *const result);
	
				static Quaternion	Concatenate(const Quaternion *const q1, const Quaternion *const q2);
				static Quaternion	Concatenate(const Quaternion& q1, const Quaternion& q2);
				static void			Concatenate(const Quaternion *const q1, const Quaternion *const q2, Quaternion *const result);
				static Quaternion	Conjugate(const Quaternion *const q);
				static Quaternion	Conjugate(const Quaternion& q);
				static void			Conjugate(const Quaternion *const q, Quaternion *const result);
				static Quaternion	Slerp(const Quaternion *const q1, const Quaternion *const q2, const float ammount);
				static void			Slerp(const Quaternion *const q1, const Quaternion *const q2, const float ammount, Quaternion *const result);
				static float		Dot(const Quaternion *const q1, const Quaternion *const q2);
				static float		GetDifferenceAngle(const Quaternion *const q1, const Quaternion *const q2);

				static Quaternion	LookAt(const Vector3 *const fwd, const Vector3 *const up);

				const bool Equals(const Quaternion& other ) const;
				bool operator!=(const Quaternion& rhs) const;
				bool operator==(const Quaternion& rhs) const;

				static const Quaternion	FromEuler(const float x, const float y, const float z);
				static const Quaternion	FromEuler(const Vector3 angles);
				const Vector3		ToEuler() const;

				static Quaternion	MultiplyVector(const Quaternion * const q, const Vector3 *v);
				static Vector3		RotateVector(const Quaternion * const q, const Vector3 *v);
				static Vector3		RotateVector(const Quaternion& q, const Vector3& v);
			};

			inline Vector3 operator * (const Quaternion &q, const Vector3 &v)	{ return Quaternion::RotateVector(&q, &v); }
			inline Quaternion operator * (const Quaternion &q1, const Quaternion &q2)	{ return Quaternion::Concatenate(q1, q2); }
		}
	}
}

#endif