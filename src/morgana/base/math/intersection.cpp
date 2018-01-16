#include "intersection.h"
#include "mathutils.h"

#include <math.h>

using namespace MorganaEngine::Base::Math;


const bool Intersection::SphereBox(const Vector3& sphereCenter, const float& sphereRadius, const Box& bbox)
{
	float dmin = 0;

	const Vector3 center = sphereCenter;
	const Vector3 bmin = bbox.center - bbox.offset;
	const Vector3 bmax = bbox.center + bbox.offset;
	
	if (center.x < bmin.x) {
		dmin += Mathf::Pow(center.x - bmin.x, 2);
	} else if (center.x > bmax.x) {
		dmin += Mathf::Pow(center.x - bmax.x, 2);
	}

	if (center.y < bmin.y) {
		dmin += Mathf::Pow(center.y - bmin.y, 2);
	} else if (center.y > bmax.y) {
		dmin += Mathf::Pow(center.y - bmax.y, 2);
	}

	if (center.z < bmin.z) {
		dmin += Mathf::Pow(center.z - bmin.z, 2);
	} else if (center.z > bmax.z) {
		dmin += Mathf::Pow(center.z - bmax.z, 2);
	}

	return dmin <= Mathf::Pow(sphereRadius, 2);
}

const void Intersection::DistancePointToLine(const Vector3& linepoint1, const Vector3& linepoint2, const Vector3& point, float &outDistSqr, float &outT)
{
	/*
	float minimum_distance(vec2 v, vec2 w, vec2 p) {
	// Return minimum distance between line segment vw and point p
	const float l2 = length_squared(v, w);  // i.e. |w-v|^2 -  avoid a sqrt
	if (l2 == 0.0) return distance(p, v);   // v == w case
	// Consider the line extending the segment, parameterized as v + t (w - v).
	// We find projection of point p onto the line.
	// It falls where t = [(p-v) . (w-v)] / |w-v|^2
	const float t = dot(p - v, w - v) / l2;
	if (t < 0.0) return distance(p, v);       // Beyond the 'v' end of the segment
	else if (t > 1.0) return distance(p, w);  // Beyond the 'w' end of the segment
	const vec2 projection = v + t * (w - v);  // Projection falls on the segment
	return distance(p, projection);
	}*/
	Vector3 v = linepoint1;
	Vector3 w = linepoint2;
	Vector3 p = point;

	Vector3 ld = v - w;
	float l2 = ld.LengthSquared();

	if (l2 < 1e-5f)
	{
		ld = p - v;
		outDistSqr = ld.LengthSquared();
		outT = 0.0f;
		return;
	}

	Vector3 v1, v2;
	v1 = p - v;
	v2 = w - v;
	float t = Vector3::Dot(&v1, &v2) / l2;
	if (t < 0.0f)
	{
		ld = p - v;
		outDistSqr = ld.LengthSquared();
		outT = t;
		return;
	}
	if (t > 1.0f)
	{
		ld = p - w;
		outDistSqr = ld.LengthSquared();
		outT = t;
		return;
	}

	Vector3 proj = v + t * (w - v);

	ld = proj - p;
	outT = t;
	outDistSqr = ld.LengthSquared();
}

#if 0
// --------------------
//
// Defines
//
// --------------------

#define CLIP_RIGHT	    (1<<0)	// cohen-sutherland clipping outcodes
#define CLIP_LEFT		(1<<1)
#define CLIP_TOP		(1<<2)
#define CLIP_BOTTOM	    (1<<3)
#define CLIP_FRONT	    (1<<4)
#define CLIP_BACK		(1<<5)
#define CLIP_ALL        (CLIP_RIGHT | CLIP_LEFT | CLIP_TOP | CLIP_BOTTOM | CLIP_FRONT | CLIP_BACK);

// --------------------

// calculates the cohen-sutherland outcode for a point and a bounding box.
//
// bbox_min:	min Vector3 of the bounding box
// bbox_max:	max Vector3 of the bounding box
// pnt:			the point to check
//
// returns:		the outcode
//
static unsigned long calc_outcode(const Vector3 &bbox_min, const Vector3 &bbox_max, const Vector3 &pnt)
{
	unsigned long outcode = 0;

	if (pnt.x > bbox_max.x) {
		outcode |= CLIP_RIGHT;
	}
	else if (pnt.x < bbox_min.x) {
		outcode |= CLIP_LEFT;
	}
	if (pnt.y > bbox_max.y) {
		outcode |= CLIP_TOP;
	}
	else if (pnt.y < bbox_min.y) {
		outcode |= CLIP_BOTTOM;
	}
	if (pnt.z > bbox_max.z) {
		outcode |= CLIP_BACK;
	}
	else if (pnt.z < bbox_min.z) {
		outcode |= CLIP_FRONT;
	}

	return outcode;
}

// --------------------
//
// External Functions
//
// --------------------

