#include "window.h"
#include "../inputs/input.h"

using namespace MorganaEngine::Framework::WindowSystem;
using namespace MorganaEngine::Framework::Inputs;

__implement_class(Window, Widget);

Window*	Window::currentActive = NULL;

Window::Window()
{
	sizeable = false;
	movable = false;
	minSize = Vector2::One * 50.0f;
	DoAddProperties();
}

Window::~Window()
{
	if (currentActive == this)
	{
		currentActive = NULL;
	}
}

void Window::Start()
{
	super::Start();
}

void Window::Update()
{
	super::Update();
}

Vector2 touchPosScreenSpace;
Vector2 initialRectTopLeft;
bool canResize = false;
Vector2 initialSize;
void Window::OnTouchDown(Component* c, Vector2 p, bool& handled)
{
	super::OnTouchDown(c, p, handled);
	touchPosScreenSpace = Input::GetMousePosScr();
	initialRectTopLeft = rectTransform->rect->TopLeft();

	canResize = false;
	if (sizeable)
	{
		const float cornerSquare = 20.0f;
		const Rectf r = Rectf(rectTransform->rect->size() - Vector2::One * cornerSquare * 0.5f, Vector2::One * cornerSquare);
		if (r.Contains(p))
		{
			canResize = true;
			initialSize = rectTransform->rect->size();
		}
	}

	Activate();
}

void Window::OnTouchUp(Component* c, Vector2 p, bool& handled)
{
	super::OnTouchUp(c, p, handled);
}

void Window::OnTouchMove(Component* c, Vector2 p, bool& handled)
{
	super::OnTouchMove(c, p, handled);

	if (canResize)
	{
		Rectf r = rectTransform->rect;
		r.width = Mathf::Max(minSize->x, initialSize.x + p.x - touchPosScreenSpace.x);
		r.height = Mathf::Max(minSize->y, initialSize.y + p.y - touchPosScreenSpace.y);
		rectTransform->rect = r;
	}
	else if (movable)
	{
		Rectf r = rectTransform->rect;
		r.pos(initialRectTopLeft + Input::GetMousePosScr() - touchPosScreenSpace);
		rectTransform->rect = r;
	}
}

void Window::Activate()
{
	currentActive = this;

	gameObject->BringToFront();
}

void Window::DoAddProperties()
{
	super::DoAddProperties();
	__add_property1(sizeable);
	__add_property1(movable);
}
