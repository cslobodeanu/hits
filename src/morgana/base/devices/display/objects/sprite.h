#ifndef __MORGANA_BASE_DEVICES_DISPLAY_DISPLAY_OBJECTS_SPRITE_H__
#define __MORGANA_BASE_DEVICES_DISPLAY_DISPLAY_OBJECTS_SPRITE_H__

#include "rect.h"
#include "texture.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Devices
		{
			namespace Display
			{
				namespace DisplayObjects
				{
					struct SpriteInfo_t
					{
						Texture*	texture;
						Rect		rect;
						bool		transposed;

						int			borderLeft, borderTop, borderBottom, borderRight;

						SpriteInfo_t()
						{
							texture = NULL;
							rect = Rect(0, 0, 0, 0);
							transposed = false;

							borderLeft = 0;
							borderTop = 0;
							borderBottom = 0;
							borderRight = 0;
						}

						SpriteInfo_t(const SpriteInfo_t& other)
						{
							texture = other.texture;
							rect = other.rect;
							transposed = other.transposed;

							borderLeft = other.borderLeft;
							borderTop = other.borderTop;
							borderBottom = other.borderBottom;
							borderRight = other.borderRight;
						}

						const Rectf rectf() const
						{
							const float width = (float)texture->GetBounds().width;
							const float height = (float)texture->GetBounds().height;
							return Rectf((float)rect.x / width, (float)rect.y / height, (float)rect.width / width, (float)rect.height / height);
						}
					};
				}
			}
		}
	}
}

#endif