// checks if a line crosses from the front of a Plane to the back
//
// start:	start point of the line
// dir:	direction of the line, does not need to be normalized
// p:		the Plane
// fraction: (OUT) how far along the line it crossed the Plane
//
// returns: true if the line crossed from front to back
//
bool Intersection::Ray_Plane(Vector3 &start, Vector3 &dir, Plane &p, Vector3& intercept)
{
	float dist, len;

	dist = p.Distance(start);
	if (dist < 0) {
		// behind Plane
		return false;
	}

	len = -(Vector3::Dot(&dir, &p.GetNormal()));

	//
	intercept = start + dir * (dist / len);
	return true;
}

// checks if a sphere crosses from the front of a Plane to the back
//
// start:	start point of the line
// dir:	direction of the line, does not need to be normalized
// radius:	the radius of the sphere
// p:		the Plane
// fraction: (OUT) how far along the line it crossed the Plane
//
// returns: true if the sphere crossed from front to back
//
bool collide_sphere_plane(Vector3 &start, Vector3 &dir, float radius, Plane &p, Vector3 &hit_point)
{
	// find the closest point on the sphere to the Plane
	Vector3 sphere_bottom = start - (p.GetNormal() * radius);

	// collide the point like a regular ray
	if (!Intersection::Ray_Plane(sphere_bottom, dir, p, hit_point)) {
		// no dice
		return false;
	}

	return true;
}

// see if a point in inside a face by projecting into 2d.
//
// hit_point:	the point to test
// verts:		polygon vertices
// norm1:		polygon GetNormal()
//
// returns: true if hit point is within polygon
//
bool Intersection::Point_Triangle(Vector3* tri, Vector3& normal, Vector3& point_to_test)
{
	// given largest component of GetNormal(), return i & j
	static int ij_table[3][2] = { { 2, 1 },	// norm x biggest
	{ 0, 2 },	// norm y biggest
	{ 1, 0 } };// norm z biggest	int i0, i1,i2;

	float u0, u1, u2, v0, v1, v2;
	int i0, i1, i2;
	float alpha, beta;

	// find largest component of GetNormal()
	float abs_x = (float)fabs(normal.x);
	float abs_y = (float)fabs(normal.y);
	float abs_z = (float)fabs(normal.z);
	if (abs_x > abs_y) {
		if (abs_x > abs_z) {
			i0 = 0;
		}
		else {
			i0 = 2;
		}
	}
	else {
		if (abs_y > abs_z) {
			i0 = 1;
		}
		else {
			i0 = 2;
		}
	}

	// pretend that norm1 is an array of three floats.
	float *norm1_xyz = &normal.x;
	// figure out the two dimensions we are going to use
	if (norm1_xyz[i0] > 0.0f) {
		i1 = ij_table[i0][0];
		i2 = ij_table[i0][1];
	}
	else {
		i1 = ij_table[i0][1];
		i2 = ij_table[i0][0];
	}

	// pretend that hit_point and *(verts[N]) are arrays of three floats.
	float *hit_point_xyz = &point_to_test.x;
	float *verts0_xyz = &(tri[0].x);
	float *verts1_xyz = &(tri[1].x);
	float *verts2_xyz = &(tri[2].x);

	// get deltas for hitpoint
	u0 = hit_point_xyz[i1] - verts0_xyz[i1];
	v0 = hit_point_xyz[i2] - verts0_xyz[i2];
	// ditto edge 1
	u1 = verts1_xyz[i1] - verts0_xyz[i1];
	v1 = verts1_xyz[i2] - verts0_xyz[i2];
	// ditto edge 2
	u2 = verts2_xyz[i1] - verts0_xyz[i1];
	v2 = verts2_xyz[i2] - verts0_xyz[i2];

	// calculate alpha and beta
	if ((u1 > -0.0001f) && (u1 < 0.0001f)) {
		// special case to guard against divide by zero
		beta = u0 / u2;
		if ((beta >= 0.0f) && (beta <= 1.0f)) {
			alpha = (v0 - beta*v2) / v1;
			return ((alpha >= 0.0f) && (alpha + beta <= 1.0f));
		}
	}
	else {
		beta = (v0*u1 - u0*v1) / (v2*u1 - u2*v1);
		if ((beta >= 0.0f) && (beta <= 1.0f))	{
			alpha = (u0 - beta*u2) / u1;
			return ((alpha >= 0.0f) && (alpha + beta <= 1.0f));
		}
	}

	// not inside polygon
	return false;
}

