#ifndef __MORGANA_BASE_TYPES_RECT_H__
#define __MORGANA_BASE_TYPES_RECT_H__

#include "../math/vector2.h"
#include "../math/vector3.h"

namespace MorganaEngine
{
	namespace Base
	{
		using namespace Math;
		namespace Types
		{
			template <class T>
			class RectangleType
			{
			public:
				T x, y, width, height;

				inline T		Area()          const { return width * height; }
				inline float    Aspect()        const { return float(width) / float(height); }
				inline float    OneOverAspect() const { return float(height) / float(width); }
				inline bool     IsLandscape()   const { return width >= height; }
				inline bool     IsPortrait()    const { return height > width;  }
				inline void		Set(T _x, T _y, T _w, T _h) { x = _x; y = _y; width = _w; height = _h;}

				inline void     ScaleSizeWithAspect(const T refSize, T &sizeWidth, T &sizeHeight) const
				{
					if(width >= height)
					{
						sizeWidth  = refSize;
						sizeHeight = (refSize * height) / width;
					}
					else
					{
						sizeWidth  = (refSize * width) / height;
						sizeHeight = refSize;
					}
					if (sizeWidth % 2)  ++sizeWidth;
					if (sizeHeight % 2) ++sizeHeight;
				}

				inline RectangleType() {}
				inline RectangleType(const T X, const T Y, const T Width, const T Height) : x(X), y(Y), width(Width), height(Height) {}

				inline bool IsPointInside(T pX,T pY, float radius = 0.0f)
				{
					return (pX>=(x - radius) && pY >= (y - radius) && pX <=(x+width + radius) && pY<=(y+height + radius));
				}
				inline void ClampPoint(float &pX,float &pY)
				{
					if (pX >= x + width) pX = float(x + width);
					if (pX <= x) pX = float(x);
					if (pY >= y + height) pY = float(y + height);
					if (pY <= y) pY = float(y);
				}

				inline void Inflate(const T dw, const T dh)
				{
					x -= dw / 2;
					width += dw;
					y -= dh / 2;
					height += dh;
				}

				inline void Normalize(float withValue)
				{
					x = (float)x / withValue;
					y = (float)y / withValue;
					width = (float)width / withValue;
					height = (float)height / withValue;
				}

				bool operator  != (const RectangleType<T> &v) const
				{
					return memcmp(&x, &v.x, 4 * sizeof(T)) != 0;
				}
				bool operator  == (const RectangleType<T> &v)
				{
					return memcmp(&x, &v.x, 4 * sizeof(T)) == 0;
				}

				RectangleType<T>& operator  = (const RectangleType<T> &other)
				{
					memcpy(&x, &other.x, 4 * sizeof(T));
					return *this;
				}

				inline void Offset(const T& ox, const T& oy)
				{
					x += ox;
					y += oy;
				}

				inline RectangleType<T> ToOrigin()
				{
					RectangleType<T> r(0.0f, 0.0f, width, height);
					return r;
				}

				const inline T right() const
				{
					return x + width;
				}

				const inline T bottom() const
				{
					return y + height;
				}
			};

			class Rectf : public RectangleType < float >
			{
			public:
				Rectf() : RectangleType<float>()
				{
					x = 0.0f;
					y = 0.0f;
					width = 0.0f;
					height = 0.0f;
				}

				Rectf(const float x, const float y, const float width, const float height) : RectangleType<float>(x, y, width, height)
				{}

				Rectf(const float x, const float y, SizeF size) : RectangleType<float>(x, y, size.x, size.y)
				{}

				Rectf(Vector2 pos, SizeF size) : RectangleType<float>(pos.x, pos.y, size.x, size.y)
				{}

				inline const SizeF size() const
				{
					return SizeF(width, height);
				}

				inline const Vector2 pos() const
				{
					return Vector2(x, y);
				}

				inline const void pos(const Vector2& p)
				{
					x = p.x;
					y = p.y;
				}

				inline void Set(const Vector2& pos, const Vector2& size)
				{
					x = pos.x;
					y = pos.y;
					width = size.x;
					height = size.y;
				}

				inline void Set(const float& x, const float& y, const float& width, const float& height)
				{
					this->x = x;
					this->x = y;
					this->width = width;
					this->height = height;
				}


				inline const bool Contains(const Vector2& p) const
				{
					return p.x >= x && p.y >= y && p.x < (x + width) && p.y < (y + height);
				}

				static Rectf Zero;
				static Rectf One;

				inline Vector2 GetPosition()
				{
					return Vector2(x, y);
				}

				inline Rectf ToOrigin() const
				{
					Rectf r(0.0f, 0.0f, width, height);
					return r;
				}

				inline const Vector2 TopLeft() const
				{
					return Vector2(x, y);
				}

				inline const Vector2 TopRight() const
				{
					return Vector2(x + width, y);
				}

				inline const Vector2 BottomLeft() const
				{
					return Vector2(x, y + height);
				}

				inline const Vector2 BottomRight() const
				{
					return Vector2(x + width, y + height);
				}

				inline void Inflate(const float dw, const float dh)
				{
					x -= dw / 2.0f;
					width += dw;
					y -= dh / 2.0f;
					height += dh;
				}
			};

			typedef RectangleType<int>		Rect;

			struct RectSize
			{
				int width;
				int height;
			};
		}
	}
}

#endif
