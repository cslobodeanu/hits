#ifndef __NW_POINT_H__
#define __NW_POINT_H__

#include <math.h>

namespace MorganaEngine
{
	namespace Base
	{
		namespace Types
		{
			template <class T> 
			struct mPointT
			{
				union
				{
					struct
					{
						T x, y;
					};
					struct
					{
						T Left, Top;
					};
				};

				inline mPointT<T>()
				{
					x = y = 0;
				}

				mPointT<T>(const T& xx, const T& yy)
				{
					x = xx;
					y = yy;
				}

				mPointT<T>(const mPointT<T>& other)
				{
					x = other.x;
					y = other.y;
				}

			
				mPointT<T>& operator = (const mPointT<T>& p)
				{
					x = p.x;
					y = p.y;
					return *this;
				}

				bool operator == (const mPointT<T>& p) const
				{
					return p.x == x && p.y == y;
				}

				T Distance(const mPointT<T>& p) const
				{
					return (T)sqrtf((float)(p.x - x) * (p.x - x) + (float)(p.y - y) * (p.y - y));
				}

				T DistanceSquared(mPointT<T> p)
				{
					return (T)((p.x - x) * (p.x - x) + (p.y - y) * (p.y - y));
				}

				inline void Set(const T& px, const T& py)
				{
					x = px;
					y = py;
				}
			};

			typedef mPointT<int>	mPoint;
			typedef mPointT<float>	mPointf;
			typedef mPoint			mSize;
		}
	}
}

#endif