// given an edge of a polygon and a moving sphere, find the first contact the sphere
//  makes with the edge, if any.  note that hit_time must be primed with a  value of 1
//  before calling this function the first time.  it will then maintain the closest
//  collision in subsequent calls.
//
// xs0:			start point (center) of sphere
// vs: 			path of sphere during frame
// rad:			radius of sphere
// v0:			vertex #1 of the edge
// v1:			vertex #2 of the edge
// hit_time:	(OUT) time at which sphere collides with polygon edge
// hit_point:	(OUT) point on edge that is hit
//
// returns - whether the edge (or it's vertex) was hit
bool collide_edge_sphereline(Vector3 &xs0, Vector3 &vs, float rad, Vector3 &v0, Vector3 &v1, float &hit_time, Vector3 &hit_point)
{
	static Vector3 temp_sphere_hit;
	bool try_vertex = false; // Assume we don't have to try the vertices.

	Vector3 ve = v1 - v0;
	Vector3 delta = xs0 - v0;
	float delta_dot_ve = Vector3::Dot(&delta, &ve);
	float delta_dot_vs = Vector3::Dot(&delta, &vs);
	float delta_sqr = delta.LengthSquared();
	float ve_dot_vs = Vector3::Dot(&ve, &vs);
	float ve_sqr = ve.LengthSquared();
	float vs_sqr = vs.LengthSquared();

	float temp;

	// position of the collision along the edge is given by: xe = v0 + ve*s, where s is
	//  in the range [0,1].  position of sphere along its path is given by:
	//  xs = xs + vs*t, where t is in the range [0,1].  t is time, but s is arbitrary.
	//
	// solve simultaneous equations
	// (1) distance between edge and sphere center must be sphere radius
	// (2) line between sphere center and edge must be perpendicular to edge
	//
	// (1) (xe - xs)*(xe - xs) = rad*rad
	// (2) (xe - xs) * ve = 0
	//
	// then apply mathematica

	float A, B, C, root, discriminant;
	float root1 = 0.0f;
	float root2 = 0.0f;
	A = ve_dot_vs * ve_dot_vs - ve_sqr * vs_sqr;
	B = 2 * (delta_dot_ve * ve_dot_vs - delta_dot_vs * ve_sqr);
	C = delta_dot_ve * delta_dot_ve + rad * rad * ve_sqr - delta_sqr * ve_sqr;

	if (A > -0.0001f && A < 0.0001f) {
		// degenerate case, sphere is traveling parallel to edge
		try_vertex = true;
	}
	else {
		discriminant = B*B - 4 * A*C;
		if (discriminant > 0) {
			root = (float)Math::Sqrt(discriminant);
			root1 = (-B + root) / (2 * A);
			root2 = (-B - root) / (2 * A);

			// sort root1 and root2, use the earliest intersection.  the larger root
			//  corresponds to the final contact of the sphere with the edge on its
			//  way out.
			if (root2 < root1) {
				temp = root1;
				root1 = root2;
				root2 = temp;
			}

			// root1 is a time, check that it's in our currently valid range
			if ((root1 < 0) || (root1 >= hit_time)) {
				return false;
			}

			// find sphere and edge positions
			temp_sphere_hit = xs0 + vs * root1;

			Vector3 diff = temp_sphere_hit - v0;
			// check if hit is between v0 and v1
			float s_edge = (Vector3::Dot(&diff, &ve)) / ve_sqr;
			if ((s_edge >= 0) && (s_edge <= 1)) {
				// bingo
				hit_time = root1;
				hit_point = v0 + ve * s_edge;
				return true;
			}
		}
		else {
			// discriminant negative, sphere passed edge too far away
			return false;
		}
	}

	// sphere missed the edge, check for a collision with the first vertex.  note
	//  that we only need to check one vertex per call to check all vertices.
	A = vs_sqr;
	B = 2 * delta_dot_vs;
	C = delta_sqr - rad * rad;

	discriminant = B*B - 4 * A*C;
	if (discriminant > 0) {
		root = (float)Math::Sqrt(discriminant);
		root1 = (-B + root) / (2 * A);
		root2 = (-B - root) / (2 * A);

		// sort the solutions
		if (root1 > root2) {
			temp = root1;
			root1 = root2;
			root2 = temp;
		}

		// check hit vertex is valid and earlier than what we already have
		if ((root1 < 0) || (root1 >= hit_time)) {
			return false;
		}
	}
	else {
		// discriminant negative, sphere misses vertex too
		return false;
	}

	// bullseye
	hit_time = root1;
	hit_point = v0;
	return true;
}

// tests two axis-aligned bounding boxes for intersection
//
// min1:	- min Vector3 of bounding box 1
// max1:	- max Vector3 of bounding box 1
// min2:	- min Vector3 of bounding box 2
// max2:	- max Vector3 of bounding box 2
//
// returns true if boxes intersect
//
bool ix_box_box_aligned(Vector3 &min1, Vector3 &max1, Vector3 &min2, Vector3 &max2)
{
	if (min1.x > max2.x || min1.y > max2.y || min1.z > max2.z || max1.x < min2.x || max1.y < min2.y || max1.z < min2.z) {
		return false;
	}

	return true;
}

