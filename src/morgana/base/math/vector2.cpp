#include "Vector2.h"
#include "Vector3.h"
#include "Matrix.h"
#include "memath.h"
#include <math.h>

using namespace MorganaEngine::Base::Math;

const Vector2 Vector2::Right	= Vector2(1.f, 0.f);
const Vector2 Vector2::Up		= Vector2(0.f, 1.f);
const Vector2 Vector2::Left		= Vector2(-1.f, 0.f);
const Vector2 Vector2::Down		= Vector2(0.f, -1.f);
const Vector2 Vector2::Zero		= Vector2(0.f, 0.f);
const Vector2 Vector2::One		= Vector2(1.f, 1.f);


Vector2::Vector2(const Vector3 &v)
{
	x = v.x;
	y = v.y;
}

void Vector2::Normalize()
{
	const float rlen = Mathf::Rsqrt(x*x + y*y);
	x *= rlen;
	y *= rlen;
}

void Vector2::Normalize(const Vector2 *const v, Vector2 *const result)
{
	const float rlen = Mathf::Rsqrt(v->x*v->x + v->y*v->y);
	result->x = v->x * rlen;
	result->y = v->y * rlen;
}

Vector2	Vector2::Normalize(const Vector2 *const v)
{
	const float rlen = Mathf::Rsqrt(v->x*v->x + v->y*v->y);
	return Vector2(v->x * rlen, v->y * rlen);
}

void  Vector2::Negate()
{ 
	x = -x;
	y = -y;
}

float Vector2::Length()			const	{ return Mathf::Sqrt(x*x + y*y); }
float Vector2::LengthReversed()	const	{ return Mathf::Rsqrt(x*x + y*y); }
float Vector2::LengthSquared()	const	{ return x*x + y*y; }
float Vector2::Angle()			const	{ return atan2f(-x, -y); }

void Vector2::Rotate(float angle) 
{
	float c = Mathf::Cos(angle);
	float s = Mathf::Sin(angle);
	float tempX = x, tempY = y;

	x = c * tempX - s * tempY;
	y = s * tempX + c * tempY;
}

float Vector2::Dot(const Vector2 *const v1, const Vector2 *const v2)
{
	return v1->x*v2->x + v1->y*v2->y;
}

float Vector2::DotNorm(const Vector2 *const v1, const Vector2 *const p1v2, const Vector2 *const p2v2)
{
    const float dx = p2v2->x - p1v2->x;
    const float dy = p2v2->y - p1v2->y;
    const float dot = v1->x * dx + v1->y * dy;
    return dot * Mathf::Rsqrt(dx*dx + dy*dy);
}

float Vector2::Distance2(const Vector2 *const v1, const Vector2 *const v2)
{
	const float dx = v1->x - v2->x;
	const float dy = v1->y - v2->y;
	return dx*dx + dy*dy;
}

float Vector2::Distance2(const Vector2& v1, const Vector2& v2)
{
	const float dx = v1.x - v2.x;
	const float dy = v1.y - v2.y;
	return dx*dx + dy*dy;
}

float Vector2::Distance(const Vector2 *const v1, const Vector2 *const v2)
{
	const float dx = v1->x - v2->x;
	const float dy = v1->y - v2->y;
	return Mathf::Sqrt(dx*dx + dy*dy);
}

float Vector2::Distance(const Vector2& v1, const Vector2& v2)
{
	const float dx = v1.x - v2.x;
	const float dy = v1.y - v2.y;
	return Mathf::Sqrt(dx*dx + dy*dy);
}

void Vector2::Add(const Vector2 *const v1, const Vector2 *const v2, Vector2 *const result)
{
	result->x = v1->x + v2->x;
	result->y = v1->y + v2->y;
}

void Vector2::Add(const Vector2 *const v, const float val, Vector2 *const result)
{
	result->x = v->x + val;
	result->y = v->y + val;
}

void Vector2::Subtract(const Vector2 *const v1, const Vector2 *const v2, Vector2 *const result)
{
	result->x = v1->x - v2->x;
	result->y = v1->y - v2->y;
}

void Vector2::Subtract(const Vector2 *const v, const float val, Vector2 *const result)
{
	result->x = v->x - val;
	result->y = v->y - val;
}

void Vector2::Divide(const Vector2 *const v1, const Vector2 *const v2, Vector2 *const result)
{
	result->x = v1->x / v2->x;
	result->y = v1->y / v2->y;
}

void Vector2::Divide(const Vector2 *const v, const float val, Vector2 *const result)
{
	result->x = v->x / val;
	result->y = v->y / val;
}

void Vector2::Multiply(const Vector2 *const v1, const Vector2 *const v2, Vector2 *const result)
{
	result->x = v1->x * v2->x;
	result->y = v1->y * v2->y;
}

void Vector2::Multiply(const Vector2 *const v, const float val, Vector2 *const result)
{
	result->x = v->x * val;
	result->y = v->y * val;
}

void Vector2::Negate(const Vector2 *const v, Vector2 *const result)
{
	result->x = -v->x;
	result->y = -v->y;
}

Vector2 Vector2::Add(const Vector2 *const v1, const Vector2 *const v2)
{
	return Vector2(v1->x + v2->x, v1->y + v2->y);
}

Vector2 Vector2::Add(const Vector2 *const v, const float val)
{
	return Vector2(v->x + val, v->y + val);
}

Vector2 Vector2::Subtract(const Vector2 *const v1, const Vector2 *const v2)
{
	return Vector2(v1->x - v2->x, v1->y - v2->y);
}

Vector2 Vector2::Subtract(const Vector2 *const v, const float val)
{
	return Vector2(v->x - val, v->y - val);
}

