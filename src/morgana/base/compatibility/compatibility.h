#ifndef __MORGANA_BASE_COMPATIBILITY_COMPATIBILITY_H_INCLUDED__
#define __MORGANA_BASE_COMPATIBILITY_COMPATIBILITY_H_INCLUDED__

#include "platform.h"

//// STRINGS
#ifdef OS_WINDOWS_WIN32

#include <cmath>

#define b_strcpy(dest, sz_dest, src) strcpy_s(dest, sz_dest, src)
#define b_vsprintf(dest, sz_dest, ...) vsprintf_s(dest, sz_dest, __VA_ARGS__)
#define b_vswprintf(dest, sz_dest, ...) vswprintf_s(dest, sz_dest, __VA_ARGS__)
#if _MSC_FULL_VER == 180020617
#define snprintf _snprintf_s
#endif
#else
#define b_strcpy(dest, sz_dest, src) strcpy(dest, src)
#define b_vsprintf(dest, sz_dest, ...) vsprintf(dest, __VA_ARGS__)
#define b_vswprintf(dest, sz_dest, ...) vswprintf(dest, __VA_ARGS__)
#endif


#ifdef OS_WINDOWS

#define ulong		unsigned long
#define ushort		unsigned short

#define uint		unsigned int //av check
#define int32		long
#define uint32		unsigned long

#define uint8		unsigned char
#define int8		char

#define int16		short
#define uint16		unsigned short

typedef __int64					int64;
typedef unsigned __int64		uint64;

#define copysign _copysign

#endif

typedef unsigned char byte;

inline unsigned char CLAMP255(int a)
{
	if(a&(~0xFF)) return (-a)>>31;
	else return (unsigned char)a;
}

#include <math.h>

inline double roundf(double x) { return floor(x + 0.5); }
inline float truncf(float x) { return ((float)((int)x));}
inline float expm1f(float x) { return (float)(exp(x)-1.f); }
inline float exp2f(float x) { return powf(2, x); } //this can be taken from FreeBS// 
#define log2f(x) (logf(x)*1.4426950408889634f)

#define null NULL

#endif