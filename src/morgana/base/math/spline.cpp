#include "spline.h"
#include "intersection.h"

using namespace MorganaEngine::Base::Math;
using namespace MorganaEngine::Base::Types;

Spline::Spline()
{
	mArcLength = 0.0f;
	mGranularity = 24;
	mClosed = false;
}


Spline::Spline(const Vector3& startPos, const Vector3& endPos, int num_curves, float trajstepx, int granularity)
{
	mArcLength = 0.0f;
	mGranularity = 24;
	mClosed = false;

	Vector3 dir = endPos;
	dir -= startPos;
	dir.Normalize();
	Vector3 left = Vector3::Cross(&dir,&Vector3::UnitY);
	left.Normalize();

	SetGranularity(granularity);
	AddPoint(Vector3(startPos.x, startPos.y, startPos.z));

	float traj_step_z = (endPos - startPos).Length()/num_curves;

	for (int i = 1; i < num_curves; i++)
	{
        float traj_step_x = trajstepx * /*(Math::Rand(2) - 1.0f);/*/(i % 3 - 1);
		//_nwKrnOutDebug("traj_step_x %f\n",traj_step_x);
		Vector3 newPos = (dir*traj_step_z*(float)i + left*traj_step_x)+startPos;
		Vector2 newPoint(newPos.x,newPos.z);
		//_nwKrnOutDebug("newPos.x,newPos.z %f %f\n",newPos.x,newPos.z);
		AddPoint(newPos);
	}
	//_nwKrnOutDebug("traj_step_z %f\n",traj_step_z);
	//_nwKrnOutDebug("start %f %f %f\n",start.x,start.y,start.z);
	//_nwKrnOutDebug("dest %f %f %f\n",dest.x,dest.y,dest.z);

	AddPoint(Vector3(endPos.x, endPos.y, endPos.z));
}



Spline::~Spline()
{
}

Spline* Spline::Clone()
{
    Spline* sp = new Spline();

	sp->mArcLength = mArcLength;
	sp->mGranularity = mGranularity;
	sp->mClosed = mClosed;

	sp->xCubics.Copy(xCubics);
	sp->yCubics.Copy(yCubics);
	sp->zCubics.Copy(zCubics);
	sp->xCoords.Copy(xCoords);
	sp->yCoords.Copy(yCoords);
	sp->zCoords.Copy(zCoords);
	sp->controlPoints.Copy(controlPoints);
	sp->splinePoints.Copy(splinePoints);
    sp->splineDistances.Copy(splineDistances);
	sp->segmentLengths.Copy(segmentLengths);

    return sp;
}

void Spline::Reverse()
{
	Array<Vector3>	TmpPoints;

	TmpPoints.Copy(controlPoints);
	ClearAllPoints();

	for(int i = 0;i<TmpPoints.Length();i++)
	{
		AddPoint(TmpPoints[TmpPoints.Length()-i-1]);
	}
}

void Spline::AddPoint(Vector3 thePoint, bool regenerate)
{
	//_nwKrnOutDebug("Adding point %d: %f %f\n",mPoints.Length(),thePoint.x,thePoint.y);
	xCoords.Add((float)thePoint.x);
	yCoords.Add((float)thePoint.y);
	zCoords.Add((float)thePoint.z);

	controlPoints.Add(thePoint);
	if (regenerate)
		RegenerateSplines();
}


void Spline::AddPoint(Vector3 thePoint)
{
	//_nwKrnOutDebug("Adding point %d: %f %f\n",mPoints.Length(),thePoint.x,thePoint.y);
	AddPoint(thePoint, true);
}

float Spline::NormalizeDistanceOnSpline(float d) const
{
	if(mClosed)
	{
		while(d < 0.0f)	d += mArcLength;
		while(d > mArcLength) d -= mArcLength;
		return d;
	}
	else
	{
		if(d < 0.0f) return 0.0f;
		if(d > mArcLength) return mArcLength;
		return d;
	}
}

