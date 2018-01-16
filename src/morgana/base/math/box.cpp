#include "box.h"
#include <math.h>
#include "memath.h"

using namespace MorganaEngine::Base::Math;

Box Box::Zero;
Box Box::Invalid(-1.0f);

bool Box::Contains(const Vector3 *const pos) const
{
    const Vector3 adiff(fabsf(pos->x - center.x),
                        fabsf(pos->y - center.y),
                        fabsf(pos->z - center.z));
    
    return  adiff.x < offset.x &&
            adiff.y < offset.y &&
            adiff.z < offset.z;
}

bool Box::Contains(const Vector3 *const pos, const float radius) const
{
    const Vector3 adiff(fabsf(pos->x - center.x),
                        fabsf(pos->y - center.y),
                        fabsf(pos->z - center.z));
    
    return  adiff.x < (offset.x + radius) ||
            adiff.y < (offset.y + radius) ||
            adiff.z < (offset.z + radius);
}

Box Box::FromMinMax(const Vector3 &minBB,const Vector3 &maxBB)
{
	Box b;
    b.center = (minBB + maxBB) * 0.5f;
	b.offset = maxBB - b.center;

	return b;
}

Box Box::FromPosSize(const Vector3 &pos,const float size)
{
	Box b;
    b.center = pos;
	b.offset = Vector3(size, size, size);

	return b;
}

Vector3 Box::RandomPoint(void) const
{
    Vector3 v1 = Vector3(Mathf::Rand11(), Mathf::Rand11(), Mathf::Rand11());
    return center + v1 * offset;
}

void Box::Combine(const Box &bb1,const Box &bb2,Box &bbout)
{
	const Vector3 min1 = bb1.center - bb1.offset;
	const Vector3 max1 = bb1.center + bb1.offset;
	const Vector3 min2 = bb2.center - bb2.offset;
	const Vector3 max2 = bb2.center + bb2.offset;
    bbout = FromMinMax(Vector3::Min(min1,min2),Vector3::Max(max1,max2));
}

void Box::CombinePrecise(const Box &bb1,const Box &bb2,Box &bbout)
{
	static Vector3 c[16];
	((Box)bb1).GetCorners(c);
	((Box)bb2).GetCorners(c + 8);

	Vector3 max = -Vector3::MaxFloat;
	Vector3 min = -max;

	for(Vector3 *ptr = c, *end = ptr + 16; ptr < end; ptr++)
	{
		Vector3::Min(ptr, &min, &min);
		Vector3::Max(ptr, &max, &max);
	}

	bbout = FromMinMax(min, max);
}


void Box::GetCorners( Vector3* c )
{
	Vector3 vMin = center - offset;
	Vector3 vMax = center + offset;
	c[0] = Vector3(vMin.x, vMax.y, vMax.z);
	c[1] = Vector3(vMin.x, vMax.y, vMin.z);
	c[2] = Vector3(vMax.x, vMax.y, vMin.z);
	c[3] = Vector3(vMax.x, vMax.y, vMax.z);

	c[4] = Vector3(vMax.x, vMin.y, vMax.z);
	c[5] = Vector3(vMax.x, vMin.y, vMin.z);
	c[6] = Vector3(vMin.x, vMin.y, vMin.z);
	c[7] = Vector3(vMin.x, vMin.y, vMax.z);

}

void Box::Transform( const Matrix* m )
{
	Vector3 c[8];
	GetCorners(c);

	Vector3 max = -Vector3::One * Mathf::MaxFloat;
	Vector3 min = -max;

	for(int i = 0; i < 8; i++)				
	{
		Vector3 p;
		Vector3::Transform(&c[i], m, &p);
		Vector3::Min(&p, &min, &min);
		Vector3::Max(&p, &max, &max);
	}

	center = (max+min) * 0.5f;
	offset = (max-min) * 0.5f;

}

bool Box::Intersects(const Box &bb1,const Box &bb2)
{
	const Vector3 min1 = bb1.center - bb1.offset;
	const Vector3 max1 = bb1.center + bb1.offset;
	const Vector3 min2 = bb2.center - bb2.offset;
	const Vector3 max2 = bb2.center + bb2.offset;
    if ((max1.x < min2.x) || (max2.x < min1.x))
    {
        return false;
    }
    if ((max1.y < min2.y) || (max2.y < min1.y))
    {
        return false;
    }
	if ((max1.z < min2.z) || (max2.z < min1.z))
	{
		return false;
	}
    return true;
}

