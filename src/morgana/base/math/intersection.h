#ifndef __MORGANA_BASE_MATH_INTERSECTION_H__
#define __MORGANA_BASE_MATH_INTERSECTION_H__

#include "vector3.h"
#include "box.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Math
		{
			class Intersection
			{
			public:

				static const bool SphereBox(const Vector3& sphereCenter, const float& sphereRadius, const Box& bbox);
				static const void DistancePointToLine(const Vector3& linepoint1, const Vector3& linepoint2, const Vector3& point, float &outDistSqr, float &outT);

				/*static bool Ray_BoundingBox(const Vector3 &bbox_min, const Vector3 &bbox_max, const Vector3 &origin, const Vector3 &direction, Vector3 &intercept);
				static bool Ray_Triangle(Vector3* tri, Vector3 &origin, Vector3 &direction, Vector3 &intercept);
				static bool Ray_Plane(Vector3 &start, Vector3 &dir, Plane &p, Vector3 &intercept);
				static bool Point_Triangle(Vector3* tri, Vector3& normal, Vector3& point_to_test);

				static bool	Ray_Mesh(Vector3 &origin, Vector3 &direction, Mesh* m, Vector3 &intercept, bool both_sides = false, const float extra_scale = 1.0f);
				static bool	Ray_MeshTransform(Vector3 &origin, Vector3 &direction, Mesh* m, Matrix* mat, Vector3 &intercept, bool both_sides = false);

				static bool SphereSphere(const Vector3 * const pos1, const float _rad1, const Vector3 * const pos2, const float _rad2);
				static bool SphereSphere(const Vector3 * const pos, const float _rad, const BoundingSphere * const sphere);
				static bool SphereSphere(const BoundingSphere * const sphere0, const BoundingSphere * const sphere1);
				static bool SphereBox(const Vector3 * const spherePos, const float rad, const Vector3 *const boxPos, const BoundingBox *const box);
				static bool SphereOrientedBox(const Vector3 *const spherePos, const float rad, const Vector3 *const boxPos, const BoundingBox *const box, const Matrix *const mat, const float scale = 1.f);
				static bool SphereOrientedBox(const BoundingSphere *const sphere, const Vector3 *const boxPos, const BoundingBox *const box, const Matrix *const mat, const float scale = 1.f);
				static bool PointSphere(const Vector3 *const pos, const Vector3 *const spherePos, const float sphereRadius);
				static bool PointSphere(const Vector3 *const pos, const BoundingSphere *const sphere);
				static bool PointSphere2(const Vector3 *const pos, const BoundingSphere *const sphere);
				static bool PointBox(const Vector3 *const pos, const Vector3 *const boxPos, const BoundingBox *const box);
				static bool PointOrientedBox(const Vector3 *const pos, const Vector3 *const boxPos, const BoundingBox *const box, const Matrix *const mat);

				static bool SphereBox(const Vector3* pos, float rad, const Matrix* box);
				static bool SphereCylinder(const Vector3* pos, float rad, const Matrix* cyl);
				static bool IntersectPlane(Vector3 normplane, Ray& r, Vector3& intercept);

				static Vector2 IntersectLines(const Vector2& v1, const Vector2& v2, float a, float b, float c);
				static int IntersectPolyWithLine(Vector2* src, Vector2* dst, int cnt, float a, float b, float c);

				static bool SphereCone(const Vector3 * const SpherePos, const float SphereRadius, const Vector3 * const ConeApex, const Vector3 * const ConeDirection, const float ConeAngle);
				static float DistancePointToLineSquared(const Vector3 *const linepoint1, const Vector3 *const linepoint2, const Vector3 *const point);
				static float DistancePointToLineProjSquared(const Vector3 *const linepoint1, const Vector3 *const linepoint2, const Vector3 *const point);

				//Triangle1/Triangle2 point to an array of 3 Vector3, which are the triangle's vertices
				static bool TriangleTriangle(const Vector3 * const Triangle1, const Vector3 * const Triangle2);

				static bool PointInsideConvexSpace(const Vector3 * const Point, int PlaneCount, const Vector3 * const PlanePoints, const Vector3 * const Normals);

				static const bool TriangleSphere(const Vector3& t1, const Vector3& t2, const Vector3& t3, const Vector3& center, const float r);

				static float PointPlaneDistanceSign(const Vector3 &planeNormal, const float planeD, const Vector3 &p);
				static float PointPlaneDistance(const Vector3 &planeNormal, const float planeD, const Vector3 &p);
				static bool SegmentPlane(const Vector3 &planeNormal, const float planeD, const Vector3 &s1, const Vector3 &s2, float *t, Vector3 *out);
				*/
			};
		}
	}
}

#endif