// Returns the Tangent of a point on the line specified in Ribbon Coordinates.
// Tangents Tell the direction of travel and can be used to rotate the object to
// face the correct direction.  
Vector3 Spline::GetTangentAt(float theLength) const
{
	if(xCubics.Length() == 0) return Vector3(0,0,0);
	if(splinePoints.Length() == 0) return Vector3(0,0,0);

	float aDistance = NormalizeDistanceOnSpline(theLength);
	int anIndex = 0;
	for(int i = 0; i < segmentLengths.Length(); i++, anIndex++)
	{
		aDistance -= segmentLengths[i];
		if(aDistance < 0.0f)
		{
			aDistance += segmentLengths[i];
			break;
		}
	}

	if(anIndex < segmentLengths.Length())
	{
		float u = aDistance/segmentLengths[anIndex];
		Vector3 aVector(xCubics[anIndex].tangent(u), yCubics[anIndex].tangent(u), zCubics[anIndex].tangent(u));
		aVector.Normalize();

		return Vector3(aVector.x, aVector.y, aVector.z);
	}

	return Vector3(0,0,0);
}

// Resolves a Distance on the spline to a Cartesian 2d Point on the Screen. 
// Exact position evaluated on the spline.
Vector3 Spline::GetPointAt(float theDistanceOnTheSpline) const
{
	if(controlPoints.Length() == 0) return Vector3(0,0,0);
	if(splinePoints.Length() == 0) return controlPoints.Last();

	float aDistance = NormalizeDistanceOnSpline(theDistanceOnTheSpline);
	int anIndex = 0;

	for(int i = 0; i < segmentLengths.Length(); i++, anIndex++)
	{
		aDistance -= segmentLengths[i];
		if(aDistance < 0.0f)
		{
			aDistance += segmentLengths[i];
			break;
		}
	}
	
	if(anIndex < segmentLengths.Length())
	{
		float u = aDistance/segmentLengths[anIndex];
		return Vector3(xCubics[anIndex].eval(u), yCubics[anIndex].eval(u), zCubics[anIndex].eval(u));
	}

	return controlPoints.Last();
}

void Spline::GetPointAndTangentAt(float theDistanceOnTheSpline, Vector3* pp) const
{
	pp[0] = Vector3(0.0f, 0.0f, 0.0f);
	pp[1] = Vector3(0.0f, 0.0f, 0.0f);

	if(controlPoints.Length() == 0) return;
	if(splinePoints.Length() == 0) return;
	
	float aDistance = NormalizeDistanceOnSpline(theDistanceOnTheSpline);
	int anIndex = 0;

	int start = 0;
	int end = segmentLengths.Length();
	int middle = (end + start) / 2;
	while(!(aDistance >= segmentLengthsCummulative[middle] && aDistance <= segmentLengthsCummulative[middle + 1]))
	{
		if(aDistance >= segmentLengthsCummulative[middle])
		{
			start = middle;
		}
		else
		{
			end = middle;
		}

		middle = (end + start) / 2;

		if(middle == 0 || middle == segmentLengths.Length() - 1)
		{
			break;
		}
	}

	/*for(int i = 0; i < mSplineSegmentLengths.Length(); i++, anIndex++)
	{
		aDistance -= mSplineSegmentLengths[i];
		if(aDistance < 0.0f)
		{

			aDistance += mSplineSegmentLengths[i];
			break;
		}
	}*/

	anIndex = middle;
	aDistance -= segmentLengthsCummulative[middle];
	
	if(anIndex < segmentLengths.Length())
	{
		float u = aDistance/segmentLengths[anIndex];
		pp[0] = Vector3(xCubics[anIndex].eval(u), yCubics[anIndex].eval(u), zCubics[anIndex].eval(u));
		pp[1] = Vector3(xCubics[anIndex].tangent(u), yCubics[anIndex].tangent(u), zCubics[anIndex].tangent(u));
		pp[1].Normalize();
	}
}

