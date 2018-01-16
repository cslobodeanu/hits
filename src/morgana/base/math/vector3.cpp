#include "Vector3.h"
#include "Vector2.h"
#include "memath.h"
#include <math.h>
#include "meio.h"

using namespace MorganaEngine::Base::Math;

const Vector3 Vector3::Left		= Vector3(-1.f, 0.f, 0.f);
const Vector3 Vector3::Right	= Vector3(+1.f, 0.f, 0.f);
const Vector3 Vector3::Down		= Vector3(0.f, -1.f, 0.f);
const Vector3 Vector3::Up		= Vector3(0.f, +1.f, 0.f);
const Vector3 Vector3::Forward	= Vector3(0.f, 0.f, -1.f);
const Vector3 Vector3::Backward	= Vector3(0.f, 0.f, +1.f);

const Vector3 Vector3::UnitX	= Vector3(1.f, 0.f, 0.f);
const Vector3 Vector3::UnitY	= Vector3(0.f, 1.f, 0.f);
const Vector3 Vector3::UnitZ	= Vector3(0.f, 0.f, 1.f);
Vector3 Vector3::Zero		= Vector3(0.f, 0.f, 0.f);
const Vector3 Vector3::One		= Vector3(1.f, 1.f, 1.f);

const Vector3 Vector3::MinFloat	= Vector3(Mathf::MinFloat, Mathf::MinFloat, Mathf::MinFloat);
const Vector3 Vector3::MaxFloat	= Vector3(Mathf::MaxFloat, Mathf::MaxFloat, Mathf::MaxFloat);

const Vector3 Vector3::Luma     = Vector3(0.299f, 0.587f, 0.114f);

const Vector3 Vector3::DiagonalXY = Vector3(1.0f, 1.0f, 0.0f);
const Vector3 Vector3::DiagonalXZ = Vector3(1.0f, 0.0f, 1.0f);
const Vector3 Vector3::DiagonalYZ = Vector3(0.0f, 1.0f, 1.0f);
const Vector3 Vector3::DiagonalXYZ = Vector3(1.0f, 1.0f, 1.0f);

const Vector4 Vector4::Zero = Vector4(0.f, 0.f, 0.f, 0.f);
const Vector4 Vector4::One = Vector4(1.f, 1.f, 1.f, 1.f);

const Vector3 Vector3::FlipX = Vector3(-1.0f, 1.0f, 1.0f);
const Vector3 Vector3::FlipY = Vector3(1.0f, -1.0f, 1.0f);
const Vector3 Vector3::FlipZ = Vector3(1.0f, 1.0f, -1.0f);

void Vector3::Normalize()
{
	const float rlen = Mathf::Rsqrt(x*x + y*y + z*z);
	x *= rlen;
	y *= rlen;
	z *= rlen;
}

void Vector3::Normalize(const Vector3 *const v, Vector3 *const result)
{
	const float rlen = Mathf::Rsqrt(v->x*v->x + v->y*v->y + v->z*v->z);
	result->x = v->x * rlen;
	result->y = v->y * rlen;
	result->z = v->z * rlen;
}

Vector3	Vector3::Normalize(const Vector3 *const v)
{
	const float rlen = Mathf::Rsqrt(v->x*v->x + v->y*v->y + v->z*v->z);
	return Vector3(v->x * rlen, v->y * rlen, v->z * rlen);
}

Vector3	Vector3::Normalize(const Vector3 &v)
{
	return Normalize(&v);
}

void  Vector3::Negate()
{ 
	x = -x;
	y = -y;
	z = -z;
}

float Vector3::Length()			const	{ return Mathf::Sqrt(x*x + y*y + z*z); }
float Vector3::LengthReversed()	const	{ return Mathf::Rsqrt(x*x + y*y + z*z); }
float Vector3::LengthSquared()	const	{ return x*x + y*y + z*z; }
float Vector3::LengthXZ()	    const	{ return Mathf::Sqrt(x*x + z*z); }
float Vector3::LengthXZSquared()const	{ return x*x + z*z; }
float Vector3::Yaw()			const	{ return atan2f(-x, -z); }
float Vector3::Pitch()			const	{ return asinf(y); }

