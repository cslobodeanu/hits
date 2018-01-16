#include "mathutils.h"

#include <math.h>
#include "../compatibility/compatibility.h"
#include <stdio.h>
#include <limits.h>

#include <time.h>

using namespace MorganaEngine::Base::Math;

const float	Mathf::E			= 2.718281828459045f;
const float	Mathf::Log2E		= 1.442695040888963f;
const float	Mathf::Log10E		= 0.434294481903252f;

const float	Mathf::Pi			= 3.141592653589793f;
const float	Mathf::PiOver2		= 1.570796326794897f;
const float	Mathf::PiOver3		= 1.047197551196598f;
const float	Mathf::PiOver4		= 0.785398163397448f;
const float	Mathf::PiOver6		= 0.523598775598299f;
const float	Mathf::TwoPi		= 6.283185307179586f;
const float	Mathf::TwoPiOver3	= 2.094395102393195f;
const float	Mathf::FourPiOver3	= 4.188790204786391f;
const float Mathf::Sqrt2        = 1.414213562373095f;

const float Mathf::MaxFloat         =  1E+37f;
const float	Mathf::MinFloat         = -1E+37f;
const float	Mathf::MinPositiveFloat	=  1E-37f;
const float	Mathf::MaxNegativeFloat	= -1E-37f;
const float	Mathf::Epsilon			= 0.00000001f;

const unsigned int		Mathf::MaxUint		= 4294967295u;
const unsigned int		Mathf::MinUint		= 0u;
const unsigned short	Mathf::MaxUshort	= 65535u;
const unsigned short	Mathf::MinUshort	= 0u;
const unsigned char		Mathf::MaxUchar		= 255u;
const unsigned char		Mathf::MinUchar		= 0u;

const int				Mathf::MaxInt		= +2147483647;
const int				Mathf::MinInt		= -2147483647;
const short				Mathf::MaxShort		= +32767;
const short				Mathf::MinShort		= -32767;
const char				Mathf::MaxChar		= +127;
const char				Mathf::MinChar		= -127;

float	Mathf::Ceil(const float val)					{ return ceilf(val); }
int		Mathf::CeilToInt(const float val)				{ return (int)ceilf(val); }
float	Mathf::Floor(const float val)					{ return floorf(val); }
int		Mathf::FloorToInt(const float val)				{ return (int)floorf(val); }
float	Mathf::Round(const float val)					{ return (float)roundf(val); }
int		Mathf::RoundToInt(const float val)				{ return (int)roundf(val); }
float	Mathf::Trunc(const float val)					{ return truncf(val); }

float	Mathf::Pow(const float x, const float y)		{ return powf(x, y); }
float	Mathf::Exp(const float power)					{ return expf(power); }
float	Mathf::Expm1(const float power)					{ return expm1f(power); }
float	Mathf::Log(const float val)						{ return logf(val); }
float	Mathf::Log(const float val, const float base)	{ return logf(val) / logf(base); }
float	Mathf::Log2(const float val)					{ return log2f(val); }
float	Mathf::Log10(const float val)					{ return log10f(val); }

class __RandomNumberGenerator
{
private:
	static unsigned int m_w;
	static unsigned int m_z;


public:
	static unsigned int max;
	// The random generator seed can be set three ways:
	// 1) specifying two non-zero unsigned integers
	// 2) specifying one non-zero unsigned integer and taking a default value for the second
	// 3) setting the seed from the system time
	static void SetSeed(unsigned int u, unsigned int v);

	static void SetSeed(unsigned int u);

	static void SetSeedFromSystemTime();

	static float GetUniform();

	static unsigned int GetUInt();


};


void Mathf::SeedRand(unsigned int seed)
{
	__RandomNumberGenerator::SetSeed(seed);
}

unsigned int Mathf::Rand()
{
	return __RandomNumberGenerator::GetUInt();
}

float Mathf::Rand(const float min, const float max)
{
	return ((max - min) * float(Rand()) / float(__RandomNumberGenerator::max)) + min; 
}

float Mathf::Rand(const float max)
{
	return max * float(Rand()) / float(__RandomNumberGenerator::max);
}