// Creates and stores the Cubic Spline Data needed for all functions releated to Class.
// Requires 3 or more points caches the Splines as series of straight lines for faster operations
// such as picking and drawing.
void Spline::RegenerateSplines()
{
	//mega hack :D
	if(controlPoints.Length() == 2)
	{
		Vector3 p1 = controlPoints[0];
		Vector3 p2 = controlPoints[1];
		Vector3 p3 = (p1 + p2) * 0.5f;

		controlPoints.SetLength(0);
		xCoords.SetLength(0);
		yCoords.SetLength(0);
		zCoords.SetLength(0);
		mArcLength = 0.0f;

		AddPoint(p1, false);
		AddPoint(p3, false);
		AddPoint(p2, false);
	}

	if(mClosed) //Full Circle
	{
		RegenerateClosedSpline(xCoords, xCubics);
		RegenerateClosedSpline(yCoords, yCubics);
		RegenerateClosedSpline(zCoords, zCubics);
	}
	else
	{
		RegenerateSpline(xCoords, xCubics);
		RegenerateSpline(yCoords, yCubics);
		RegenerateSpline(zCoords, zCubics);
	}

	splinePoints.SetLength(0);
    splineDistances.SetLength(0);
	segmentLengths.SetLength(0);
	if(xCubics.Length())
	{
		splinePoints.Add(Vector3((float)xCubics[0].eval(0), (float)yCubics[0].eval(0), (float)zCubics[0].eval(0)));
        splineDistances.Add(0.0f);
		for(int x = 0; x < xCubics.Length(); x++)
		{
			segmentLengths.Add(0.0f);

			for(int steps = 1; steps <= (int)mGranularity; steps++)
			{
				float u = (float) steps / (float)mGranularity;
				splinePoints.Add(Vector3((float)xCubics[x].eval(u), (float)yCubics[x].eval(u), (float)zCubics[x].eval(u)));

				// Cache Segment Lengths
				unsigned int index = x*mGranularity + steps-1;
                float ddist = Vector3::Distance(splinePoints[index],splinePoints[index + 1]);
				segmentLengths[x] += ddist;

                splineDistances.Add(splineDistances.Last() + ddist);

			}
		}

		mArcLength = 0.0f;
		for(int i = 1; i < splinePoints.Length(); i++)
			mArcLength += (float)Mathf::Sqrt( ((float)splinePoints[i].x - splinePoints[i-1].x)*((float)splinePoints[i].x - splinePoints[i-1].x)
			+ ((float)splinePoints[i].y - splinePoints[i-1].y)*((float)splinePoints[i].y - splinePoints[i-1].y)
			+ ((float)splinePoints[i].z - splinePoints[i-1].z)*((float)splinePoints[i].z - splinePoints[i-1].z) );
	}

	MakeSplineSegmentLengthsCummulative();
}


// Generates the Spline with the assumption that the end is connected to the beginning
void Spline::RegenerateClosedSpline(Array<float>& theInput, Array<Cubic>& theOutput)
{
	theOutput.SetLength(0);

	if(theInput.Length() < 3) return;

	int n = (int)theInput.Length() - 1;

	Array<float> w;
    w.SetLength(n+1);
	Array<float> v;
    v.SetLength(n+1);
	Array<float> y;
    y.SetLength(n+1);
	Array<float> D;
    D.SetLength(n+1);

	float z, F, G, H;
	int k;
	/* We solve the equation
	[4 1      1] [D[0]]   [3(theInput[1] - theInput[n])  ]
	|1 4 1     | |D[1]|   |3(theInput[2] - theInput[0])  |
	|  1 4 1   | | .  | = |      .         |
	|    ..... | | .  |   |      .         |
	|     1 4 1| | .  |   |3(theInput[n] - theInput[n-2])|
	[1      1 4] [D[n]]   [3(theInput[0] - theInput[n-1])]

	by decomposing the matrix into upper triangular and lower matrices
	and then back sustitution.  See Spath "Spline Algorithms for Curves
	and Surfaces" pp 19--21. The D[i] are the derivatives at the knots.
	*/
	w[1] = v[1] = z = 1.0f/4.0f;
	y[0] = z * 3 * (theInput[1] - theInput[n]);
	H = 4;
	F = 3 * (theInput[0] - theInput[n-1]);
	G = 1;
	for ( k = 1; k < n; k++) {
		v[k+1] = z = 1/(4 - v[k]);
		w[k+1] = -z * w[k];
		y[k] = z * (3*(theInput[k+1]-theInput[k-1]) - y[k-1]);
		H = H - G * w[k];
		F = F - G * y[k-1];
		G = -v[k] * G;
	}
	H = H - (G+1)*(v[n]+w[n]);
	y[n] = F - (G+1)*y[n-1];

	D[n] = y[n]/H;
	D[n-1] = y[n-1] - (v[n]+w[n])*D[n]; /* This equation is WRONG! in my copy of Spath */
	for ( k = n-2; k >= 0; k--) {
		D[k] = y[k] - v[k+1]*D[k+1] - w[k+1]*D[n];
	}


	/* now compute the coefficients of the cubics */
	for ( k = 0; k < n; k++) {
		theOutput.Add(Cubic((float)theInput[k], D[k], 3*(theInput[k+1] - theInput[k]) - 2*D[k] - D[k+1],
			2*(theInput[k] - theInput[k+1]) + D[k] + D[k+1]));
	}
	theOutput.Add(Cubic((float)theInput[n], D[n], 3*(theInput[0] - theInput[n]) - 2*D[n] - D[0],
		2*(theInput[n] - theInput[0]) + D[n] + D[0]));
}

