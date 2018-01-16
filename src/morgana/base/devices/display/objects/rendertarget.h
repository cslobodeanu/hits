#ifndef __MORGANA_BASE_DEVICES_DISPLAY_OBJECTS_RENDER_TARGET_H__
#define __MORGANA_BASE_DEVICES_DISPLAY_OBJECTS_RENDER_TARGET_H__

#include "../displayobject.h"
#include "../rendepipelinedefs.h"

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
					class Texture;
					class RenderTarget : public DisplayObject
					{
						__declare_class(RenderTarget, DisplayObject);

					public:

						RenderTarget();
						virtual ~RenderTarget();

						const Rect GetBounds() const;
						void SetBounds(const Rect& r);

						const Texture*	GetDepthTexture() const;
						const Texture*	GetColorTexture() const;

						struct CreateParams_t
						{
							int			width, height;
							PixelFormat	colorFormat, depthStencilFormat;
							int			createBuffersBits; //  MDISPLAY_BUFFER_COLOR, MDISPLAY_BUFFER_DEPTH, MDISPLAY_BUFFER_STENCIL
							int			attachTexturesBits; // MDISPLAY_BUFFER_COLOR, MDISPLAY_BUFFER_DEPTH, MDISPLAY_BUFFER_STENCIL
							Texture		*colorTexture, *depthTexture, *stencilTexture;
							unsigned int	depthBuffer;

							CreateParams_t()
							{
								colorTexture = depthTexture = stencilTexture = NULL;
								depthBuffer = 0;
								colorFormat = PixelFormat::FormatRGBA;
								depthStencilFormat = PixelFormat::FormatDepth;
							}
						};

						static RenderTarget*	Screen;
					};
				}
			}
		}
	}
}
#endif