float Mathf::Rand01()
{
	return float(Rand()) / float(__RandomNumberGenerator::max);
}

float Mathf::Rand11()
{
	return (2.f * float(Rand()) / float(__RandomNumberGenerator::max)) - 1.f;
}

float Mathf::RandNNInt(const float min, const float max)
{
	float v = Rand01() * (max-min) + min;

	if (Rand() & 0x0001)
		return v;
	else
		return -v;
}

float Mathf::RandAngle()
{
	return (TwoPi * float(Rand()) / float(__RandomNumberGenerator::max)) - Pi;
}

float Mathf::RandNN(const float n)
{
	return n * ((2.f * float(Rand()) / float(__RandomNumberGenerator::max)) - 1.f);
}

float Mathf::RandAround( const float center, const float radius )
{
	return Rand(center - radius, center + radius);
}

float Mathf::RandAroundFactor( const float center, const float radiusFactor )
{
	return RandAround(center, center * radiusFactor);
}

float Mathf::Sqrt(const float val)
{
	const float halfx = 0.5f * val;
	float _x;

	int carmack = *(const int *) &val;
	carmack = 0x5F3759DF - (carmack >> 1);
	_x = *(float *) &carmack;

	_x = _x * (1.5f - halfx * _x * _x);
	_x = _x * (1.5f - halfx * _x * _x);
	return _x * val;
}

float Mathf::Rsqrt(const float val)
{
	const float halfx = 0.5f * val;
	float _x;

	int carmack = *(const int *) &val;
	carmack = 0x5F3759DF - (carmack >> 1);
	_x = *(float *) &carmack;

	_x = _x * (1.5f - halfx * _x * _x);
	_x = _x * (1.5f - halfx * _x * _x);
	return _x;
}

float Mathf::Square(const float val)
{
	return val*val;
}

float	Mathf::Sin(const float rad)						{ return sinf(rad); }
float	Mathf::Cos(const float rad)						{ return cosf(rad); }
float	Mathf::Tan(const float rad)						{ return tanf(rad); }
float	Mathf::Asin(const float val)						{ return asinf(val); }
float	Mathf::Acos(const float val)						{ return acosf(val); }
float	Mathf::Atan(const float val)						{ return atanf(val); }
float	Mathf::Atan2(const float y, const float x)		{ return atan2f(y, x); }

float	Mathf::Abs(const float val)						{ return fabsf(val); }
int		Mathf::Abs(const int val)						{ return abs(val); }


unsigned int __RandomNumberGenerator::m_w = 521288629;
unsigned int __RandomNumberGenerator::m_z = 362436069;
unsigned int __RandomNumberGenerator::max = UINT_MAX;


// The random generator seed can be set three ways:
// 1) specifying two non-zero unsigned integers
// 2) specifying one non-zero unsigned integer and taking a default value for the second
// 3) setting the seed from the system time

void __RandomNumberGenerator::SetSeed(unsigned int u, unsigned int v)
{
	if (u != 0) m_w = u; 
	if (v != 0) m_z = v;
}

void __RandomNumberGenerator::SetSeed(unsigned int u)
{
	m_w = 362436069;
	m_z = u+1;
}

void __RandomNumberGenerator::SetSeedFromSystemTime()
{
	long x = (long) time(NULL);
	SetSeed((unsigned int)(x >> 16), (unsigned int)(x % 4294967296));
}

// Produce a uniform random sample from the open interval (0, 1).
// The method will not return either end point.
float __RandomNumberGenerator::GetUniform()
{
	// 0 <= u < 2^32
	unsigned int u = GetUInt();
	// The magic number below is 1/(2^32 + 2).
	// The result is strictly between 0 and 1.
	return (float)((u + 1.0f) * 2.328306435454494e-10);
}


unsigned int __RandomNumberGenerator::GetUInt()
{
	m_z = 36969 * (m_z & 65535) + (m_z >> 16);
	m_w = 18000 * (m_w & 65535) + (m_w >> 16);
	return (m_z << 16) + m_w;
}