// Generates the Spline
void Spline::RegenerateSpline(Array<float>& theInput, Array<Cubic>& theOutput)
{
	theOutput.SetLength(0);

	if(theInput.Length() < 3) return;

	int n = (int)theInput.Length() - 1;
	Array<float> gamma(n+1);
	Array<float> delta(n+1);
	Array<float> D(n+1);
	int i;
	/* We solve the equation
	[2 1       ] [D[0]]   [3(theInput[1] - theInput[0])  ]
	|1 4 1     | |D[1]|   |3(theInput[2] - theInput[0])  |
	|  1 4 1   | | .  | = |      .         |
	|    ..... | | .  |   |      .         |
	|     1 4 1| | .  |   |3(theInput[n] - theInput[n-2])|
	[       1 2] [D[n]]   [3(theInput[n] - theInput[n-1])]

	by using row operations to convert the matrix to upper triangular
	and then back sustitution.  The D[i] are the derivatives at the knots.
	*/

	gamma[0] = 1.0f/2.0f;
	for ( i = 1; i < n; i++) 
	{
		gamma[i] = 1/(4-gamma[i-1]);
	}
	gamma[n] = 1/(2-gamma[n-1]);

	delta[0] = 3*(theInput[1]-theInput[0])*gamma[0];
	for ( i = 1; i < n; i++) 
	{
		delta[i] = (3*(theInput[i+1]-theInput[i-1])-delta[i-1])*gamma[i];
	}
	delta[n] = (3*(theInput[n]-theInput[n-1])-delta[n-1])*gamma[n];

	D[n] = delta[n];
	for ( i = n-1; i >= 0; i--) 
	{
		D[i] = delta[i] - gamma[i]*D[i+1];
	}

	/* now compute the coefficients of the cubics */
	for ( i = 0; i < n; i++) 
	{
		theOutput.Add(Cubic(
			(float)theInput[i],
			D[i],
			3*(theInput[i+1] - theInput[i]) - 2*D[i] - D[i+1],
			2*(theInput[i] - theInput[i+1]) + D[i] + D[i+1])
		);
	}
}


