#ifndef __MORGANA_BASE_MATH_CURVE_H__
#define __MORGANA_BASE_MATH_CURVE_H__

#include "vector2.h"
#include "mearray.h"

namespace MorganaEngine
{
	namespace Base
	{
		using namespace Types;
		namespace Math
		{
			class Curve
			{
			public:
				Curve(const int res = 256);
				virtual ~Curve();

				void SetPoint(const float t, const float y);
				const float GetPoint(const float t);

				Array<Vector2>	GetControlPoints();

			protected:
				Array<Vector2>	controlPoints;
				Array<float>	points;
				void			ZeroPoints();
				void			Regenerate();
			};
		}
	}
}

#endif