// determines if a linesegment intersects a bounding box. this is based on
//  the cohen-sutherland line-clipping algorithm.
//
// bbox_min:	bounding box min Vector3
// bbox_max:	bounding box max Vector3
// p1:			end point of line segment
// p2:			other end point
// intercept:	(out) the point in/on the bounding box where the intersection
//				  occured.  note that this point may not be on the surface of the box.
//
// returns:		true if the segment and box intersect.
//
bool Intersection::Ray_BoundingBox(const Vector3 &bbox_min, const Vector3 &bbox_max, const Vector3 &origin, const Vector3 &direction, Vector3 &intercept)
{
	unsigned long outcode1, outcode2;

	Vector3 p1 = origin;
	Vector3 p2 = origin + direction;

	outcode1 = calc_outcode(bbox_min, bbox_max, p1);
	if (outcode1 == 0) {
		// point inside bounding box
		intercept = p1;
		return true;
	}

	outcode2 = CLIP_ALL;
	outcode2 &= ~outcode1;//calc_outcode( bbox_min, bbox_max, p2 );
	if (outcode2 == 0) {
		// point inside bounding box
		intercept = p2;
		return true;
	}
	if ((outcode1 & outcode2) > 0) {
		// both points on same side of box
		return false;
	}

	// check intersections
	if (outcode1 & (CLIP_RIGHT | CLIP_LEFT)) {
		if (outcode1 & CLIP_RIGHT) {
			intercept.x = bbox_max.x;
		}
		else {
			intercept.x = bbox_min.x;
		}
		float x1 = p2.x - p1.x;
		float x2 = intercept.x - p1.x;
		intercept.y = p1.y + x2 * (p2.y - p1.y) / x1;
		intercept.z = p1.z + x2 * (p2.z - p1.z) / x1;

		if (intercept.y <= bbox_max.y && intercept.y >= bbox_min.y && intercept.z <= bbox_max.z && intercept.z >= bbox_min.z) {
			return true;
		}
	}

	if (outcode1 & (CLIP_TOP | CLIP_BOTTOM)) {
		if (outcode1 & CLIP_TOP) {
			intercept.y = bbox_max.y;
		}
		else {
			intercept.y = bbox_min.y;
		}
		float y1 = p2.y - p1.y;
		float y2 = intercept.y - p1.y;
		intercept.x = p1.x + y2 * (p2.x - p1.x) / y1;
		intercept.z = p1.z + y2 * (p2.z - p1.z) / y1;

		if (intercept.x <= bbox_max.x && intercept.x >= bbox_min.x && intercept.z <= bbox_max.z && intercept.z >= bbox_min.z) {
			return true;
		}
	}

	if (outcode1 & (CLIP_FRONT | CLIP_BACK)) {
		if (outcode1 & CLIP_BACK) {
			intercept.z = bbox_max.z;
		}
		else {
			intercept.z = bbox_min.z;
		}
		float z1 = p2.z - p1.z;
		float z2 = intercept.z - p1.z;
		intercept.x = p1.x + z2 * (p2.x - p1.x) / z1;
		intercept.y = p1.y + z2 * (p2.y - p1.y) / z1;

		if (intercept.x <= bbox_max.x && intercept.x >= bbox_min.x && intercept.y <= bbox_max.y && intercept.y >= bbox_min.y) {
			return true;
		}
	}

	// nothing found
	return false;
}

// determines a collision between a ray and a sphere
//
// ray_start:	the start pos of the ray
// ray_dir:		the normalized direction of the ray
// length:		length of ray to check
// sphere_pos:	sphere position
// sphere_rad:	sphere redius
// hit_time:	(OUT) if a collision, contains the distance from ray.pos
// hit_pos:		(OUT) if a collision, contains the world point of the collision
//
// returns: true if a collision occurred
//
bool collide_ray_sphere(Vector3 &ray_start, Vector3 &ray_dir, float length, Vector3 &sphere_pos, float sphere_rad, float &hit_time, Vector3 &hit_pos)
{
	// get the offset Vector3
	Vector3 offset = sphere_pos - ray_start;

	// get the distance along the ray to the center point of the sphere
	float ray_dist = Vector3::Dot(&ray_dir, &offset);
	if (ray_dist <= 0 || (ray_dist - length) > sphere_rad) {
		// moving away from object or too far away
		return false;
	}

	// get the squared distances
	float off2 = Vector3::Dot(&offset, &offset);
	float radd2 = sphere_rad * sphere_rad;
	if (off2 <= radd2) {
		// we're in the sphere
		hit_pos = ray_start;
		hit_time = 0;
		return true;
	}

	// find hit distance squared
	float d = radd2 - (off2 - ray_dist * ray_dist);
	if (d < 0) {
		// ray passes by sphere without hitting
		return false;
	}

	// get the distance along the ray
	hit_time = (float)(ray_dist - Math::Sqrt((d)));
	if (hit_time > length) {
		// hit point beyond length
		return false;
	}

	// sort out the details
	hit_pos = ray_start + ray_dir * hit_time;
	hit_time /= length;
	return true;
}

bool Intersection::Ray_Triangle(Vector3* tri, Vector3 &origin, Vector3 &direction, Vector3 &intercept)
{
	Vector3 v1 = tri[1] - tri[0]; v1.Normalize();
	Vector3 v2 = tri[2] - tri[0]; v2.Normalize();

	Vector3 n = Vector3::Cross(&v2, &v1); n.Normalize();
	const float dist = -Vector3::Dot(&n, &tri[0]);

	Plane p(n.x, n.y, n.z, dist);
	if (Ray_Plane(origin, direction, p, intercept) == false)
		return false;

	return Point_Triangle(tri, n, intercept);
}