// Returns the linear distance on the Spline Closest to the input Point
// This is computed by using the cached line segments (not exact solution)
float Spline::GetClosestPointOnSegmentToPoint(const Vector3 &thePoint)
{
	if(splinePoints.Length())
	{
		float min_distance = Mathf::MaxFloat;// Something Large
		int min_index = -1;

		for(int i = 1; i < splinePoints.Length(); i++)
		{
			float temp_min_dist = (float)Mathf::Sqrt((thePoint.x - splinePoints[i].x)*(thePoint.x - splinePoints[i].x) + (thePoint.y - splinePoints[i].y)*(thePoint.y - splinePoints[i].y) + (thePoint.z - splinePoints[i].z)*(thePoint.z - splinePoints[i].z));
			if(temp_min_dist < min_distance)
			{
				min_distance = temp_min_dist;
				min_index = i;
			}
		}

		if(min_index > 0)// && min_distance < 100.00f)
		{
			unsigned int min_cubic_index = (min_index - 1)/mGranularity;
			float min_u = GetMinUFromLineAB(splinePoints[min_index-1], splinePoints[min_index], thePoint);
			int step_mod = (min_index)%mGranularity;

			float ret_dist = 0.0f;

			for(unsigned int i = 0; i < min_cubic_index; i++)
			{
				ret_dist += segmentLengths[i];
			}

			unsigned int start_segments = min_cubic_index*mGranularity;
			for(unsigned int i = start_segments + 1; i < start_segments+step_mod; i++)
			{
				ret_dist += (float)Mathf::Sqrt((splinePoints[i].x - splinePoints[i - 1].x)*(splinePoints[i].x - splinePoints[i - 1].x)
					+ (splinePoints[i].y-splinePoints[i-1].y)*(splinePoints[i].y-splinePoints[i-1].y)
					+ (splinePoints[i].z-splinePoints[i-1].z)*(splinePoints[i].z-splinePoints[i-1].z) );
			}

			ret_dist += (float)min_u * (float)Mathf::Sqrt((splinePoints[min_index].x - splinePoints[min_index - 1].x)*(splinePoints[min_index].x - splinePoints[min_index - 1].x)
				+ (splinePoints[min_index].y-splinePoints[min_index-1].y)*(splinePoints[min_index].y-splinePoints[min_index-1].y)
				+ (splinePoints[min_index].z-splinePoints[min_index-1].z)*(splinePoints[min_index].z-splinePoints[min_index-1].z) );

			return ret_dist;
		}
	}
	return 0.0f;
}

float Spline::GetMinDistanceFromLineAB(Vector3 A, Vector3 B, Vector3 C)
{
	float Bx_Ax = (float)(B.x - A.x);
	float By_Ay = (float)(B.y - A.y);
	float Bz_Az = (float)(B.z - A.z);

	float u = GetMinUFromLineAB(A,B,C);

	float x = (float)(A.x + u*Bx_Ax);
	float y = (float)(A.y + u*By_Ay);
	float z = (float)(A.z + u*Bz_Az);

	return Mathf::Sqrt((C.x - x)*(C.x - x) + (C.y - y)*(C.y - y) + (C.z - z)*(C.z - z));
}

float Spline::GetMinUFromLineAB(Vector3 A, Vector3 B, Vector3 C)
{
	float Bx_Ax = (float)(B.x - A.x);
	float Cx_Ax = (float)(C.x - A.x);
	float By_Ay = (float)(B.y - A.y);
	float Cy_Ay = (float)(C.y - A.y);
	float Bz_Az = (float)(B.z - A.z);
	float Cz_Az = (float)(C.z - A.z);

	float Bx_Ax_2 = (float)((Bx_Ax) * (Bx_Ax));
	float By_Ay_2 = (float)((By_Ay) * (By_Ay));
	float Bz_Az_2 = (float)((Bz_Az) * (Bz_Az));

	return (Cx_Ax*Bx_Ax + Cy_Ay*By_Ay + Cz_Az*Bz_Az)/(Bx_Ax_2 + By_Ay_2 + Bz_Az_2);
}


// Returns -1 if no point founds
int Spline::GetControlPointIdNear(Vector3 thePoint, const float& range)
{
	float min = range > 0.0f ? range * range * 10.0f : Mathf::MaxFloat;
	int index = -1;
	for(int i = 0; i < controlPoints.Length(); i++)
	{
		const float dist = (thePoint.x - controlPoints[i].x)*(thePoint.x - controlPoints[i].x) + (thePoint.y - controlPoints[i].y)*(thePoint.y - controlPoints[i].y) + (thePoint.z - controlPoints[i].z)*(thePoint.z - controlPoints[i].z);

		if( dist < min )
		{
			if(range > 0.0f)
			{
				if(dist > range * range) continue;
			}
			min = dist;
			index = i;
		}
	}

	return index;
}