void Box::Transform(const Box *const aabb, const Matrix *const mat, Box *const result)
{
    Vector3 vMin, vMax, tmp, ttmp, min_, max_;
    
    Vector3::Subtract(&aabb->center, &aabb->offset, &vMin);
    Vector3::Add(&aabb->center, &aabb->offset, &vMax);
    
    tmp.Set(vMin.x, vMax.y, vMax.z);
    Vector3::Transform(&tmp, mat, &ttmp);
    min_ = ttmp;
    max_ = ttmp;
    
    tmp.z = vMin.z;//tmp.Set(vMin.x, vMax.y, vMin.z);
    Vector3::Transform(&tmp, mat, &ttmp);
    Vector3::Min(&ttmp, &min_, &min_);
    Vector3::Max(&ttmp, &max_, &max_);
    
    tmp.x = vMax.x;//tmp.Set(vMax.x, vMax.y, vMin.z);
    Vector3::Transform(&tmp, mat, &ttmp);
    Vector3::Min(&ttmp, &min_, &min_);
    Vector3::Max(&ttmp, &max_, &max_);
    
    tmp.z = vMax.z;//tmp.Set(vMax.x, vMax.y, vMax.z);
    Vector3::Transform(&tmp, mat, &ttmp);
    Vector3::Min(&ttmp, &min_, &min_);
    Vector3::Max(&ttmp, &max_, &max_);
    
    tmp.y = vMin.y;//tmp.Set(vMax.x, vMin.y, vMax.z);
    Vector3::Transform(&tmp, mat, &ttmp);
    Vector3::Min(&ttmp, &min_, &min_);
    Vector3::Max(&ttmp, &max_, &max_);
    
    tmp.z = vMin.z;//tmp.Set(vMax.x, vMin.y, vMin.z);
    Vector3::Transform(&tmp, mat, &ttmp);
    Vector3::Min(&ttmp, &min_, &min_);
    Vector3::Max(&ttmp, &max_, &max_);
    
    tmp.x = vMin.x;//tmp.Set(vMin.x, vMin.y, vMin.z);
    Vector3::Transform(&tmp, mat, &ttmp);
    Vector3::Min(&ttmp, &min_, &min_);
    Vector3::Max(&ttmp, &max_, &max_);
    
    tmp.z = vMax.z;//tmp.Set(vMin.x, vMin.y, vMax.z);
    Vector3::Transform(&tmp, mat, &ttmp);
    Vector3::Min(&ttmp, &min_, &min_);
    Vector3::Max(&ttmp, &max_, &max_);
    
    Vector3::Add(&max_, &min_, &result->center);
    Vector3::Subtract(&max_, &min_, &result->offset);
    result->center *= 0.5f;
    result->offset *= 0.5f;
}

void Box::TransformBox(const Box &inputBB,const Matrix &mat,Box &outBB)
{
    Vector3 bbCenter = inputBB.center;
    Vector3 trbbCenter;
    Vector3::Transform(&bbCenter,&mat,&trbbCenter);

    Matrix rotMat = mat;
    rotMat.Translation() = Vector3::Zero;
    rotMat.Right() = Vector3::Abs(rotMat.Right());
    rotMat.Up() = Vector3::Abs(rotMat.Up());
    rotMat.Backward() = Vector3::Abs(rotMat.Backward());

    Vector3 bbOffset = inputBB.offset;
    Vector3 trbbOffset;
    Vector3::Transform(&bbOffset,&rotMat,&trbbOffset);

    outBB = Box(trbbCenter,trbbOffset);
}

const float Box::Distance(const Box& other)
{
	return Distance(*this, other);
}

const float Box::Distance(const Box& b1, const Box& b2)
{
	const float d2 = Distance2(b1, b2);
	return d2 > 0.0f ? Mathf::Sqrt(d2) : 0.0f;
}

const float Box::Distance2(const Box& b1, const Box& b2)
{
	const Vector3 min1 = b1.center - b1.offset;
	const Vector3 max1 = b1.center + b1.offset;

	const Vector3 min2 = b2.center - b2.offset;
	const Vector3 max2 = b2.center + b2.offset;

	const Vector3 d1 = max2 - min1;
	const Vector3 d2 = max1 - min2;

	float sqrDist = 0.0f;

	if (d1.x < 0.0f)
		sqrDist += d1.x * d1.x;
	if (d1.y < 0.0f)
		sqrDist += d1.y * d1.y;
	if (d1.z < 0.0f)
		sqrDist += d1.z * d1.z;

	if (d2.x < 0.0f)
		sqrDist += d2.x * d2.x;
	if (d2.y < 0.0f)
		sqrDist += d2.y * d2.y;
	if (d2.z < 0.0f)
		sqrDist += d2.z * d2.z;

	return sqrDist;
}
