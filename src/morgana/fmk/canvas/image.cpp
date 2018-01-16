#include "image.h"
#include "recttransform.h"
#include "meapplication.h"
#include "meresources.h"

using namespace MorganaEngine::Framework::Canvas;

__implement_component(Image, RendersToCanvas);

Image::Image()
{
	keepAspectRatio = false;

	color = Color::white;
	drawMode = DrawMode::Default;

	slicedTextureTopBottomBorder = 0.0f;
	slicedTextureLeftRightBorder = 0.0f;
	slicedScreenTopBottomBorder = 10.0f;
	slicedScreenLeftRightBorder = 10.0f;
	slicedDrawCenter = true;

	isMask = false;

	DoAddProperties();
}

Image::~Image()
{

}

void Image::OnPreRenderObject()
{
	if (isMask)
	{
		PushMask(this);
	}
}

void Image::OnPostRenderObject()
{
	if (isMask)
	{
		PopMask();
	}
}

void Image::OnRenderObject()
{
	if (isMask)
	{
		return;
	}

	Texture* tex = safeMaterial()->SetTexture(texture);

	SetSpriteColor(color);
	
	Rectf rect = GetComponent<RectTransform>()->rect->ToOrigin();

	if (drawMode == DrawMode::Default)
	{
		if (keepAspectRatio == true)
		{
			const float imgAspect = tex->GetBounds().Aspect();
			const float rectAspect = rect.Aspect();
			Rectf newRect = rect;
			if (imgAspect > rectAspect)
			{
				float ratio = imgAspect / rectAspect;
				newRect.height /= ratio;
			}

			if (imgAspect < rectAspect)
			{
				float ratio = imgAspect / rectAspect;
				newRect.width *= ratio;
			}

			newRect.x += (rect.width - newRect.width) * 0.5f;
			newRect.y += (rect.height - newRect.height) * 0.5f;

			rect = newRect;
		}
		PushQuad(rect.TopLeft(), rect.width, rect.height, Rectf::One);
	}
	else if (drawMode == DrawMode::Sliced)
	{
		Vector2 texCorner = Vector2(slicedTextureLeftRightBorder, slicedTextureTopBottomBorder);
		Vector2 texCenter = 1.0f - 2.0f * texCorner;
		Vector2 scrCorner = Vector2(slicedScreenLeftRightBorder, slicedScreenTopBottomBorder);
		Vector2 scrCenter = rect.size() - 2.0f * scrCorner;

		// top left
		Rectf rectTex = Rectf(0.0f, 0.0f, texCorner);
		Rectf rectScr = Rectf(rect.x, rect.y, scrCorner);
		PushQuad(rectScr, rectTex);

		// top right
		rectTex = Rectf(1.0f - texCorner.x, 0.0f, texCorner);
		rectScr = Rectf(rect.x + rect.width - scrCorner.x, rect.y, scrCorner);
		PushQuad(rectScr, rectTex);

		// bottom left
		rectTex = Rectf(0.0f, 1.0f - texCorner.y, texCorner);
		rectScr = Rectf(rect.x, rect.y + rect.height - scrCorner.y, scrCorner);
		PushQuad(rectScr, rectTex);

		// bottom right
		rectTex = Rectf(1.0f - texCorner, texCorner);
		rectScr = Rectf(rect.x + rect.width - scrCorner.x, rect.y + rect.height - scrCorner.y, scrCorner);
		PushQuad(rectScr, rectTex);

		// top border
		rectTex = Rectf(texCorner.x, 0.0f, texCenter.x, texCorner.y);
		rectScr = Rectf(rect.x + scrCorner.x, rect.y, scrCenter.x, scrCorner.y);
		PushQuad(rectScr, rectTex);

		// bottom border
		rectTex = Rectf(texCorner.x, 1.0f - texCorner.y, texCenter.x, texCorner.y);
		rectScr = Rectf(rect.x + scrCorner.x, rect.y + rect.height - scrCorner.y, scrCenter.x, scrCorner.y);
		PushQuad(rectScr, rectTex);

		// left border
		rectTex = Rectf(0.0f, texCorner.y, texCorner.x, texCenter.y);
		rectScr = Rectf(rect.x, rect.y + scrCorner.y, scrCorner.x, scrCenter.y);
		PushQuad(rectScr, rectTex);

		// right border
		rectTex = Rectf(1.0f - texCorner.x, texCorner.y, texCorner.x, texCenter.y);
		rectScr = Rectf(rect.x + rect.width - scrCorner.x, rect.y + scrCorner.y, scrCorner.x, scrCenter.y);
		PushQuad(rectScr, rectTex);

		// middle
		if (slicedDrawCenter == true)
		{
			rectTex = Rectf(texCorner, texCenter);
			rectScr = Rectf(rect.pos() + scrCorner, scrCenter);
			PushQuad(rectScr, rectTex);
		}
	}
}

void Image::DoAddProperties()
{
	super::DoAddProperties();

	__add_property1(keepAspectRatio);
	__add_property1(drawMode);
	__add_property1(slicedScreenLeftRightBorder);
	__add_property1(slicedScreenTopBottomBorder);
	__add_property1(slicedTextureLeftRightBorder);
	__add_property1(slicedTextureTopBottomBorder);
	__add_property1(slicedDrawCenter);
	__add_property1(isMask);
}

void Image::SetSlicedTextureBordersFromCenterSize(float centerSize)
{
	SetSlicedTextureBordersFromCenterSize(centerSize, centerSize);
}

void Image::SetSlicedTextureBordersFromCenterSize(float centerWidth, float centerHeight)
{
	slicedTextureLeftRightBorder = (1.0f - centerWidth) * 0.5f;
	slicedTextureTopBottomBorder = (1.0f - centerHeight) * 0.5f;
}

void Image::Start()
{
	super::Start();
	if (material == NULL && isMask)
		material = Material::DefaultMaskedCanvas;
}