// Returns -1 if No Segments Found
int Spline::GetSegmentIdNear(Vector3 thePoint)
{
	float min_distance = Mathf::MaxFloat;// Something Large
	int min_index = -1;

	for(int i = 0; i < splinePoints.Length(); i++)
	{
		float temp_min_dist = (float)Mathf::Sqrt((thePoint.x - splinePoints[i].x)*(thePoint.x - splinePoints[i].x) + (thePoint.y - splinePoints[i].y)*(thePoint.y - splinePoints[i].y) + (thePoint.z - splinePoints[i].z)*(thePoint.z - splinePoints[i].z));
		if(temp_min_dist < min_distance)
		{
			min_distance = temp_min_dist;
			min_index = i;
		}
	}

	if(min_index >= 0 && min_distance < 20.00f)
	{
		return min_index/mGranularity;
	}

	return -1;

}

// Returns the Coordinates of the Control Point Specified by the id
Vector3 Spline::GetControlPoint(int theControlPointId)
{
	if(theControlPointId < (int)controlPoints.Length())
		return controlPoints[theControlPointId];

	return Vector3(0,0,0);
}

// Used to Move Control Points and Adjust the Spline.  Use 'GetControlPointIdNear' to get a valid ID
void Spline::SetControlPoint(int theControlPointId, Vector3 thePoint)
{
	if(theControlPointId >= 0 && theControlPointId < (int)controlPoints.Length())
	{
		controlPoints[theControlPointId] = thePoint;
		xCoords[theControlPointId] = (float)thePoint.x;
		yCoords[theControlPointId] = (float)thePoint.y;
		zCoords[theControlPointId] = (float)thePoint.z;

		RegenerateSplines();
	}
}

// Call this To Add a Control Point at the Mid-way point of the cubic
// addressed by theSplineSegmentId
void Spline::BisectSegment(int theSplineSegmentId)
{
	if(theSplineSegmentId >= 0 && theSplineSegmentId < (int)xCubics.Length())
	{
		const int bpx = (int)xCubics[theSplineSegmentId].eval(0.5f);
		const int bpy = (int)yCubics[theSplineSegmentId].eval(0.5f);
		const int bpz = (int)zCubics[theSplineSegmentId].eval(0.5f);
		Vector3 aBisectingPoint((float)bpx, (float)bpy, (float)bpz);

		Array<Vector3> aStack;

		for(int i = (int)controlPoints.Length()-1; i > theSplineSegmentId; i--)
		{
			aStack.Add(controlPoints.Last());
			controlPoints.Pop();
			xCoords.Pop();
			yCoords.Pop();
			zCoords.Pop();
		}

		AddPoint(aBisectingPoint);

		while(!aStack.IsEmpty())
		{
			AddPoint(aStack.Last());
			aStack.Pop();
		}

		RegenerateSplines();
	}
}

void Spline::DeleteControlPoint(int theControlPointId)
{
	if(theControlPointId >= 0 && theControlPointId < (int)controlPoints.Length())
	{
		Array<Vector3> aStack;
		for(int i = (int)controlPoints.Length() - 1; i >= 0; i--)
		{
			if((int) i != theControlPointId)
				aStack.Add(controlPoints[i]);
		}

		ClearAllPoints();

		while(!aStack.IsEmpty())
		{
			AddPoint(aStack.Last());
			aStack.Pop();
		}

		RegenerateSplines();
	}
}

void Spline::ClearAllPoints()
{
	controlPoints.SetLength(0);
	xCoords.SetLength(0);
	yCoords.SetLength(0);
	zCoords.SetLength(0);

	mArcLength = 0.0f;

	RegenerateSplines();
}

