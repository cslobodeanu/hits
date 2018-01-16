#include "curve.h"
#include "spline.h"

using namespace MorganaEngine::Base::Math;
using namespace MorganaEngine::Base::Types;

Curve::Curve(const int res /*= 256*/)
{
	points.SetLength(res);
	ZeroPoints();
}

Curve::~Curve()
{

}

void Curve::SetPoint(const float t, const float y)
{
	const Vector2 p = Vector2(t, y);
	if (controlPoints.IsEmpty())
	{
		controlPoints.Add(p);
		return;
	}

	if (t < controlPoints.First().x)
	{
		controlPoints.Insert(0, p);
		Regenerate();
		return;
	}

	if (t > controlPoints.Last().x)
	{
		controlPoints.Add(p);
		Regenerate();
		return;
	}


	for (int i = 0; i < controlPoints.Length(); i++)
	{
		if (t > controlPoints[i].x)
		{
			controlPoints.Insert(i + 1, p);
			Regenerate();
			return;
		}
	}
}

const float Curve::GetPoint(const float t)
{
	const int res = points.Length();
	const float indexF = t * (float)(res - 1);
	const int index = (int)Mathf::Floor(indexF);
	if (index == res - 1)
		return points.Last();
	if (index == 0)
		return points.First();

	return Mathf::Lerp(points[index], points[(int)indexF + 1], indexF - (float)index);
}

Array<Vector2> Curve::GetControlPoints()
{
	return controlPoints;
}

void Curve::Regenerate()
{
	if (controlPoints.IsEmpty())
	{
		ZeroPoints();
		return;
	}

	if (controlPoints.Length() == 1)
	{
		for (float* v = points.ptr(); v < points.end(); v++)
			*v = controlPoints.First().y;
		return;
	}

	if (controlPoints.Length() == 2)
	{
		const Vector2& c1 = controlPoints[0];
		const Vector2& c2 = controlPoints[1];
		for (int i = 0; i < controlPoints.Length(); i++)
		{
			const float t = (float)i / (controlPoints.Length() - 1);
			points[i] = Mathf::Lerp(c1.y, c2.y, t);
		}
		return;
	}

	Spline sp;
	for (Vector2 *v = controlPoints.ptr(); v < controlPoints.end(); v++)
		sp.AddPoint(*v, false);
	sp.RegenerateSplines();

	const float len = sp.GetArcLength();
	for (int i = 0; i < controlPoints.Length(); i++)
	{
		const float d = len * (float)i / (controlPoints.Length() - 1);
		points[i] = sp.GetPointAt(d).y;
	}
}

void MorganaEngine::Base::Math::Curve::ZeroPoints()
{
	memset(points.ptr(), 0, points.bufsize());
}