bool Intersection::SphereSphere(const Vector3 *const pos1, const float _rad1, const Vector3 *const pos2, const float _rad2)
{
	Vector3 l = *pos2 - *pos1;
	float dst2 = Vector3::Dot(&l, &l);
	float mindst = _rad1 + _rad2;

	return (dst2 <= mindst * mindst);
}

bool Intersection::SphereSphere(const Vector3 *const pos, const float rad, const BoundingSphere *const sphere)
{
	Vector3 l = sphere->GetCenter() - *pos;
	float dst2 = Vector3::Dot(&l, &l);
	float mindst = rad + sphere->GetRadius();

	return (dst2 <= mindst * mindst);
}

bool Intersection::SphereSphere(const BoundingSphere *const sphere0, const BoundingSphere *const sphere1)
{
	Vector3 l = sphere1->GetCenter() - sphere0->GetCenter();
	float dst2 = Vector3::Dot(&l, &l);
	float mindst = sphere0->GetRadius() + sphere1->GetRadius();

	return (dst2 <= mindst * mindst);
}

bool Intersection::SphereBox(const Vector3 * const spherePos, const float rad, const Vector3 * const boxPos, const BoundingBox *box)
{
	Vector3 p1, p2;
	p1 = *boxPos + box->center - box->offset;
	p1 -= rad;
	p2 = *boxPos + box->center + box->offset;
	p2 += rad;

	// x test
	if (spherePos->x < p1.x || spherePos->x > p2.x)
		return false;

	// y test
	if (spherePos->y < p1.y || spherePos->y > p2.y)
		return false;

	// z test
	if (spherePos->z < p1.z || spherePos->z > p2.z)
		return false;

	return true;
}

bool Intersection::SphereOrientedBox(const Vector3 *const spherePos, const float rad, const  Vector3 *const boxPos, const BoundingBox *const box, const Matrix *const mat, const float scale)
{
	const Vector3 backward = -mat->Backward();
	Vector3 loc, relloc, p1, p2;

	relloc = *spherePos - *boxPos;

	loc.x = Vector3::Dot(&relloc, &mat->Right());
	loc.y = Vector3::Dot(&relloc, &mat->Up());
	loc.z = Vector3::Dot(&relloc, &backward);

	p1 = box->center - box->offset * scale;
	p1 -= rad;
	p2 = box->center + box->offset * scale;
	p2 += rad;

	// x test
	if (loc.x < p1.x || loc.x > p2.x)
		return false;

	// y test
	if (loc.y < p1.y || loc.y > p2.y)
		return false;

	// z test
	if (loc.z < p1.z || loc.z > p2.z)
		return false;

	return true;
}

bool Intersection::SphereOrientedBox(const BoundingSphere *const sphere, const Vector3 *const boxPos, const BoundingBox *const box, const Matrix *const mat, const float scale)
{
	const Vector3 backward = -mat->Backward();
	Vector3 loc, relloc, p1, p2;

	relloc = sphere->GetCenter() - *boxPos;

	loc.x = Vector3::Dot(&relloc, &mat->Right());
	loc.y = Vector3::Dot(&relloc, &mat->Up());
	loc.z = Vector3::Dot(&relloc, &backward);

	p1 = box->center - box->offset * scale;
	p1 -= sphere->GetRadius();
	p2 = box->center + box->offset * scale;
	p2 += sphere->GetRadius();

	// x test
	if (loc.x < p1.x || loc.x > p2.x)
		return false;

	// y test
	if (loc.y < p1.y || loc.y > p2.y)
		return false;

	// z test
	if (loc.z < p1.z || loc.z > p2.z)
		return false;

	return true;
}

bool Intersection::PointSphere2(const Vector3 *const pos, const BoundingSphere *const sphere)
{
	const Vector3 l = sphere->GetCenter() - *pos;
	return Vector3::Dot(&l, &l) <= sphere->GetRadius();
}

bool Intersection::PointSphere(const Vector3 *const pos, const Vector3 *const spherePos, const float sphereRadius)
{
	Vector3 l = *spherePos - *pos;
	float dst2 = Vector3::Dot(&l, &l);

	return (dst2 <= sphereRadius * sphereRadius);
}

bool Intersection::PointSphere(const Vector3 *const pos, const BoundingSphere *const sphere)
{
	Vector3 l = sphere->GetCenter() - *pos;
	float dst2 = Vector3::Dot(&l, &l);

	return (dst2 <= sphere->GetRadius() * sphere->GetRadius());
}

bool Intersection::PointBox(const Vector3 *const pos, const Vector3 *const boxPos, const BoundingBox *const box)
{
	Vector3 p1, p2;
	p1 = *boxPos + box->center - box->offset;
	p2 = *boxPos + box->center + box->offset;

	// x test
	if (pos->x < p1.x || pos->x > p2.x)
		return false;

	// y test
	if (pos->y < p1.y || pos->y > p2.y)
		return false;

	// z test
	if (pos->z < p1.z || pos->z > p2.z)
		return false;

	return true;
}

