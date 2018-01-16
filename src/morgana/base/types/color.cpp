#include "color.h"
#include "../math/memath.h"

using namespace MorganaEngine::Base::Types;
using namespace MorganaEngine::Base::Math;

const Color Color::white = Color(1.0f, 1.0f, 1.0f);
const Color Color::black = Color(0.0f, 0.0f, 0.0f);
const Color Color::red = Color(1.0f, 0.0f, 0.0f);
const Color Color::green = Color(0.0f, 1.0f, 0.0f);
const Color Color::blue = Color(0.0f, 0.0f, 1.0f);
const Color Color::yellow = Color(1.0f, 1.0f, 0.0f);
const Color Color::cyan = Color(0.0f, 1.0f, 1.0f);
const Color Color::magenta = Color(1.0f, 0.0f, 1.0f);
const Color Color::gray = Color(0.5f, 0.5f, 0.5f);
const Color Color::dark_slate_gray = Color(0.2f, 0.3f, 0.3f, 1.0f);
const Color Color::dim_gray = Color(0.4f, 0.4f, 0.4f, 1.0f);
const Color Color::slate_gray = Color(0.439f, 0.54f, 0.56f, 1.0f);
const Color Color::brown = Color(0.36f, 0.255f, 0.2f);
const Color Color::orange = Color(1.0f, 0.5f, 0.0f);
const Color Color::burlywood = Color(0.87f, 0.721f, 0.53f);
const Color Color::silver = Color(0.75f, 0.75f, 0.75f, 1.0f);
const Color Color::light_gray = Color(0.82f, 0.82f, 0.82f, 1.0f);
const Color Color::transparent = Color(0.0f, 0.0f, 0.0f, 0.0f);
const Color Color::normal_up = Color(0.5f, 1.0f, 0.5f);

Color::Color() { r = g = b = a = 1.0f;}
Color::Color(const float& r, const float& g, const float& b, const float& a) { this->r = r; this->g = g; this->b = b; this->a = a; }
Color::Color(const float& r, const float& g, const float& b) { this->r = r; this->g = g; this->b = b; this->a = 1.0f; }
Color::Color(const float* c) { r = c[0]; g = c[1]; b = c[2]; a = c[3]; }
Color::Color(const Color& c)
{
	r = c.r;
	g = c.g;
	b = c.b;
	a = c.a;
}

Color::Color(const Color255& c)
{
	*this = c;
}

Color::Color(const int hexa) //ARGB
{ 
	*this = Color255(hexa);
}

void Color::SetARGB(int argbColor)
{
	Color255 c; c.SetARGB(argbColor);
	*this = c;
}

int Color::GetARGB() const
{
	Color255 c;
	c.FromFloat(r, g, b, a);
	return c.GetARGB();
}

Color& Color::operator = (Color const & c)
{
	memcpy(&r, &c.r, 4 * sizeof(float));
    return *this;
}

Color& Color::operator = (Color255 const & c)
{
	c.ToFloat(r, g, b, a);
	return *this;
}

Color Color::operator + (Color const & c) const
{   
    return Color(c.r + r, c.g + g, c.b + b, c.a + a);
}

Color Color::operator - (Color const & c) const
{
	return Color(r - c.r, g - c.g, b - c.b, a - c.a);
}

Color Color::operator * (Color const & c) const
{   
    return Color(r * c.r, g * c.g, b * c.b, a * c.a);
}

Color Color::operator * (float f)
{
    return Color(r * f, g * f, b * f, a * f);
}

Color Color::operator / (float f)
{
	return Color(r / f, g / f, b / f, a / f);
}

Color::operator float* ()
{
    return &r;
}

Color::operator const Vector4() const
{
	Vector4 v;
	v.x = r;
	v.y = g;
	v.z = b;
	v.w = a;
	return v;
}

Color::operator const Color255() const
{
	return Color255((byte)(r * 255.0f), (byte)(g * 255.0f), (byte)(b * 255.0f), (byte)(a * 255.0f));
}

bool Color::operator == (Color const & c) const
{
	return memcmp(&r, &c.r, 4 * sizeof(float)) == 0;
}

bool Color::operator != (Color const & c) const
{
	return memcmp(&r, &c.r, 4 * sizeof(float)) != 0;
}

Color& Color::operator += (Color const & c)
{
	r += c.r;
	g += c.g;
	b += c.b;
	a += c.a;

    return *this;
}

const bool Color::IsWhite() const
{
	return r == 1.0f && g == 1.0f && b == 1.0f && a == 1.0f;
}

Color Color::Lerp(const Color& c1, const Color& c2, const float& f)
{
	return (c2 - c1) * f + c1;
}

