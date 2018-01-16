#include "color255.h"
#include "../math/memath.h"

using namespace MorganaEngine::Base::Types;
using namespace MorganaEngine::Base::Math;

const Color255 Color255::white = Color255(255, 255, 255);
const Color255 Color255::black = Color255(0, 0, 0);
const Color255 Color255::red = Color255(255, 0, 0);
const Color255 Color255::green = Color255(0, 255, 0);
const Color255 Color255::blue = Color255(0, 0, 255);
const Color255 Color255::yellow = Color255(255, 255, 0);
const Color255 Color255::cyan = Color255(0, 255, 255);
const Color255 Color255::magenta = Color255(255, 0, 255);
const Color255 Color255::gray = Color255(128, 128, 128);
const Color255 Color255::dark_slate_gray = Color255(49, 79, 79, 255);
const Color255 Color255::dim_gray = Color255(105, 105, 105, 255);
const Color255 Color255::slate_gray = Color255(112, 138, 144, 255);
const Color255 Color255::brown = Color255(92, 64, 51);
const Color255 Color255::orange = Color255(255, 127, 0);
const Color255 Color255::burlywood = Color255(222, 184, 135);
const Color255 Color255::transparent = Color255(0, 0, 0, 0);
const Color255 Color255::normal_up = Color255(128, 255, 128);

#define ALPHA_CHANNEL(color) ((color & 0xFF000000) >> 24)
#define RED_CHANNEL(color)   ((color & 0x00FF0000) >> 16)
#define GREEN_CHANNEL(color) ((color & 0x0000FF00) >> 8)
#define BLUE_CHANNEL(color)  ((color & 0x000000FF) >> 0)

Color255::Color255() { r = g = b = a = 255;}
Color255::Color255(const byte& r, const byte& g, const byte& b, const byte& a) { this->r = r; this->g = g; this->b = b; this->a = a; }
Color255::Color255(const byte& r, const byte& g, const byte& b) { this->r = r; this->g = g; this->b = b; this->a = 255;}
Color255::Color255(const byte* c) { r = c[0]; g = c[1]; b = c[2]; a = c[3];}

Color255::Color255(const Color255& c)
{
	//*((unsigned int*)&r) = *((unsigned int*)&c.r);
	r = c.r;
	g = c.g;
	b = c.b;
	a = c.a;
}

Color255::Color255(const int hexa) //ARGB
{ 
	a = (ALPHA_CHANNEL(hexa));
	r = (RED_CHANNEL(hexa));
	g = (GREEN_CHANNEL(hexa));
	b = (BLUE_CHANNEL(hexa));
}

Color255 Color255::FromFloat(const float& fr, const float& fg, const float& fb, const float& fa /*=1.0*/)
{
	Color255 c;
    c.r = (byte)(fr * 255.0f);
	c.g = (byte)(fg * 255.0f);
	c.b = (byte)(fb * 255.0f);
	c.a = (byte)(fa * 255.0f);

	return c;
}

#define B2F(x) ((float)x * 0.003921568627451f)

const void Color255::ToFloat(float& fr, float& fg, float& fb, float& fa) const 
{
    fr = B2F(r);
    fg = B2F(g);
    fb = B2F(b);
    fa = B2F(a);
}

void Color255::SetARGB(int argbColor)
{
	a = (ALPHA_CHANNEL(argbColor));
	r = (RED_CHANNEL(argbColor));
	g = (GREEN_CHANNEL(argbColor));
	b = (BLUE_CHANNEL(argbColor));
}

int Color255::GetARGB() const
{
	return ((a&0xFF)<<24) |  ((r&0xFF)<<16) |  ((g&0xFF)<<8) |  ((b&0xFF)<<0);
}

Color255& Color255::operator = (Color255 const & c)
{
	r = c.r;
	g = c.g;
	b = c.b;
	a = c.a;
	//*((unsigned int*)&r) = *((unsigned int*)&c.r);
    return *this;
}

/*Color255& Color255::operator = (Vector3 const & v3)
{
	FromFloat(v3.x, v3.y, v3.z, 1.0f);
	return *this;
}

Color255& Color255::operator = (Vector4 const & v4)
{
	FromFloat(v4.x, v4.y, v4.z, v4.w);
	return *this;
}*/

Color255 Color255::operator + (Color255 const & c) const
{
	Color255 cc;
    cc.r = c.r + r;
	cc.g = c.g + g;
	cc.b = c.b + b;
	cc.a = c.a + a;
    
    return cc;
}

Color255 Color255::operator * (Color255 const & c) const
{
    uint rr = (c.r * r) >> 8;
    uint gg = (c.g * g) >> 8;
    uint bb = (c.b * b) >> 8;
    uint aa = (c.a * a) >> 8;
    
    return Color255((byte)rr, (byte)gg, (byte)bb, (byte)aa);
}

Color255 Color255::operator * (float f) const
{
	Color255 c;
	c.r = (int)((float)r * f);
	c.g = (int)((float)g * f);
	c.b = (int)((float)b * f);
	c.a = (int)((float)a * f);

	return c;
}

Color255::operator byte* ()
{
    return &r;
}

Color255::operator const Vector4() const
{
	Vector4 v;
	v.x = B2F(r);
	v.y = B2F(g);
	v.z = B2F(b);
	v.w = B2F(a);
	return v;
}

/*
Color255::operator const Vector3() const
{
	Vector3 v;
	v.x = B2F(r);
	v.y = B2F(g);
	v.z = B2F(b);
	return v;
}
*/

bool Color255::operator == (Color255 const & c) const
{
	return r == c.r && g == c.g && b == c.b && a == c.a;
}

bool Color255::operator != (Color255 const & c) const
{
	return r != c.r || g != c.g || b != c.b || a != c.a;
}

Color255& Color255::operator += (Color255 const & c)
{
	r += c.r;
	g += c.g;
	b += c.b;
	a += c.a;

    return *this;
}

bool Color255::IsWhite()
{
    const unsigned int* l = (unsigned int*)&r;
    return *l == 0xffffffff;
}

Color255 Color255::Lerp(const Color255& c1, const Color255& c2, const float& f)
{
    int r = (int)((c2.r - c1.r) * f) + c1.r;
    int g = (int)((c2.g - c1.g) * f) + c1.g;
    int b = (int)((c2.b - c1.b) * f) + c1.b;
    int a = (int)((c2.a - c1.a) * f) + c1.a;
    return Color255(r, g, b, a);
}

void Color255::Saturate()
{
	r = CLAMP255(r);
	g = CLAMP255(g);
	b = CLAMP255(b);
	a = CLAMP255(a);
}
