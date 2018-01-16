#ifndef __MORGANA_BASE_TYPES_COLORHSB_H__
#define __MORGANA_BASE_TYPES_COLORHSB_H__

#include "color.h"
#include "color255.h"

namespace MorganaEngine
{
	namespace Base
	{
		using namespace Math;
		namespace Types
		{
			class ColorHSB
			{

			public:
				float hue, saturation, brightness;

				ColorHSB();
				ColorHSB(const ColorHSB& other);
				ColorHSB(float hue, float saturation, float brightness);
				ColorHSB(const Color255& other);
				ColorHSB(const Color& other);

				operator const Color() const;
				operator const Color255() const;

				static ColorHSB	RandomHue();

			private:
				const void FromRGBFloat(float fr, float fg, float fb);
				const void ToRGBFloat(float& fr, float& fg, float& fb) const;
			};
		}
	}
}
#endif