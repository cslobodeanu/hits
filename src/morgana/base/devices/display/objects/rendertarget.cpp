#include "rendertarget.h"
#include "../displayobjectfactory.h"
#include "../displaydevice.h"

using namespace MorganaEngine::Base::Devices::Display;
using namespace MorganaEngine::Base::Devices::Display::DisplayObjects;

__implement_class(RenderTarget, DisplayObject);

RenderTarget* RenderTarget::Screen = new DisplayObjects::RenderTarget();

bool firstRenderTarget = true;
RenderTarget::RenderTarget() : DisplayObject(DISPLAY_OBJECT_RENDER_TARGET, 0)
{
	if (firstRenderTarget)
	{
		firstRenderTarget = false;

		display = NULL;

		SetHandle(0);

		CreateParams_t cp;
		cp.width = -1;
		cp.height = -1;
		cp.attachTexturesBits = 0;
		cp.createBuffersBits = MDISPLAY_BUFFER_COLOR | MDISPLAY_BUFFER_DEPTH | MDISPLAY_BUFFER_STENCIL;
		SetCreateParams(&cp, sizeof(cp));
	}
}

RenderTarget::~RenderTarget()
{
	if(__CreateParams->colorTexture)
	{
		implementation<DisplayObjectFactory>()->Delete(__CreateParams->colorTexture);
		delete __CreateParams->colorTexture;
	}

	if (__CreateParams->depthTexture)
	{
		implementation<DisplayObjectFactory>()->Delete(__CreateParams->depthTexture);
		delete __CreateParams->depthTexture;
	}

	if (__CreateParams->stencilTexture)
	{
		implementation<DisplayObjectFactory>()->Delete(__CreateParams->stencilTexture);
		delete __CreateParams->stencilTexture;
	}
}

const Rect RenderTarget::GetBounds() const
{
	return Rect(0, 0, __CreateParams->width, __CreateParams->height);
}

void RenderTarget::SetBounds(const Rect& r)
{
	if (this == RenderTarget::Screen)
	{
		__CreateParams->width = r.width; 
		__CreateParams->height = r.height;
		return;
	}

	NOT_IMPLEMENTED;
}

const Texture*	RenderTarget::GetDepthTexture() const
{
	return __CreateParams->depthTexture;
}

const Texture*	RenderTarget::GetColorTexture() const
{
	return __CreateParams->colorTexture;
}