float Vector3::Dot(const Vector3 *const v1, const Vector3 *const v2)
{
	return v1->x*v2->x + v1->y*v2->y + v1->z*v2->z;
}

float Vector3::DotNorm(const Vector3 *const v1, const Vector3 *const p1v2, const Vector3 *const p2v2)
{
	const float dx = p2v2->x - p1v2->x;
	const float dy = p2v2->y - p1v2->y;
	const float dz = p2v2->z - p1v2->z;
	const float dot = v1->x * dx + v1->y * dy + v1->z * dz;
	return dot * Mathf::Rsqrt(dx*dx + dy*dy + dz*dz);
}

void Vector3::Cross(const Vector3 *const v1, const Vector3 * const v2, Vector3 * const result)
{
	result->x = v1->y*v2->z - v1->z*v2->y;
	result->y = v1->z*v2->x - v1->x*v2->z;
	result->z = v1->x*v2->y - v1->y*v2->x;
}

Vector3	Vector3::Cross(const Vector3 *const v1, const Vector3 *const v2)
{
	return Vector3(
		v1->y*v2->z - v1->z*v2->y,
		v1->z*v2->x - v1->x*v2->z,
		v1->x*v2->y - v1->y*v2->x);
}

void Vector3::Add(const Vector3 *const v1, const Vector3 *const v2, Vector3 *const result)
{
	result->x = v1->x + v2->x;
	result->y = v1->y + v2->y;
	result->z = v1->z + v2->z;
}

void Vector3::Add(const Vector3 *const v, const float val, Vector3 *const result)
{
	result->x = v->x + val;
	result->y = v->y + val;
	result->z = v->z + val;
}

void Vector3::Subtract(const Vector3 *const v1, const Vector3 *const v2, Vector3 *const result)
{
	result->x = v1->x - v2->x;
	result->y = v1->y - v2->y;
	result->z = v1->z - v2->z;
}

void Vector3::Subtract(const Vector3 *const v, const float val, Vector3 *const result)
{
	result->x = v->x - val;
	result->y = v->y - val;
	result->z = v->z - val;
}

void Vector3::Divide(const Vector3 *const v1, const Vector3 *const v2, Vector3 *const result)
{
	result->x = v1->x / v2->x;
	result->y = v1->y / v2->y;
	result->z = v1->z / v2->z;
}

void Vector3::Divide(const Vector3 *const v, const float val, Vector3 *const result)
{
	result->x = v->x / val;
	result->y = v->y / val;
	result->z = v->z / val;
}

void Vector3::Multiply(const Vector3 *const v1, const Vector3 *const v2, Vector3 *const result)
{
	result->x = v1->x * v2->x;
	result->y = v1->y * v2->y;
	result->z = v1->z * v2->z;
}

void Vector3::Multiply(const Vector3 *const v, const float val, Vector3 *const result)
{
	result->x = v->x * val;
	result->y = v->y * val;
	result->z = v->z * val;
}

