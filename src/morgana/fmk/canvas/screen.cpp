#include "screen.h"
#include "base.h"

using namespace MorganaEngine::Framework::Canvas;

int Screen::width = 0;
int Screen::height = 0;
int Screen::dpi = 96;
Vector2 Screen::size = Vector2::Zero;

void Screen::Update()
{
	AquireSize();

	Rect rtr(0, 0, Screen::width, Screen::height);
	RenderTarget::Screen->SetBounds(rtr);
}

void Screen::Initialize()
{
	AquireSize();

#ifdef OS_WINDOWS
	HDC screen = GetDC(NULL);
	dpi = GetDeviceCaps(screen, LOGPIXELSX);
	ReleaseDC(NULL, screen);
#endif
}

void Screen::AquireSize()
{
	width = MEView::main->width;
	height = MEView::main->height;

	size = Vector2((float)width, (float)height);
}