bool Intersection::PointOrientedBox(const Vector3 *const pos, const Vector3 *const boxPos, const BoundingBox *const box, const Matrix *const mat)
{
	const Vector3 backward = -mat->Backward();
	Vector3 loc, relloc, p1, p2;

	relloc = *pos - *boxPos;

	loc.x = Vector3::Dot(&relloc, &mat->Right());
	loc.y = Vector3::Dot(&relloc, &mat->Up());
	loc.z = Vector3::Dot(&relloc, &backward);

	p1 = box->center - box->offset;
	p2 = box->center + box->offset;

	// x test
	if (loc.x < p1.x || loc.x > p2.x)
		return false;

	// y test
	if (loc.y < p1.y || loc.y > p2.y)
		return false;

	// z test
	if (loc.z < p1.z || loc.z > p2.z)
		return false;

	return true;
}

float planeDistance(const Vector3& plane_p, const Vector3& plane_n, const Vector3& p)
{
	float plane_d = -Vector3::Dot(&plane_p, &plane_n);

	float dist = Vector3::Dot(&p, &plane_n) + plane_d;

	return dist;
}

bool Intersection::SphereBox(const Vector3* pos, float rad, const Matrix* box)
{
	//first check the collision using spheres
	//the box radius is the diagonal :)
	//it is the smallest sphere that contains the box
	Vector3 diag;

	Vector3::TransformNormal(&Vector3::DiagonalXYZ, box, &diag);
	float br = diag.Length();

	if (!SphereSphere(pos, rad, &box->Translation(), br))
		return false;



	Vector3 sx, sy, sz;
	Vector3 nx, ny, nz;
	Vector3 bp = box->Translation();

	//we want the corners
	Vector3::TransformNormal(&Vector3::UnitX, box, &sx);
	Vector3::TransformNormal(&Vector3::UnitY, box, &sy);
	Vector3::TransformNormal(&Vector3::UnitZ, box, &sz);

	//and the normals
	Vector3::Normalize(&sx, &nx);
	Vector3::Normalize(&sy, &ny);
	Vector3::Normalize(&sz, &nz);


	if (planeDistance(bp + sx, nx, *pos) > rad)
		return false;
	if (planeDistance(bp - sx, -nx, *pos) > rad)
		return false;

	if (planeDistance(bp + sy, ny, *pos) > rad)
		return false;
	if (planeDistance(bp - sy, -ny, *pos) > rad)
		return false;

	if (planeDistance(bp + sz, nz, *pos) > rad)
		return false;
	if (planeDistance(bp - sz, -nz, *pos) > rad)
		return false;


	return true;
}

bool Intersection::SphereCylinder(const Vector3* pos, float rad, const Matrix* cyl)
{
	//first check the collision using spheres
	//the box radius is the XZ (or YZ) diagonal :)
	//it is the smallest sphere that contains the cylinder
	Vector3 diag;

	Vector3::TransformNormal(&Vector3::DiagonalXZ, cyl, &diag);
	float br = diag.Length();

	if (!SphereSphere(pos, rad, &cyl->Translation(), br))
		return false;

	Vector3 sx;
	Vector3 sz;
	Vector3 nz;
	Vector3 cp = cyl->Translation();



	//compute the distance from the point to the center line of the cylinder
	//first, we need the radius and center line
	Vector3::TransformNormal(&Vector3::UnitX, cyl, &sx);
	Vector3::TransformNormal(&Vector3::UnitZ, cyl, &sz);
	//and the normal
	Vector3::Normalize(&sz, &nz);

	Vector3 p1, p2;
	p1 = cp + sz;
	p2 = cp - sz;

	Vector3 d1 = *pos - p1;
	Vector3 d2 = *pos - p2;

	float dist = Vector3::Cross(&d1, &d2).Length() / (p2 - p1).Length();

	if (dist > sx.Length() + rad)
		return false;

	//now check the cap planes
	if (planeDistance(p1, nz, *pos) > rad)
		return false;
	if (planeDistance(p2, -nz, *pos) > rad)
		return false;

	return true;
}

bool Intersection::IntersectPlane(Vector3 normplane, Ray& r, Vector3& intercept)
{
	Plane p;
	p = Plane(&normplane, &intercept);
	return Intersection::Ray_Plane(r.origin, r.direction, p, intercept);
}

Vector2 Intersection::IntersectLines(const Vector2& v1, const Vector2& v2, float a, float b, float c)
{
	//horizontal line
	if (a == 0.0f)
	{
		float y = -c / b;
		float t = (y - v1.y) / (v2.y - v1.y);

		return Vector2(v1.x + (v2.x - v1.x)*t, y);
	}

	//vertical line
	if (b == 0.0f)
	{
		float x = -c / a;
		float t = (x - v1.x) / (v2.x - v1.x);

		return Vector2(x, v1.y + (v2.y - v1.y)*t);
	}

	//generic case
	return Vector2(0, 0);
}

