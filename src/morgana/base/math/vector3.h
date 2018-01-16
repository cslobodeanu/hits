#ifndef __MORGANA_BASE_MATH_VECTOR3_H__
#define __MORGANA_BASE_MATH_VECTOR3_H__

#include "mathutils.h"
#include "vector2.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Math
		{
			class Matrix;
			class Quaternion;

			class Vector3
			{
			public:
				float x;
				float y;
				float z;

				static const Vector3 Left;
				static const Vector3 Right;
				static const Vector3 Up;
				static const Vector3 Down;
				static const Vector3 Backward;
				static const Vector3 Forward;
				static const Vector3 UnitX;
				static const Vector3 UnitY;
				static const Vector3 UnitZ;
				static Vector3 Zero;
				static const Vector3 MinFloat;
				static const Vector3 MaxFloat;
				static const Vector3 One;
				static const Vector3 Luma;
				static const Vector3 DiagonalXY;
				static const Vector3 DiagonalXZ;
				static const Vector3 DiagonalYZ;
				static const Vector3 DiagonalXYZ;
				static const Vector3 FlipX;
				static const Vector3 FlipY;
				static const Vector3 FlipZ;

				inline Vector3()                                                                        {}
				inline Vector3(const float X)								: x(X), y(X), z(X)			{}
				inline Vector3(const float X, const float Y)				: x(X), y(Y), z(0.0f)       {}
				inline Vector3(const float X, const float Y, const float Z) : x(X), y(Y), z(Z)          {}
				inline Vector3(const Vector2& v2, const float Z = 0.0f)		: x(v2.x), y(v2.y), z(Z)	{}

				inline float * Ptr() { return &x; }
				inline const float * Ptr() const { return &x; }

				void	Normalize();
				void	Negate();

				float	Length()		 const;
				float	LengthReversed() const;
				float	LengthSquared()  const;
				float   LengthXZ()	     const;
				float   LengthXZSquared()const;
				float	Yaw()			 const;
				float	Pitch()			 const;

				float Max() const;
				float Min() const;

				static float	Dot(const Vector3 *const v1, const Vector3 *const v2);
				static float    Dot(const Vector3 &v1, const Vector3 &v2) { return Dot(&v1, &v2); }
				static float    DotNorm(const Vector3 *const v1, const Vector3 *const p1v2, const Vector3 *const p2v2);

				static void		Cross(const Vector3 *const v1, const Vector3 *const v2, Vector3 *const result);
				static Vector3	Cross(const Vector3 *const v1, const Vector3 *const v2);

				static float	Distance(const Vector3 *const v1, const Vector3 *const v2);
				static float	Distance2(const Vector3 *const v1, const Vector3 *const v2);
				static float	DistanceXZ(const Vector3 *const v1, const Vector3 *const v2);
				static float	Distance2XZ(const Vector3 *const v1, const Vector3 *const v2);

				static float	Distance(const Vector3 & v1, const Vector3 & v2);
				static float	Distance2(const Vector3 & v1, const Vector3 & v2);

				static void		Add(const Vector3 *const v, const float val, Vector3 *const result);
				static Vector3	Add(const Vector3 *const v, const float val);
				static void		Add(const Vector3 *const v1, const Vector3 *const v2, Vector3 *const result);
				static Vector3	Add(const Vector3 *const v1, const Vector3 *const v2);
				static void		Subtract(const Vector3 *const v, const float val, Vector3 *const result);
				static Vector3	Subtract(const Vector3 *const v, const float val);
				static void		Subtract(const Vector3 *const v1, const Vector3 *const v2, Vector3 *const result);
				static Vector3	Subtract(const Vector3 *const v1, const Vector3 *const v2);
				static void		Divide(const Vector3 *const v, const float val, Vector3 *const result);
				static Vector3	Divide(const Vector3 *const v, const float val);
				static void		Divide(const Vector3 *const v1, const Vector3 *const v2, Vector3 *const result);
				static Vector3	Divide(const Vector3 *const v1, const Vector3 *const v2);
				static void		Multiply(const Vector3 *const v, const float val, Vector3 *const result);
				static Vector3	Multiply(const Vector3 *const v, const float val);
				static void		Multiply(const Vector3 *const v1, const Vector3 *const v2, Vector3 *const result);
				static Vector3	Multiply(const Vector3 *const v1, const Vector3 *const v2);
				static Vector3	Multiply(const Vector3& v1, const Vector3& v2);
				static void		Negate(const Vector3 *const v, Vector3 *const result);
				static Vector3	Negate(const Vector3 *const v);
				static void		Normalize(const Vector3 *const v, Vector3 *const result);
				static Vector3	Normalize(const Vector3 *const v);
				static Vector3	Normalize(const Vector3 &v);

				static void		Lerp(const Vector3 *const v1, const Vector3 *const v2, const float ammount, Vector3 *const result);
				static void		Clamp(const Vector3 *const v, const Vector3 *const min, const Vector3 *const max, Vector3 *const result);
				static void		Clamp(const Vector3 *const v, const float min, const float max, Vector3 *const result);
				static void		Max(const Vector3 *const v1, const Vector3 *const v2, Vector3 *const result);
				static void		Min(const Vector3 *const v1, const Vector3 *const v2, Vector3 *const result);
				static Vector3	Max(const Vector3 *const v1, const Vector3 *const v2);
				static Vector3	Min(const Vector3 *const v1, const Vector3 *const v2);
				inline static Vector3 Max(const Vector3 &v1, const Vector3 &v2) { return Max(&v1, &v2); }
				inline static Vector3 Min(const Vector3 &v1, const Vector3 &v2) { return Min(&v1, &v2); }

				static Vector3	Min3(const Vector3 &v1,const Vector3 &v2,const Vector3 &v3);
				static Vector3	Max3(const Vector3 &v1,const Vector3 &v2,const Vector3 &v3);

				static void	    Abs(const Vector3 *const v, Vector3 *const result);
				static Vector3	Abs(const Vector3 *const v);
				inline static Vector3 Abs(const Vector3 &v) { return Abs(&v); }

				static void		Reflect(const Vector3 *const v, const Vector3 *const normal, Vector3 *const result);

				static void		Transform(const Vector3 *const position, const Matrix *const mat, Vector3 *const result);
				static void		Transform(const Vector2 *const position, const Matrix *const mat, Vector3 *const result);
				static void		TransformNoTranslation(const Vector3 *const position, const Matrix *const mat, Vector3 *const result);
				static Vector3	Transform(const Vector3 *const position, const Matrix *const mat);
				static Vector3	TransformNoTranslation(const Vector3 *const position, const Matrix *const mat);
				Vector3			Transform(const Matrix *const mat);

				static void     TransformPerspectiveDivide(const Vector3 *const position, const Matrix *const mat, Vector3 *const result);

				static void		TransformNormal(const Vector3 *const direction, const Matrix *const mat, Vector3 *const result);
				static void		TransformNormal(const Vector3 *const direction, const Quaternion *const quaternion, Vector3 *const result);
				static Vector3	TransformNormal(const Vector3 *const direction, const Matrix *const mat);
				static Vector3	TransformNormal(const Vector3 *const direction, const Quaternion *const quaternion);
				Vector3			TransformNormal(const Matrix *const mat);
				Vector3			TransformNormal(const Quaternion *const quaternion);

				static void		TransformInverseNormal(const Vector3 *const direction, const Matrix *const mat, Vector3 *const result);
				static void		TransformInverseNormal(const Vector3 *const direction, const Quaternion *const quaternion, Vector3 *const result);

				void Randomize(float rx = 1.0f, float ry = 1.0f, float rz = 1.0f);
				void RandomizeNNInt(float minx, float miny, float minz, float maxx, float maxy, float maxz);
				static Vector3 Random(float rx = 1.0f, float ry = 1.0f, float rz = 1.0f) { Vector3 r; r.Randomize(rx, ry, rz); return r; }
				static Vector3 RandomNNInt(float minx, float miny, float minz, float maxx, float maxy, float maxz) {Vector3 r; r.RandomizeNNInt(minx, miny, minz, maxx, maxy, maxz); return r;}
				static Vector3 Random(const Vector3* vec) {Vector3 r; r.Randomize(vec->x, vec->y, vec->z); return r;}

				static void RandomPointInPlane(const Vector3 &Org,const float Length,Vector3 &Result);

				void operator += (float val);
				void operator += (const Vector3 &v);
				void operator -= (float val);
				void operator -= (const Vector3 &v);
				void operator *= (float val);
				void operator *= (const Vector3 &v);
				void operator /= (float val);
				void operator /= (const Vector3 &v);
				Vector3& operator  = (const Vector3 &v);
				Vector3& operator  = (const Vector2 &v);
				bool operator  != (const Vector3 &v);
				bool operator  != (const Vector2 &v);
				bool operator  == (const Vector3 &v);

				inline void Set(const float x, const float y, const float z)
				{
					this->x = x;
					this->y = y;
					this->z = z;
				}

				inline void Set(const float v)
				{
					this->x = v;
					this->y = v;
					this->z = v;
				}

				inline void Load(const float *const v)
				{
					this->x = v[0];
					this->y = v[1];
					this->z = v[2];
				}

				inline void GetValues(float *v) const
				{
					v[0] = this->x;
					v[1] = this->y;
					v[2] = this->z;
				}

				Vector2 to2D(void) const;

				static void TransformOnlyZ(const Vector3 *const position, const Matrix *const mat, float* result);

				const float Get(const int axis) const;
				void Set(const float v,const int axis);

				static const float AngleBetweenVectors(const Vector3 &v1,const Vector3 &v2);
				static const float AngleBetweenNormalizedVectors(const Vector3 &v1,const Vector3 &v2);

				inline const bool AlmostEqual(const Vector3& other) const { return Distance2(this, &other) < Mathf::Epsilon; }
			};

			inline Vector3 operator + (const Vector3 &v1, const Vector3 &v2)	{ return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z); }
			inline Vector3 operator + (const Vector3 &v1, const float val)		{ return Vector3(v1.x + val, v1.y + val, v1.z + val); }
			inline Vector3 operator + (const float val, const Vector3 &v1)		{ return Vector3(v1.x + val, v1.y + val, v1.z + val); }
			inline Vector3 operator - (const Vector3 &v1, const Vector3 &v2)	{ return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z); }
			inline Vector3 operator - (const Vector3 &v1, const float val)		{ return Vector3(v1.x - val, v1.y - val, v1.z - val); }
			inline Vector3 operator - (const float val, const Vector3 &v1)		{ return Vector3(val - v1.x, val - v1.y, val - v1.z); }
			inline Vector3 operator * (const Vector3 &v1, const Vector3 &v2)	{ return Vector3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z); }
			inline Vector3 operator * (const Vector3 &v1, const float val)		{ return Vector3(v1.x * val, v1.y * val, v1.z * val); }
			inline Vector3 operator * (const float val, const Vector3 &v1)		{ return Vector3(v1.x * val, v1.y * val, v1.z * val); }
			inline Vector3 operator / (const Vector3 &v1, const Vector3 &v2)	{ return Vector3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z); }
			inline Vector3 operator / (const Vector3 &v1, const float val)		{ return Vector3(v1.x / val, v1.y / val, v1.z / val); }
			inline Vector3 operator / (const float val, const Vector3 &v1)		{ return Vector3(val / v1.x, val / v1.y, val / v1.z); }
			inline Vector3 operator - (const Vector3 &v)						{ return Vector3(-v.x, -v.y, -v.z); }
			inline float operator ^ (const Vector3 &v1, const Vector3 &v2)		{ return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z; }

		}
	}
}

#endif
