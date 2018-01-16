#ifndef __MORGANA_BASE_TYPES_COLOR255_H__
#define __MORGANA_BASE_TYPES_COLOR255_H__

#include "../compatibility/compatibility.h"
#include "../math/vector4.h"
#include "../math/vector3.h"

namespace MorganaEngine
{
	namespace Base
	{
		using namespace Math;
		namespace Types
		{
			class Color255
			{

			public:
				byte r,g,b,a;

				Color255();
				Color255(const byte& r, const byte& g, const byte& b, const byte& a);
				Color255(const byte& r, const byte& g, const byte& b);
				Color255(const byte* c);
				Color255(const Color255& c);
				Color255(const int hexa);

				static Color255 FromFloat(const float& fr, const float& fg, const float& fb, const float& fa = 1.0f);
            
				const void ToFloat(float& fr, float& fg, float& fb, float& fa) const;
				void SetARGB(int argbColor);
				int GetARGB() const;

				Color255& operator = (Color255 const & c);
				//Color255& operator = (Vector3 const & v3);
				//Color255& operator = (Vector4 const & v4);
            
				Color255 operator + (Color255 const & c) const;

				Color255 operator * (Color255 const & c) const;
            
				Color255 operator * (float f) const;
            
				operator byte* ();
				operator const Math::Vector4 () const;
				//operator const Math::Vector3 () const;
            
				bool operator == (Color255 const & c) const;
            
				bool operator != (Color255 const & c) const;
            
				Color255& operator += (Color255 const & c);
            
				bool IsWhite();
            
				static Color255 Lerp(const Color255& c1, const Color255& c2, const float& f);

				void			Saturate();
            
				static const Color255 white;
				static const Color255 black;
				static const Color255 red;
				static const Color255 green;
				static const Color255 blue;
				static const Color255 yellow;
				static const Color255 cyan;
				static const Color255 magenta;
				static const Color255 gray;		 
				static const Color255 dark_slate_gray;			 
				static const Color255 dim_gray;			 
				static const Color255 slate_gray;
				static const Color255 brown;
				static const Color255 orange;
				static const Color255 burlywood;
				static const Color255 transparent;
				static const Color255 normal_up;
			};
		}
	}
}
#endif