#ifndef __MORGANA_BASE_TYPES_COLOR_H__
#define __MORGANA_BASE_TYPES_COLOR_H__

#include "../compatibility/compatibility.h"
#include "../math/vector4.h"
#include "../math/vector3.h"
#include "color255.h"

namespace MorganaEngine
{
	namespace Base
	{
		using namespace Math;

		namespace Types
		{
			/*
				This class implements a RGBA color, together with basic operations: copy, add, interpolate, convert from/to HSB.
				Also there are some constants for the most used colors.
				*/
			class Color
			{
			public:
				float r, g, b, a;

				Color();
				Color(const float& r, const float& g, const float& b, const float& a);
				Color(const float& r, const float& g, const float& b);
				Color(const float* c);
				Color(const Color& c);
				Color(const Color255& c);
				Color(const int hexa);

				void SetARGB(int argbColor);
				int GetARGB() const;

				Color& operator = (Color const & c);
				Color& operator = (Color255 const & c);
				Color& operator = (Vector3 const & v3);
				Color& operator = (Vector4 const & v4);

				Color operator + (Color const & c) const;
				Color operator - (Color const & c) const;

				Color operator * (Color const & c) const;

				Color operator * (float f);
				Color operator / (float f);

				void operator *= (float val)
				{
					r *= val;
					g *= val;
					b *= val;
					a *= val;
				}

				operator float* ();
				operator const Math::Vector4() const;
				operator const Color255() const;
				//operator const Math::Vector3 () const;

				inline const Color255 ToColor255() const
				{
					return *this;
				}

				bool operator == (Color const & c) const;

				bool operator != (Color const & c) const;

				Color& operator += (Color const & c);

				const bool		IsWhite() const;

				static Color	Lerp(const Color& c1, const Color& c2, const float& f);

				static const Color white;
				static const Color black;
				static const Color red;
				static const Color green;
				static const Color blue;
				static const Color yellow;
				static const Color cyan;
				static const Color magenta;
				static const Color gray;
				static const Color dark_slate_gray;
				static const Color dim_gray;
				static const Color slate_gray;
				static const Color brown;
				static const Color orange;
				static const Color burlywood;
				static const Color transparent;
				static const Color silver;
				static const Color light_gray;
				static const Color normal_up;
			};

			inline Color operator + (const Color &c1, const float val)		{ return Color(c1.r + val, c1.g + val, c1.b + val, c1.a); }
			inline Color operator + (const float val, const Color &c1)		{ return Color(c1.r + val, c1.g + val, c1.b + val, c1.a); }
			inline Color operator - (const Color &c1, const float val)		{ return Color(c1.r - val, c1.g - val, c1.b - val, c1.a); }
			inline Color operator - (const float val, const Color &c1)		{ return Color(val - c1.r, val - c1.g, val - c1.b, c1.a); }
		}
	}
}
#endif