Vector3	Vector3::Multiply(const Vector3& v1, const Vector3& v2)
{
	return Vector3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

void Vector3::Negate(const Vector3 *const v, Vector3 *const result)
{
	result->x = -v->x;
	result->y = -v->y;
	result->z = -v->z;
}

float Vector3::Distance2(const Vector3 *const v1, const Vector3 *const v2)
{
	const float dx = v1->x - v2->x;
	const float dy = v1->y - v2->y;
	const float dz = v1->z - v2->z;
	return dx*dx + dy*dy + dz*dz;
}

float Vector3::Distance(const Vector3 *const v1, const Vector3 *const v2)
{
	const float dx = v1->x - v2->x;
	const float dy = v1->y - v2->y;
	const float dz = v1->z - v2->z;
	return Mathf::Sqrt(dx*dx + dy*dy + dz*dz);
}

float Vector3::Distance2(const Vector3 & v1, const Vector3 & v2)
{
	const float dx = v1.x - v2.x;
	const float dy = v1.y - v2.y;
	const float dz = v1.z - v2.z;
	return dx*dx + dy*dy + dz*dz;
}

float Vector3::Distance(const Vector3 & v1, const Vector3 & v2)
{
	const float dx = v1.x - v2.x;
	const float dy = v1.y - v2.y;
	const float dz = v1.z - v2.z;
	return Mathf::Sqrt(dx*dx + dy*dy + dz*dz);
}

float Vector3::DistanceXZ(const Vector3 *const v1, const Vector3 *const v2)
{
	const float dx = v1->x - v2->x;
	const float dz = v1->z - v2->z;
	return Mathf::Sqrt(dx*dx + dz*dz);
}

float Vector3::Distance2XZ(const Vector3 *const v1, const Vector3 *const v2)
{
	const float dx = v1->x - v2->x;
	const float dz = v1->z - v2->z;
	return dx*dx + dz*dz;
}

Vector3 Vector3::Add(const Vector3 *const v1, const Vector3 *const v2)
{
	return Vector3(v1->x + v2->x, v1->y + v2->y, v1->z + v2->z);
}

Vector3 Vector3::Add(const Vector3 *const v, const float val)
{
	return Vector3(v->x + val, v->y + val, v->z + val);
}

Vector3 Vector3::Subtract(const Vector3 *const v1, const Vector3 *const v2)
{
	return Vector3(v1->x - v2->x, v1->y - v2->y, v1->z - v2->z);
}

Vector3 Vector3::Subtract(const Vector3 *const v, const float val)
{
	return Vector3(v->x - val, v->y - val, v->z - val);
}

Vector3 Vector3::Divide(const Vector3 *const v1, const Vector3 *const v2)
{
	return Vector3(v1->x / v2->x, v1->y / v2->y, v1->z / v2->z);
}

Vector3 Vector3::Divide(const Vector3 *const v, const float val)
{
	return Vector3(v->x / val, v->y / val, v->z / val);
}

Vector3 Vector3::Multiply(const Vector3 *const v1, const Vector3 *const v2)
{
	return Vector3(v1->x * v2->x, v1->y * v2->y, v1->z * v2->z);
}

Vector3 Vector3::Multiply(const Vector3 *const v, const float val)
{
	return Vector3(v->x * val, v->y * val, v->z * val);
}

Vector3 Vector3::Negate(const Vector3 *const v)
{
	return Vector3(-v->x, -v->y, -v->z);
}


void Vector3::Lerp(const Vector3 *const v1, const Vector3 *const v2, const float ammount, Vector3 *const result)
{
	if(ammount <= 0.f)
	{
		result->x = v1->x;
		result->y = v1->y;
		result->z = v1->z;
	}
	else if(ammount >= 1.f)
	{
		result->x = v2->x;
		result->y = v2->y;
		result->z = v2->z;
	}
	else
	{
		result->x = v1->x + (v2->x - v1->x) * ammount;
		result->y = v1->y + (v2->y - v1->y) * ammount;
		result->z = v1->z + (v2->z - v1->z) * ammount;
	}
}

void Vector3::Clamp(const Vector3 *const v, const Vector3 *const min, const Vector3 *const max, Vector3 *const result)
{
	result->x = v->x > min->x ? (v->x < max->x ? v->x : max->x) : min->x;
	result->y = v->y > min->y ? (v->y < max->y ? v->y : max->y) : min->y;
	result->z = v->z > min->z ? (v->z < max->z ? v->z : max->z) : min->z;
}

void Vector3::Clamp(const Vector3 *const v, const float min, const float max, Vector3 *const result)
{
	result->x = v->x > min ? (v->x < max ? v->x : max) : min;
	result->y = v->y > min ? (v->y < max ? v->y : max) : min;
	result->z = v->z > min ? (v->z < max ? v->z : max) : min;
}

void Vector3::Max(const Vector3 *const v1, const Vector3 *const v2, Vector3 *const result)
{
	result->x = v1->x > v2->x ? v1->x : v2->x;
	result->y = v1->y > v2->y ? v1->y : v2->y;
	result->z = v1->z > v2->z ? v1->z : v2->z;
}

float Vector3::Max() const { return Mathf::Max(Mathf::Max(x, y), z); }
float Vector3::Min() const { return Mathf::Min(Mathf::Min(x, y), z); }

void Vector3::Min(const Vector3 *const v1, const Vector3 *const v2, Vector3 *const result)
{
	result->x = v1->x > v2->x ? v2->x : v1->x;
	result->y = v1->y > v2->y ? v2->y : v1->y;
	result->z = v1->z > v2->z ? v2->z : v1->z;
}


Vector3 Vector3::Max( const Vector3 *const v1, const Vector3 *const v2 )
{
	Vector3 ret;
	Max(v1,v2,&ret);
	return ret;
}

Vector3 Vector3::Min( const Vector3 *const v1, const Vector3 *const v2 )
{
	Vector3 ret;
	Min(v1,v2,&ret);
	return ret;
}

Vector3	Vector3::Min3(const Vector3 &v1,const Vector3 &v2,const Vector3 &v3)
{
	return Vector3((v1.x < v2.x) ? ((v1.x < v3.x) ? v1.x : v3.x) : ((v2.x < v3.x) ? v2.x : v3.x),
		(v1.y < v2.y) ? ((v1.y < v3.y) ? v1.y : v3.y) : ((v2.y < v3.y) ? v2.y : v3.y),
		(v1.z < v2.z) ? ((v1.z < v3.z) ? v1.z : v3.z) : ((v2.z < v3.z) ? v2.z : v3.z));
}

Vector3	Vector3::Max3(const Vector3 &v1,const Vector3 &v2,const Vector3 &v3)
{
	return Vector3((v1.x > v2.x) ? ((v1.x > v3.x) ? v1.x : v3.x) : ((v2.x > v3.x) ? v2.x : v3.x),
		(v1.y > v2.y) ? ((v1.y > v3.y) ? v1.y : v3.y) : ((v2.y > v3.y) ? v2.y : v3.y),
		(v1.z > v2.z) ? ((v1.z > v3.z) ? v1.z : v3.z) : ((v2.z > v3.z) ? v2.z : v3.z));
}

void Vector3::Abs(const Vector3 *const v, Vector3 *const result)
{
	result->x = Mathf::Abs(v->x);
	result->y = Mathf::Abs(v->y);
	result->z = Mathf::Abs(v->z);
}

Vector3 Vector3::Abs(const Vector3 *const v)
{
	Vector3 x;
	Abs(v,&x);
	return x;
}

void Vector3::Reflect(const Vector3 *const v, const Vector3 *const normal, Vector3 *const result)
{
	const float dot2 = (v->x*normal->x + v->y*normal->y + v->z*normal->z) * 2.f;
	result->x = v->x - normal->x*dot2;
	result->y = v->y - normal->y*dot2;
	result->z = v->z - normal->z*dot2;
}

void Vector3::Transform(const Vector3 *const position, const Matrix *const mat, Vector3 *const result)
{
	/*if (memcmp(mat->Ptr(), Matrix::Identity.Ptr(), 16 * sizeof(float)) == 0)
	{
		*result = *position;
		return;
	}*/

	result->x = mat->m11 * position->x + mat->m12 * position->y + mat->m13 * position->z + mat->m14;
	result->y = mat->m21 * position->x + mat->m22 * position->y + mat->m23 * position->z + mat->m24;
	result->z = mat->m31 * position->x + mat->m32 * position->y + mat->m33 * position->z + mat->m34;
}

void Vector3::Transform(const Vector2 *const position, const Matrix *const mat, Vector3 *const result)
{
	/*if (memcmp(mat->Ptr(), Matrix::Identity.Ptr(), 16 * sizeof(float)) == 0)
	{
	*result = *position;
	return;
	}*/

	result->x = mat->m11 * position->x + mat->m12 * position->y + mat->m14;
	result->y = mat->m21 * position->x + mat->m22 * position->y + mat->m24;
	result->z = mat->m31 * position->x + mat->m32 * position->y + mat->m34;
}

void Vector3::TransformNoTranslation(const Vector3 *const position, const Matrix *const mat, Vector3 *const result)
{
	/*if (memcmp(mat->Ptr(), Matrix::Identity.Ptr(), 16 * sizeof(float)) == 0)
	{
	*result = *position;
	return;
	}*/

	result->x = mat->m11 * position->x + mat->m12 * position->y + mat->m13 * position->z;
	result->y = mat->m21 * position->x + mat->m22 * position->y + mat->m23 * position->z;
	result->z = mat->m31 * position->x + mat->m32 * position->y + mat->m33 * position->z;
}

Vector3	Vector3::Transform(const Vector3 *const position, const Matrix *const mat)
{
	Vector3 result;
	Transform(position, mat, &result);
	return result;
}

Vector3	Vector3::TransformNoTranslation(const Vector3 *const position, const Matrix *const mat)
{
	Vector3 result;
	TransformNoTranslation(position, mat, &result);
	return result;
}

Vector3 Vector3::Transform(const Matrix *const mat)
{
	return Transform(this, mat);
}

void Vector3::TransformPerspectiveDivide(const Vector3 *const position, const Matrix *const mat, Vector3 *const result)
{
	result->x = mat->m11 * position->x + mat->m12 * position->y + mat->m13 * position->z + mat->m14;
	result->y = mat->m21 * position->x + mat->m22 * position->y + mat->m23 * position->z + mat->m24;
	result->z = mat->m31 * position->x + mat->m32 * position->y + mat->m33 * position->z + mat->m34;
	const float w = mat->m41 * position->x + mat->m42 * position->y + mat->m43 * position->z + mat->m44;
	(*result) /= w;
}

void Vector3::TransformNormal(const Vector3 *const direction, const Matrix *const mat, Vector3 *const result)
{
	result->x = mat->m11 * direction->x + mat->m12 * direction->y + mat->m13 * direction->z;
	result->y = mat->m21 * direction->x + mat->m22 * direction->y + mat->m23 * direction->z;
	result->z = mat->m31 * direction->x + mat->m32 * direction->y + mat->m33 * direction->z;
}

void Vector3::TransformNormal(const Vector3 *const direction, const Quaternion *const quaternion, Vector3 *const result)
{		
	const float tx = quaternion->w*direction->x + quaternion->y*direction->z - quaternion->z*direction->y;
	const float ty = quaternion->w*direction->y - quaternion->x*direction->z + quaternion->z*direction->x;
	const float tz = quaternion->w*direction->z + quaternion->x*direction->y - quaternion->y*direction->x;
	const float tw = -quaternion->x*direction->x - quaternion->y*direction->y - quaternion->z*direction->z;

	result->x = -tw*quaternion->x + tx*quaternion->w - ty*quaternion->z + tz*quaternion->y;
	result->y = -tw*quaternion->y + ty*quaternion->w - tz*quaternion->x + tx*quaternion->z;
	result->z = -tw*quaternion->z + tz*quaternion->w - tx*quaternion->y + ty*quaternion->x;
}

Vector3	Vector3::TransformNormal(const Vector3 *const direction, const Matrix *const mat)
{
	Vector3 result;
	TransformNormal(direction, mat, &result);
	return result;
}

Vector3	Vector3::TransformNormal(const Vector3 *const direction, const Quaternion *const quaternion)
{
	Vector3 result;
	TransformNormal(direction, quaternion, &result);
	return result;
}

Vector3	Vector3::TransformNormal(const Matrix *const mat)
{
	Vector3 result;
	TransformNormal(this, mat, &result);
	return result;
}

Vector3	Vector3::TransformNormal(const Quaternion *const quaternion)
{
	Vector3 result;
	TransformNormal(this, quaternion, &result);
	return result;
}

void Vector3::TransformInverseNormal(const Vector3 *const direction, const Matrix *const mat, Vector3 *const result)
{
	result->x = mat->m11 * direction->x + mat->m21 * direction->y + mat->m31 * direction->z;
	result->y = mat->m12 * direction->x + mat->m22 * direction->y + mat->m32 * direction->z;
	result->z = mat->m13 * direction->x + mat->m23 * direction->y + mat->m33 * direction->z;
}

void Vector3::TransformInverseNormal(const Vector3 *const direction, const Quaternion *const quaternion, Vector3 *const result)
{
	const float tx = quaternion->w*direction->x - quaternion->y*direction->z + quaternion->z*direction->y;
	const float ty = quaternion->w*direction->y + quaternion->x*direction->z - quaternion->z*direction->x;
	const float tz = quaternion->w*direction->z - quaternion->x*direction->y + quaternion->y*direction->x;
	const float tw = quaternion->x*direction->x + quaternion->y*direction->y + quaternion->z*direction->z;

	result->x = tw*quaternion->x + tx*quaternion->w + ty*quaternion->z - tz*quaternion->y;
	result->y = tw*quaternion->y + ty*quaternion->w + tz*quaternion->x - tx*quaternion->z;
	result->z = tw*quaternion->z + tz*quaternion->w + tx*quaternion->y - ty*quaternion->x;
}

void Vector3::operator += (const Vector3 &v)
{
	x += v.x;
	y += v.y;
	z += v.z;
}

void Vector3::operator += (float val)
{
	x += val;
	y += val;
	z += val;
}

void Vector3::operator -= (const Vector3 &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

void Vector3::operator -= (float val)
{
	x -= val;
	y -= val;
	z -= val;
}

void Vector3::operator *= (const Vector3 &v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
}

void Vector3::operator *= (float val)
{
	x *= val;
	y *= val;
	z *= val;
}

void Vector3::operator /= (const Vector3 &v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
}

void Vector3::operator /= (float val)
{
	x /= val;
	y /= val;
	z /= val;
}

Vector3& Vector3::operator  = (const Vector3 &v)
{
	//memcpy(&x, &v.x, 3 * sizeof(float));
	 	x = v.x;
	 	y = v.y;
	 	z = v.z;
	return *this;
}

Vector3& Vector3::operator  = (const Vector2 &v)
{
	//memcpy(&x, &v.x, 2 * sizeof(float));
	z = 0.0f;
 	x = v.x;
 	y = v.y;
	return *this;
}

bool Vector3::operator  != (const Vector3 &v)
{
	return (x != v.x ||
		y != v.y ||
		z != v.z );
}

bool Vector3::operator  == (const Vector3 &v)
{
	return memcmp(&x, &v.x, 3 * sizeof(float)) == 0;
}

bool Vector3::operator  != (const Vector2 &v)
{
	return memcmp(&x, &v.x, 3 * sizeof(float)) != 0;
}

void Vector3::Randomize(float rx, float ry, float rz)
{
	x = Mathf::Rand11() * rx;
	y = Mathf::Rand11() * ry;
	z = Mathf::Rand11() * rz;
}

void Vector3::RandomizeNNInt(float minx, float miny, float minz, float maxx, float maxy, float maxz)
{
	x = Mathf::RandNNInt(minx, maxx);
	y = Mathf::RandNNInt(miny, maxy);
	z = Mathf::RandNNInt(minz, maxz);
}

void Vector3::RandomPointInPlane(const Vector3 &Org,const float Length,Vector3 &Result)
{
	float phi = Mathf::RandAngle();
	Vector3 v;
	v.Set(sin(phi),0.0f,cos(phi));
	v *= Mathf::Rand(Length);
	Result = Org + v;
}

Vector2 Vector3::to2D(void) const
{
	return Vector2(z,x);
}

void Vector3::TransformOnlyZ(const Vector3 *const position, const Matrix *const mat, float* result)
{
	*result = mat->m31 * position->x + mat->m32 * position->y + mat->m33 * position->z + mat->m34;
}

const float Vector3::Get(const int axis) const
{
	ASSERT(axis >= 0 && axis <= 2);
	return (&x)[axis];
	return 0;
}

void Vector3::Set(const float v,const int axis)
{
	ASSERT(axis >= 0 && axis <= 2);
	(&x)[axis] = v;
}

const float Vector3::AngleBetweenVectors(const Vector3 &v1,const Vector3 &v2)
{
	Vector3 vv1 = Vector3::Normalize(v1);
	Vector3 vv2 = Vector3::Normalize(v2);

	return AngleBetweenNormalizedVectors(vv1,vv2);
}

const float Vector3::AngleBetweenNormalizedVectors(const Vector3 &v1,const Vector3 &v2)
{
	return Mathf::Acos(Mathf::Clamp(Vector3::Dot(v1,v2),-1.0f,1.0f));
}
