#include "stdwindow.h"

using namespace MorganaEngine::Standard::UI;

__implement_class(StdWindow, Window);

StdWindow::StdWindow()
{
}

void StdWindow::Awake()
{
	super::Awake();

	rectTransform->rect = Rectf(100.0f, 100.0f, 320.0f, 240.0f);
	material = new Material(*Material::DefaultFontCanvas);

}

void StdWindow::Start()
{
	super::Start();
}

void StdWindow::Update()
{
	super::Update();

	DesktopTheme* thm = Desktop::main->theme;

	const Vector2 titleOfs = Vector2(5.0f, 5.0f);
	const float titleHeight = thm->titleFont->GetLineHeight(thm->titleFontSize) + titleOfs.y * 2.0f;

	clientRect.Set(2.0f, titleHeight, rectTransform->rect->width - 4.0f, rectTransform->rect->height - titleHeight);
}

void StdWindow::OnRenderObject()
{
	Rectf rect = rectTransform->rect->ToOrigin();

	DesktopTheme* thm = Desktop::main->theme;

	SpriteInfo_t* white = thm->titleFont->RequestSprite(FontTrueType::White, 16);
	SpriteInfo_t* blob = thm->titleFont->RequestSprite(FontTrueType::Blob, 32);

	// shadow first
	safeMaterial()->SetTexture(blob->texture);
	SetSpriteColor(Color255(0, 0, 0, 55));
	//PushQuad(rect.TopLeft() + Vector2::One * 5.0f, rect.width, rect.height, white->rectf());
	PushSlicedSprite(blob, rect.TopLeft() - Vector2::One * 7.0f, rect.width + 16.0f, rect.height + 16.0f, Vector2::One * 0.95f);

	// bkg
	safeMaterial()->SetTexture(white->texture);
	SetSpriteColor(thm->windowBkgColor);
	PushSprite(white, rect.TopLeft(), rect.width, rect.height);

	const Vector2 titleOfs = Vector2(5.0f, 5.0f);

	// title bkg
	const float titleHeight = thm->titleFont->GetLineHeight(thm->titleFontSize) + titleOfs.y * 2.0f;
	safeMaterial()->SetTexture(white->texture);
	SetSpriteColor(Color::Lerp(thm->titleBkgColor, thm->windowBkgColor, currentActive == this ? 0.0f : 0.74f));
	PushQuad(rect.TopLeft(), rect.width, titleHeight, white->rectf());

	// borders
	const float bt = 1.5f;
	PushSprite(white, rect.TopLeft(), bt, rect.height);
	PushSprite(white, rect.TopRight() - Vector2(bt, 0.0f), bt, rect.height);
	PushSprite(white, rect.BottomLeft() - Vector2(0.0f, bt), rect.width, bt);

	// title
	SetSpriteColor(Color::Lerp(thm->titleTextColor, thm->windowBkgColor, currentActive == this ? 0.0f : 0.74f));

	String tmp = caption;
	Vector2 sz = TextTrueType::Measure(this, thm->titleFont, thm->titleFontSize, tmp.c_str());
	while ((sz.x + titleOfs.x * 2.0f) > rectTransform->rect->width)
	{
		tmp[tmp.Length() - 1] = 0;
		if (tmp.Length() >= 3)
		{
			memset(&tmp[tmp.Length() - 3], '.', 3);
		}
		sz = TextTrueType::Measure(this, thm->titleFont, thm->titleFontSize, tmp.c_str());
	}
	TextTrueType::Draw(this, thm->titleFont, thm->titleFontSize, tmp.c_str(), Vector2((rect.width - sz.x) * 0.5f, titleOfs.y), safeMaterial());

	SetSpriteColor(Color(1.0f, 0, 0, 0.15f));
}