int Intersection::IntersectPolyWithLine(Vector2* src, Vector2* dst, int cnt, float a, float b, float c)
{
	Vector2 s = src[cnt - 1];

	int n = 0;
	for (int i = 0; i<cnt; i++)
	{
		Vector2 e = src[i];

		if (e.x*a + e.y*b + c >= 0.0f)
		{
			if (s.x*a + s.y*b + c < 0.0f)
			{
				dst[n++] = IntersectLines(s, e, a, b, c);
			}
			dst[n++] = e;
		}
		else
		{
			if (s.x*a + s.y*b + c >= 0.0f)
			{
				dst[n++] = IntersectLines(s, e, a, b, c);
			}
		}
		s = e;
	}

	return n;
}

bool Intersection::SphereCone(const Vector3 * const SpherePos, const float SphereRadius, const Vector3 * const ConeApex, const Vector3 * const ConeDirection, const float ConeAngle)
{
	Vector3 V = (*SpherePos) - (*ConeApex);
	float a = Vector3::Dot(&V, ConeDirection);
	float b = a * Math::Tan(ConeAngle);
	float c = Math::Sqrt(Vector3::Dot(&V, &V) - a * a);
	float d = c - b;
	float e = d - Math::Cos(ConeAngle);
	if (e >= SphereRadius)
	{
		return false;
	}
	else
	{
		return true;
		/*        if (e <= -SphereRadius)
		{
		return true;
		}
		else
		{
		return true;
		}*/
	}
}

float Intersection::DistancePointToLineSquared(const Vector3 *const linepoint1, const Vector3 *const linepoint2, const Vector3 *const point)
{
	float dist, t;
	DistancePointToLine(linepoint1, linepoint2, point, dist, t);
	return dist;
}

float Intersection::DistancePointToLineProjSquared(const Vector3 *const linepoint1, const Vector3 *const linepoint2, const Vector3 *const point)
{
	float dist, t;
	DistancePointToLine(linepoint1, linepoint2, point, dist, t);
	if ((t < 0.0f) || (t > 1.0f))
	{
		return 1e30f;
	}
	return dist;
}

int tri_tri_intersect(float V0[3], float V1[3], float V2[3], float U0[3], float U1[3], float U2[3]);

bool Intersection::TriangleTriangle(const Vector3 * const Triangle1, const Vector3 * const Triangle2)
{
	float V0[3];
	float V1[3];
	float V2[3];
	float U0[3];
	float U1[3];
	float U2[3];
	Triangle1[0].Store(V0);
	Triangle1[1].Store(V1);
	Triangle1[2].Store(V2);
	Triangle2[0].Store(U0);
	Triangle2[1].Store(U1);
	Triangle2[2].Store(U2);
	return tri_tri_intersect(V0, V1, V2, U0, U1, U2);
}

bool Intersection::PointInsideConvexSpace(const Vector3 * const Point, int PlaneCount, const Vector3 * const PlanePoints, const Vector3 * const Normals)
{
	for (int a = 0; a < PlaneCount; a += 1)
	{
		Vector3 P1, P2;
		P1 = (*Point) - PlanePoints[a];
		P2 = Normals[a];

		float d = Vector3::Dot(&P1, &P2);
		if (d < 0.0f)
		{
			return false;
		}
	}
	return true;
}

const bool Intersection::TriangleSphere(const Vector3& t1, const Vector3& t2, const Vector3& t3, const Vector3& center, const float r)
{
	const Vector3 A = t1 - center;
	const Vector3 B = t2 - center;
	const Vector3 C = t3 - center;
	const float rr = r * r;

	const Vector3 BA = B - A;
	const Vector3 CA = C - A;

	const Vector3 V = Vector3::Cross(&BA, &CA);
	const float d = Vector3::Dot(&A, &V);
	const float e = Vector3::Dot(&V, &V);

	const bool sep1 = (d * d) > (rr * e);
	if (sep1 != false)
	{
		return false;
	}

	{
		const float aa = Vector3::Dot(&A, &A);
		const float ab = Vector3::Dot(&A, &B);
		const float ac = Vector3::Dot(&A, &C);
		const float bb = Vector3::Dot(&B, &B);
		const float bc = Vector3::Dot(&B, &C);
		const float cc = Vector3::Dot(&C, &C);

		const bool sep2 = (aa > rr) && (ab > aa) && (ac > aa);
		const bool sep3 = (bb > rr) && (ab > bb) && (bc > bb);
		const bool sep4 = (cc > rr) && (ac > cc) && (bc > cc);
		if ((sep2 != false) || (sep3 != false) || (sep4 != false))
		{
			return false;
		}


		{
			const Vector3 AB = B - A;
			const Vector3 BC = C - B;
			const Vector3 CA = A - C;

			const float d1 = ab - aa;
			const float d2 = bc - bb;
			const float d3 = ac - cc;
			const float e1 = Vector3::Dot(&AB, &AB);
			const float e2 = Vector3::Dot(&BC, &BC);
			const float e3 = Vector3::Dot(&CA, &CA);
			const Vector3 Q1 = A * e1 - d1 * AB;
			const Vector3 Q2 = B * e2 - d2 * BC;
			const Vector3 Q3 = C * e3 - d3 * CA;
			const Vector3 QC = C * e1 - Q1;
			const Vector3 QA = A * e2 - Q2;
			const Vector3 QB = B * e3 - Q3;
			const bool sep5 = (Vector3::Dot(&Q1, &Q1) > (rr * e1 * e1)) && (Vector3::Dot(&Q1, &QC) > 0.0f);
			const bool sep6 = (Vector3::Dot(&Q2, &Q2) > (rr * e2 * e2)) && (Vector3::Dot(&Q2, &QA) > 0.0f);
			const bool sep7 = (Vector3::Dot(&Q3, &Q3) > (rr * e3 * e3)) && (Vector3::Dot(&Q3, &QB) > 0.0f);

			if ((sep5 != false) || (sep6 != false) || (sep7 != false))
			{
				return false;
			}
		}
	}

	return true;
}

