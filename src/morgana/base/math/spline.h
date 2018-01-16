#ifndef __MORGANA_BASE_MATH_SPLINE_H__
#define __MORGANA_BASE_MATH_SPLINE_H__


#include "mearray.h"
#include "vector3.h"

namespace MorganaEngine
{
	namespace Base
	{
		using namespace Types;
		namespace Math
		{
			class Cubic
			{
				float a, b, c, d;         /* a + b*u + c*u^2 +d*u^3 */

			public:
				Cubic()
				{
					a = b = c = d = 0.f;
				}
				Cubic(float a, float b, float c, float d)
				{
					this->a = a;
					this->b = b;
					this->c = c;
					this->d = d;
				}
				~Cubic(){};

				/** evaluate cubic */
			public:
				inline float eval(float u)
				{
					return (((d*u) + c)*u + b)*u + a;
				}

				inline float tangent(float u)
				{
					return ((3 * d*u) + 2 * c)*u + b;
				}
			};

			class Spline
			{
			public:
				Array<Cubic>			xCubics;
				Array<Cubic>			yCubics;
				Array<Cubic>			zCubics;
				Array<float>			xCoords;
				Array<float>			yCoords;
				Array<float>			zCoords;

				Array<Vector3>			controlPoints;
				Array<Vector3>			splinePoints;
				Array<float>			splineDistances;

				Array<float>			segmentLengths;

				Array<float>			segmentLengthsCummulative;
				void					MakeSplineSegmentLengthsCummulative();

				virtual void RegenerateSpline(Array<float>& theInput, Array<Cubic>& theOutput);
				virtual void RegenerateClosedSpline(Array<float>& theInput, Array<Cubic>& theOutput);
				virtual float GetMinUFromLineAB(Vector3 A, Vector3 B, Vector3 C);
				virtual float GetMinDistanceFromLineAB(Vector3 A, Vector3 B, Vector3 C);

				float			mArcLength;
				unsigned int	mGranularity;

				bool			mClosed;

			public:
				Spline();
				Spline(const Vector3& startPos, const Vector3& endPos, int numCurves, float trajstepx, int granularity);
				virtual ~Spline();

				Spline*					Clone();
				void						Reverse();


				// The main functions
				virtual void				AddPoint(Vector3 thePoint, bool regenerate);
				virtual void				AddPoint(Vector3 thePoint);
				virtual void				RegenerateSplines();	// You Shouldn't have to call this.
				virtual Vector3				GetPointAt(float theDistanceOnTheSpline) const;
				virtual Vector3				GetPointAtNormalized(float normalizedDistance) const;
				virtual Vector3				GetTangentAt(float theLength) const;
				virtual Vector3             GetTangentAtNormalized(float normalizedDistance) const;

				virtual void				GetPointAndTangentAt(float theLength, Vector3*) const;

				float						NormalizeDistanceOnSpline(float d) const;

				// ADT functions
				virtual int					GetNumControlPoints(){ return (int)controlPoints.Length(); };
				virtual int					GetNumSplineSegments(){ return (int)xCubics.Length(); };
				virtual float				GetArcLength() const { return mArcLength; };
				virtual float				GetSegmentLength(int index) { return segmentLengths[index]; }
				virtual void				SetClosed(bool bClosed){ mClosed = bClosed; RegenerateSplines(); };
				virtual bool				isClosed(){ return mClosed; };
				virtual int					GetGranularity(){ return mGranularity; };
				virtual void				SetGranularity(int theGranularity){ mGranularity = theGranularity; RegenerateSplines(); };

				// For Curve Refinement
				virtual void				BisectSegment(int theSplineSegmentId);
				virtual void				DeleteControlPoint(int theControlPointId);
				virtual void				ClearAllPoints();

				// Picking Helper Functions
				virtual float				GetClosestPointOnSegmentToPoint(const Vector3 &thePoint);
				virtual int					GetControlPointIdNear(Vector3 thePoint, const float& range = 20.0f);
				virtual int					GetSegmentIdNear(Vector3 thePoint);
				virtual Vector3				GetControlPoint(int theControlPointId);
				virtual void				SetControlPoint(int theControlPointId, Vector3 thePoint);

				void						Vibrate(float step, float amplitude, bool random = false);

				virtual void                GetPointAndTangentPreciseAt(float dist, Vector3 &point, Vector3 &tangent);
				virtual Vector3             GetPointPreciseAt(float dist);
				virtual Vector3             GetTangentPreciseAt(float dist);
				virtual float               GetNearestPointPreciseDist(const Vector3 &point);
			};
		}
	}
}

#endif
