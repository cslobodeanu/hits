#ifndef __MORGANA_FMK_RESOURCES_IMAGES_STD_IMAGES_H__
#define __MORGANA_FMK_RESOURCES_IMAGES_STD_IMAGES_H__

#include "metypes.h"
#include "meutils.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Resources
		{
			namespace Images
			{
				class StandardImage
				{
				public:
					template<int stride>
					static PixelBuffer<stride> Fill(const int width, const int height, const Color255& fillColor)
					{
						PixelBuffer<stride> pb(width, height);

						if (stride == 1)
							memset(pb, fillColor.r, pb.size());
						else if (stride == 2)
						{
							for (byte *ptr = pb.ptr(), *end = pb.end(); ptr < end; ptr += stride)
							{
								*ptr = fillColor.r;
								*(ptr + 1) = fillColor.a;
							}
						}
						else
						{
							for (byte *ptr = pb.ptr(), *end = pb.end(); ptr < end; ptr+= stride)
							{
								memcpy(ptr, &fillColor.r, stride);
							}
						}

						return pb;
					}

					template<int stride>
					static PixelBuffer<stride> Fill(const int size, const Color255& fillColor)
					{
						return Fill<stride>(size, size, fillColor);
					}

					template<int stride>
					static PixelBuffer<stride> VerticalGradient(const int width, const int height, const Color255& fillColor1 = Color::white, const Color255& fillColor2 = Color::black, const float _pow = 1.0f)
					{
						PixelBuffer<stride> pb(width, height);

						for (int y = 0; y < height; y++)
						{
							const float f = (float)y / (height - 1);
							const Color255 c = Color255::Lerp(fillColor1, fillColor2, Mathf::Pow(f, _pow));

							for (int x = 0; x < width; x++)
							{
								const int index = x * stride + y * pb.pitch();
								memcpy(pb.ptr() + index, &c.r, stride);
							}
						}

						return pb;
					}

					template<int stride>
					static PixelBuffer<stride> HorizontalGradient(const int width, const int height, const Color255& fillColor1 = Color::white, const Color255& fillColor2 = Color::black, const float _pow = 1.0f)
					{
						PixelBuffer<stride> pb(width, height);

						for (int y = 0; y < height; y++)
						{
							if (y == 0)
							{
								for (int x = 0; x < width; x++)
								{
									const float f = (float)x / (width - 1);
									const Color255 c = Color255::Lerp(fillColor1, fillColor2, Mathf::Pow(f, _pow));

									const int index = x * stride;
									memcpy(pb.ptr() + index, &c.r, stride);
								}
							}
							else
							{
								memcpy(pb.ptr() + y * pb.pitch(), pb.ptr(), pb.pitch());
							}
						}

						return pb;
					}

					template<int stride>
					static PixelBuffer<stride> Checkered(const int width, const int height, const Color255& fillColor1, const Color255& fillColor2, const int squareSize)
					{
						PixelBuffer<stride> pb(width, height);

						bool greysq = true;
						for (int y = 0; y < height; y++)
						{
							const int index_y = y / squareSize;
							for (int x = 0; x < width; x++)
							{
								const int index_x = x / squareSize;

								const Color255 c = ((index_x % 2 == 0 && index_y % 2 == 0) || (index_x % 2 == 1 && index_y % 2 == 1)) ? fillColor1 : fillColor2;
								memcpy(pb.ptr() + x * stride + y * pb.pitch(), &c.r, stride);
							}
						}

						return pb;
					}

					template<int stride>
					static PixelBuffer<stride> Disc(const int width, const int height, const Color255& fillColor, const Color255& bkgColor, float att = 1.0f)
					{
						PixelBuffer<stride> pb(width, height);

						const Vector2 center((float)width * 0.5f, (float)height * 0.5f);
						const float radius = (float)width * 0.5f;

						for (int y = 0; y < height; y++)
						{
							for (int x = 0; x < width; x++)
							{
								const Vector2 p = Vector2((float)x, (float)y);
								const float d = Vector2::Distance(&p, &center) / radius;
								const float f = Mathf::Clamp01(Mathf::Pow(d, att));

								const Color255 c = Color255::Lerp(fillColor, bkgColor, f);

								byte* pixel = pb.pixel(x, y);

								if (stride == 2)
								{
									pixel[0] = c.r;
									pixel[1] = c.a;
								}
								else
									memcpy(pixel, &c.r, stride);
							}
						}

						return pb;
					}

					template<int stride>
					static PixelBuffer<stride> Disc(const int size, const Color255& fillColor, const Color255& bkgColor, float att = 1.0f)
					{
						return Disc<stride>(size, size, fillColor, bkgColor, att);
					}

					template<int stride>
					static PixelBuffer<stride> Circle(const int width, const int height, const Color255& fillColor, const Color255& bkgColor, float thickness = 1.0f, float att = 1.0f)
					{
						PixelBuffer<stride> pb(width, height);

						const float halfThickness = thickness * 0.5f;

						const Vector2 center((float)width * 0.5f, (float)height * 0.5f);
						const float radius = (float)width * 0.5f - halfThickness * 1.1f;

						for (int y = 0; y < height; y++)
						{
							for (int x = 0; x < width; x++)
							{
								const Vector2 p = Vector2((float)x, (float)y);
								const float d = Mathf::Clamp01(Mathf::Abs(Vector2::Distance(&p, &center) - radius) / halfThickness);
								const float f = Mathf::Clamp01(Mathf::Pow(d, att));

								const Color255 c = Color255::Lerp(fillColor, bkgColor, f);

								memcpy(pb.ptr() + x * stride + y * pb.pitch(), &c.r, stride);
							}
						}

						return pb;
					}

					template<int stride>
					static PixelBuffer<stride> Circle(const int size, const Color255& fillColor, const Color255& bkgColor, float thickness = 1.0f, float att = 1.0f)
					{
						return Circle<stride>(size, size, fillColor, bkgColor, thickness, att);
					}
				};
			}
		}
	}
}

#endif