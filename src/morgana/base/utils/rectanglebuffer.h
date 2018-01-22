#ifndef __MORGANA_BASE_UTILS_RECTANGLE_BUFFER_H__
#define __MORGANA_BASE_UTILS_RECTANGLE_BUFFER_H__

namespace MorganaEngine
{
	namespace Base
	{
		namespace Utils
		{
			template<int stride>
			class RectangleBuffer
			{
				Array<byte> pixels;
				int width, height;
				byte tmp1[stride];
				byte tmp2[stride];
				byte tmp3[stride];
			public:
				RectangleBuffer()
				{
					Alloc(0, 0);
				}

				RectangleBuffer(const int width, const int height, const byte* src = NULL)
				{
					Alloc(width, height, src);
				}

				RectangleBuffer(const RectangleBuffer& other)
				{
					Alloc(other.width, other.height, (byte*)other);
				}

				void Alloc(const int width, const int height, const byte* src = NULL)
				{
					this->width = width;
					this->height = height;
					pixels.SetLength(width * height * stride);
					if (src != NULL)
					{
						memcpy(pixels.ptr(), src, size());
					}
				}

				inline operator byte*() const
				{
					return pixels.ptr();
				}

				inline byte* ptr() const
				{
					return pixels.ptr();
				}

				inline byte* end() const
				{
					return pixels.end();
				}

				inline byte* pixel(const int x, const int y) const
				{
					return pixels.ptr() + (x + y * width) * stride;
				}

				inline const size_t size() const
				{
					return width * height * stride;
				}

				inline const int pitch() const
				{
					return width * stride;
				}

				inline const int _width() const
				{
					return width;
				}

				inline const int _height() const
				{
					return height;
				}

				const void Transpose(byte* outBuffer) const
				{
					Transpose(pixels.ptr(), outBuffer, width, height);
				}

				void static Transpose(const byte* srcBuffer, byte* outBuffer, const int width, const int height)
				{
					for (int y = 0; y < height; y++)
						for (int x = 0; x < width; x++)
						{
							const byte* src = srcBuffer + (x + y * width) * stride;
							byte* dst = outBuffer + (y + x * height) * stride;
							memcpy(dst, src, stride);
						}
				}

				void Transpose()
				{
					LocalMemoryBlock lmb(size());
					Transpose((byte*)lmb.ptr());
					memcpy(pixels.ptr(), lmb.ptr(), size());

					const int tmp = height;
					height = width;
					width = tmp;
				}

				inline byte* samplei(const int x, const int y, const int halfSampleX, const int halfSampleY)
				{
					int count = 0;
					unsigned int accum[stride];
					memset(accum, 0, sizeof(accum));
					for (int iy = -halfSampleY; iy <= halfSampleY; iy++)
					{
						for (int ix = -halfSampleX; ix <= halfSampleX; ix++)
						{
							int ofsX = ix + x;
							if (ofsX < 0) continue;
							if (ofsX >= width) continue;

							int ofsY = iy + y;
							if (ofsY < 0) continue;
							if (ofsY >= height) continue;

							byte* src = pixel(ofsX, ofsY);
							for (int k = 0; k < stride; k++)
								accum[k] += src[k];

							count++;
						}
					}

					if (count > 0)
					{
						for (int i = 0; i < stride; i++)
						{
							tmp1[i] = (byte)(accum[i] / count);
						}
					}
					else
					{
						memset(tmp1, 0, stride);
					}

					return &tmp1[0];
				}

				inline byte* sample(const int x, const int y, const float sampleX, const float sampleY)
				{
					const int sx = Mathf::CeilToInt(sampleX * 0.5f);
					const int sy = Mathf::CeilToInt(sampleY * 0.5f);
					return samplei(x, y, sx, sy);
				}

				inline byte* sample(const int x, const int y, const float sampleXY)
				{
					return sample(x, y, sampleXY, sampleXY);
				}

				const void Resample(byte* outBuffer, const int newWidth, const int newHeight)
				{
					const float samplex = (float)width / (float)newWidth;
					const float sampley = (float)height / (float)newHeight;

					const int sx = Mathf::RoundToInt(samplex * 0.5f);
					const int sy = Mathf::RoundToInt(sampley * 0.5f);

					for (int y = 0; y < newHeight; y++)
					{
						for (int x = 0; x < newWidth; x++)
						{
							byte* dst = outBuffer + (x + y * newWidth) * stride;
							const float px = ((float)x /*+ 0.5f*/) * samplex;
							const float py = ((float)y /*+ 0.5f*/) * sampley;
							memcpy(dst, samplei((int)px, (int)py, sx, sy), stride);
						}
					}
				}

				const void Resample(RectangleBuffer& outBuffer)
				{
					Resample((byte*)outBuffer, outBuffer.width, outBuffer.height);
				}

				void Negative()
				{
					const int sz = size();
					for (byte *p = pixels.ptr(), *end = pixels.end(); p < end; p++)
					{
						(*p) = 255 - (*p);
					}
				}

				void Clear(const byte val = 0)
				{
					memset(pixels.ptr(), val, size());
				}

				void Blit(const RectangleBuffer src, const int ofsx, const int ofsy)
				{
					const int lw = Mathf::Min(width - ofsx, src.width) * stride;

					for (int y = 0; y < src.height; y++)
					{
						if (y + ofsy >= height) break;

						memcpy(pixel(ofsx, y + ofsy), src.pixel(0, y), lw);
					}
				}

				void Blit(const RectangleBuffer src, const int srcX, const int srcY, const int srcW, const int srcH, const int ofsx, const int ofsy)
				{
					const int lw = Mathf::Min(width - ofsx, srcW) * stride;

					for (int y = 0; y < srcH; y++)
					{
						if (y + ofsy >= height) break;

						memcpy(pixel(ofsx, y + ofsy), src.pixel(srcX, srcY + y), lw);
					}
				}

				void Blur(float blurSamples = 2.0f)
				{
					const int sx = Mathf::RoundToInt(blurSamples * 0.5f);

					for (int y = 0; y < height; y++)
					{
						for (int x = 0; x < width; x++)
						{
							memcpy(pixel(x, y), sample(x, y, (float)sx), stride);
						}
					}
				}

				void BlitChannel(const int dstChannel, const byte* srcBytes, const int srcChannel, const int srcStride)
				{
					const int lw = pitch();

					for (int y = 0; y < height; y++)
					{
						for (int x = 0; x < width; x++)
							*(pixel(x, y) + dstChannel) = srcBytes[(x + y * width) * srcStride + srcChannel];
					}
				}
			};

			typedef RectangleBuffer<1>	GrayscaleBuffer;
			typedef RectangleBuffer<2>	GrayscaleAlphaBuffer;
			typedef RectangleBuffer<3>	RGBBuffer;
			typedef RectangleBuffer<4>	RGBABuffer;
		}
	}
}

#endif