void Spline::Vibrate(float step, float amplitude, bool random)
{
	Array<Vector3> newpoints;

	float len = 0.0f;
	int index = 0;

	while(len < GetArcLength())
	{
		Vector3 p2 = GetTangentAt(len);
		Vector3 p3 = Vector3(p2.x, p2.y, p2.z); p3.Normalize();
		Vector3 right = Vector3::Cross(&p3,&Vector3::UnitY);

		float f = random ? (float)Mathf::Rand(amplitude) : amplitude;
		if(index % 2) f *= -1;

		p2 = GetPointAt(len);
		p3 = Vector3(p2.x, p2.y, p2.z) + right * f;
		
		newpoints.Add(Vector3(p3.x, p3.y, p3.z));
		
		len += step;
		index++;
	}

	if((index - 1) * step < step * 0.75f)
		newpoints.Add(GetPointAt(GetArcLength()));

	ClearAllPoints();

	for(int i = 0; i< newpoints.Length(); i++)
		AddPoint(newpoints[i], false);

	RegenerateSplines();
}

void Spline::MakeSplineSegmentLengthsCummulative()
{
	segmentLengthsCummulative.Clear();
	float len = 0;
	for(int i = 0; i < segmentLengths.Length(); i++)
	{
		segmentLengthsCummulative.Add(len);
		len += segmentLengths[i];
	}
}

Vector3 Spline::GetPointAtNormalized( float normalizedDistance ) const
{
	return GetPointAt(normalizedDistance * GetArcLength());
}

Vector3 Spline::GetTangentAtNormalized( float normalizedDistance ) const
{
    return GetTangentAt(normalizedDistance * GetArcLength());
}

void Spline::GetPointAndTangentPreciseAt(float dist,Vector3 &point,Vector3 &tangent)
{
	point = Vector3(0.0f, 0.0f, 0.0f);
	tangent = Vector3(0.0f, 0.0f, 0.0f);

	if(controlPoints.Length() == 0) return;
	if(splinePoints.Length() == 0) return;
	
	float aDistance = NormalizeDistanceOnSpline(dist);
	int anIndex = 0;

	int start = 0;
	int end = splineDistances.Length();
	int middle = (end + start) / 2;
	while(!(aDistance >= splineDistances[middle] && aDistance <= splineDistances[middle + 1]))
	{
		if(aDistance >= splineDistances[middle])
		{
			start = middle;
		}
		else
		{
			end = middle;
		}

		middle = (end + start) / 2;

		if(middle == 0 || middle == splineDistances.Length() - 1)
		{
			break;
		}
	}

	anIndex = middle;
	aDistance -= splineDistances[middle];

    if (anIndex == (splineDistances.Length() - 1))
    {
        point = splinePoints[anIndex];
        tangent = GetTangentAtNormalized(1.0f);
    }
    else
    {
        Vector3::Lerp(&splinePoints[anIndex],&splinePoints[anIndex + 1],aDistance / (splineDistances[anIndex + 1] - splineDistances[anIndex]),&point);

        float invLen = 1.0f / (splineDistances.Length() - 1);
		float u = Mathf::Lerp(invLen * anIndex, invLen * (anIndex + 1), aDistance / (splineDistances[anIndex + 1] - splineDistances[anIndex]));
        tangent = GetTangentAtNormalized(u);
    }
}

Vector3 Spline::GetPointPreciseAt(float dist)
{
    Vector3 point,tangent;
    GetPointAndTangentPreciseAt(dist,point,tangent);
    return point;
}

Vector3 Spline::GetTangentPreciseAt(float dist)
{
	//todo testing
    Vector3 point,tangent;
    GetPointAndTangentPreciseAt(dist,point,tangent);
    return tangent;
}

float Spline::GetNearestPointPreciseDist(const Vector3 &point)
{
    //todo optimize
    int best;
    float dist,t;
    float bestDist = 1e30f,bestT;

    int len1 = (splinePoints.Length() - 1);
    for (int a = 0;a < len1;a += 1)
    {
        Intersection::DistancePointToLine(splinePoints[a], splinePoints[a + 1], point, dist, t);
        if (dist < bestDist)
        {
            bestDist = dist;
            bestT = t;
            best = a;
        }
    }

	bestT = Mathf::Max(0.0f, bestT);
	bestT = Mathf::Min(1.0f, bestT);

    return splineDistances[best] + bestT * Vector3::Distance(splinePoints[best], splinePoints[best + 1]);
}
