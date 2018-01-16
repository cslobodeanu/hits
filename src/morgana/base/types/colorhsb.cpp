#include "colorhsb.h"

using namespace MorganaEngine::Base::Types;

ColorHSB::ColorHSB()
{
	hue = saturation = brightness = 1.0f;
}


ColorHSB::ColorHSB(const ColorHSB& other)
{
	hue = other.hue;
	saturation = other.saturation;
	brightness = other.brightness;
}

ColorHSB::ColorHSB(float hue, float saturation, float brightness)
{
	this->hue = hue;
	this->saturation = saturation;
	this->brightness = brightness;
}

ColorHSB::ColorHSB(const Color255& other)
{
	float fr = (float)other.r / 255.0f;
	float fg = (float)other.g / 255.0f;
	float fb = (float)other.b / 255.0f;

	FromRGBFloat(fr, fg, fb);
}

ColorHSB::ColorHSB(const Color& other)
{
	FromRGBFloat(other.r, other.g, other.b);
}

ColorHSB::operator const Color() const
{
	Color c;
	c.a = 1.0f;
	ToRGBFloat(c.r, c.g, c.b);
	return c;
}

ColorHSB::operator const Color255() const
{
	float fr, fg, fb;
	ToRGBFloat(fr, fg, fb);
	return Color255::FromFloat(fr, fg, fb);
}

const void ColorHSB::FromRGBFloat(float fr, float fg, float fb)
{
	const float max = Mathf::Max(Mathf::Max(fr, fg), fb);
	const float min = Mathf::Min(Mathf::Min(fr, fg), fb);

	float delta;

	brightness = max;				// v

	delta = max - min;

	if (max != 0)
		saturation = delta / max;		// s
	else {
		// fr = fg = fb = 0		// s = 0, v is undefined
		saturation = 0;
		hue = 0.0f;
		return;
	}

	if (fr == max)
		hue = (fg - fb) / delta;		// between yellow & magenta
	else if (fg == max)
		hue = 2 + (fb - fr) / delta;	// between cyan & yellow
	else
		hue = 4 + (fr - fg) / delta;	// between magenta & cyan

	hue /= 6.0f;				// degrees
	if (hue < 0.0f)
		hue += 1.0f;

	if (saturation == 0.0f)
		hue = 0.0f;
}

const void ColorHSB::ToRGBFloat(float& fr, float& fg, float& fb) const
{
	if (saturation == 0.0f)
	{
		fr = brightness;
		fg = brightness;
		fb = brightness;
		return;
	}


	float h = hue * 6.0f;
	int i = (int)Mathf::Floor(h);
	float f = h - i;
	float p = brightness * (1 - saturation);
	float q = brightness * (1 - saturation * f);
	float t = brightness * (1 - saturation * (1 - f));

	switch (i)
	{
	case 0:
		fr = brightness;
		fg = t;
		fb = p;
		break;

	case 1:
		fr = q;
		fg = brightness;
		fb = p;
		break;

	case 2:
		fr = p;
		fg = brightness;
		fb = t;
		break;

	case 3:
		fr = p;
		fg = q;
		fb = brightness;
		break;

	case 4:
		fr = t;
		fg = p;
		fb = brightness;
		break;

	default:        // case 5:
		fr = brightness;
		fg = p;
		fb = q;
		break;
	}
}

ColorHSB ColorHSB::RandomHue()
{
	ColorHSB hsb;
	hsb.hue = Mathf::Rand01();
	return hsb;
}