Vector2 Vector2::Divide(const Vector2 *const v1, const Vector2 *const v2)
{
	return Vector2(v1->x / v2->x, v1->y / v2->y);
}

Vector2 Vector2::Divide(const Vector2 *const v, const float val)
{
	return Vector2(v->x / val, v->y / val);
}

Vector2 Vector2::Multiply(const Vector2 *const v1, const Vector2 *const v2)
{
	return Vector2(v1->x * v2->x, v1->y * v2->y);
}

Vector2 Vector2::Multiply(const Vector2 *const v, const float val)
{
	return Vector2(v->x * val, v->y * val);
}

Vector2 Vector2::Multiply(const Vector2& v1, const Vector2& v2)
{
	return Vector2(v1.x * v2.x, v1.y * v2.y);
}

Vector2 Vector2::Negate(const Vector2 *const v)
{
	return Vector2(-v->x, -v->y);
}

void Vector2::Lerp(const Vector2 *const v1, const Vector2 *const v2, const float ammount, Vector2 *const result)
{
	if(ammount <= 0.f)
	{
		result->x = v1->x;
		result->y = v1->y;
	}
	else if(ammount >= 1.f)
	{
		result->x = v2->x;
		result->y = v2->y;
	}
	else
	{
		result->x = v1->x + (v2->x - v1->x) * ammount;
		result->y = v1->y + (v2->y - v1->y) * ammount;
	}
}

void Vector2::Clamp(const Vector2 *const v, const Vector2 *const min, const Vector2 *const max, Vector2 *const result)
{
	result->x = v->x > min->x ? (v->x < max->x ? v->x : max->x) : min->x;
	result->y = v->y > min->y ? (v->y < max->y ? v->y : max->y) : min->y;
}

void Vector2::Clamp(const Vector2 *const v, const float min, const float max, Vector2 *const result)
{
	result->x = v->x > min ? (v->x < max ? v->x : max) : min;
	result->y = v->y > min ? (v->y < max ? v->y : max) : min;
}

void Vector2::Max(const Vector2 *const v1, const Vector2 *const v2, Vector2 *const result)
{
	result->x = v1->x > v2->x ? v1->x : v2->x;
	result->y = v1->y > v2->y ? v1->y : v2->y;
}

void Vector2::Min(const Vector2 *const v1, const Vector2 *const v2, Vector2 *const result)
{
	result->x = v1->x > v2->x ? v2->x : v1->x;
	result->y = v1->y > v2->y ? v2->y : v1->y;
}

void Vector2::Reflect(const Vector2 *const v, const Vector2 *const normal, Vector2 *const result)
{
	const float dot2 = (v->x*normal->x + v->y*normal->y) * 2.f;
	result->x = v->x - normal->x*dot2;
	result->y = v->y - normal->y*dot2;
}

void Vector2::Transform(const Vector2 *const position, const Matrix *const mat, Vector2 *const result)
{
	result->x = mat->m11 * position->x + mat->m12 * position->y + mat->m14;
	result->y = mat->m21 * position->x + mat->m22 * position->y + mat->m24;
}

void Vector2::TransformPerspectiveDivide(const Vector2 *const position, const Matrix *const mat, Vector2 *const result)
{
	result->x = mat->m11 * position->x + mat->m12 * position->y + mat->m14;
	result->y = mat->m21 * position->x + mat->m22 * position->y + mat->m24;
    const float w = mat->m41 * position->x + mat->m42 * position->y + mat->m44;
    (*result) /= w;
}

void Vector2::TransformNormal(const Vector2 *const direction, const Matrix *const mat, Vector2 *const result)
{
	result->x = mat->m11 * direction->x + mat->m12 * direction->y;
	result->y = mat->m21 * direction->x + mat->m22 * direction->y;
}

Vector2& Vector2::operator  = (const Vector3 &v)
{
	x = v.x;
	y = v.y;
	return *this;
}

Vector2& Vector2::operator  = (const Vector2 &v)
{
	x = v.x;
	y = v.y;
	return *this;
}

Vector2& Vector2::operator  = (const float &v)
{
	x = v;
	y = v;
	return *this;
}

void Vector2::operator += (const Vector2 &v)
{
	x += v.x;
	y += v.y;
}

void Vector2::operator += (float val)
{
	x += val;
	y += val;
}

void Vector2::operator -= (const Vector2 &v)
{
	x -= v.x;
	y -= v.y;
}

void Vector2::operator -= (float val)
{
	x -= val;
	y -= val;
}

void Vector2::operator *= (const Vector2 &v)
{
	x *= v.x;
	y *= v.y;
}

void Vector2::operator *= (float val)
{
	x *= val;
	y *= val;
}

void Vector2::operator /= (const Vector2 &v)
{
	x /= v.x;
	y /= v.y;
}

void Vector2::operator /= (float val)
{
	x /= val;
	y /= val;
}

void Vector2::Randomize(float rx, float ry)
{
	x = Mathf::Rand11() * rx;
	y = Mathf::Rand11() * ry;
}

const bool Vector2::Equals(const Vector2& other, const float margin /*= 0.00000001f*/) const
{
	return Vector2::Distance2(this, &other) < margin * margin;
}

bool Vector2::operator!=(const Vector2& rhs) const
{
	return !Equals(rhs);
}

Vector2 MorganaEngine::Base::Math::operator+(const Vector2 &lhs, const Vector3 &rhs)
{
	return Vector2(lhs.x + rhs.x, lhs.y + rhs.y);
}

Vector2 MorganaEngine::Base::Math::operator+(const Vector3 &lhs, const Vector2 &rhs)
{
	return Vector2(lhs.x + rhs.x, lhs.y + rhs.y);
}