float Intersection::PointPlaneDistanceSign(const Vector3 &planeNormal, const float planeD, const Vector3 &p)
{
	float t = (Vector3::Dot(&planeNormal, &p) - planeD);
	return Math::Sign(t);
}

float Intersection::PointPlaneDistance(const Vector3 &planeNormal, const float planeD, const Vector3 &p)
{
	float t = (Vector3::Dot(&planeNormal, &p) - planeD) / Vector3::Dot(&planeNormal, &planeNormal);
	return t;
}

bool Intersection::SegmentPlane(const Vector3 &planeNormal, const float planeD, const Vector3 &s1, const Vector3 &s2, float *t, Vector3 *out)
{
	Vector3 ds = s2 - s1;
	float r = (planeD - Vector3::Dot(&planeNormal, &s1)) / Vector3::Dot(&planeNormal, &ds);
	if ((r < 0.0f) || (r > 1.0f))
	{
		return false;
	}
	if (t != NULL)
	{
		(*t) = r;
	}
	if (out != NULL)
	{
		(*out) = s1 + r * ds;
	}
	return true;
}

bool Intersection::Ray_Mesh(Vector3 &origin, Vector3 &direction, Mesh* m, Vector3 &intercept, bool both_sides /*= false*/, const float extra_scale /*= 1.0f*/)
{
	if (m->iBufferType != IndexBufferType::UnsignedShort)
		return false;

	if (m->drawMode != DrawMode::TriangleList)
		return false;

	const Vector3 min = m->boundsBox.center - m->boundsBox.offset * extra_scale;
	const Vector3 max = m->boundsBox.center + m->boundsBox.offset * extra_scale;

	if (Ray_BoundingBox(min, max, origin, direction, intercept) == false)
		return false;

	float dist = Math::MaxFloat;

	const unsigned char* verts = m->Vertices<unsigned char>();
	const int vertStride = VertexBufferType::SizeOf[m->vBufferType];

	const unsigned short* inds = m->Indices<unsigned short>();

	for (int i = 0; i<m->numIndices; i += 3)
	{
		const unsigned short i1 = inds[i];
		const unsigned short i2 = inds[i + 1];
		const unsigned short i3 = inds[i + 2];

		Vector3 tri[3];

		Vector3 v1 = *((Vector3*)(verts + vertStride * i1));
		Vector3 v2 = *((Vector3*)(verts + vertStride * i2));
		Vector3 v3 = *((Vector3*)(verts + vertStride * i3));

		if (extra_scale != 1.0f)
		{
			const Vector3 center = (v1 + v2 + v3) / 3.0f;

			v1 = (v1 - center) * extra_scale + center;
			v2 = (v2 - center) * extra_scale + center;
			v3 = (v3 - center) * extra_scale + center;
		}

		tri[0] = v1;
		tri[1] = v2;
		tri[2] = v3;

		Vector3 ip;
		if (Ray_Triangle(tri, origin, direction, ip) == false)
		{
			if (both_sides)
			{
				tri[1] = v3;
				tri[2] = v2;
				if (Ray_Triangle(tri, origin, direction, ip) == false)
					continue;
			}
			else
				continue;
		}

		const float d = Vector3::Distance(&origin, &ip);

		if (d < dist)
		{
			dist = d;
			intercept = ip;
		}
	}

	return dist < Mathf::MaxFloat;
}

bool Intersection::Ray_MeshTransform(Vector3 &origin, Vector3 &direction, Mesh* m, Matrix* mat, Vector3 &intercept, bool both_sides /*= false*/)
{
	Matrix invMat;
	Matrix::Invert(mat, &invMat);

	Ray r(origin, direction);
	r.Transform(&invMat);

	return Ray_Mesh(r.origin, r.direction, m, intercept, both_sides);
}
#endif