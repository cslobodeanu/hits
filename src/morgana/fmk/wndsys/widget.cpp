#include "widget.h"
#include "../canvas/canvas.h"

using namespace MorganaEngine::Framework::WindowSystem;

__implement_component(Widget, RendersToCanvas);

Widget* Widget::capture = NULL;

Widget::Widget()
{
	rectTransform = NULL;
	DoAddProperties();
}

Widget::~Widget()
{

}

void Widget::Start()
{
	super::Start();

	InputEventsReceiver* ier = GetComponent<InputEventsReceiver>();
	ier->OnTouchDown += Callback(_OnTouchDown, Component*, Vector2, bool&);
	ier->OnTouchUp += Callback(_OnTouchUp, Component*, Vector2, bool&);
	ier->OnTouchMove += Callback(_OnTouchMove, Component*, Vector2, bool&);
}

void Widget::Update()
{

}

void Widget::_OnTouchDown(Component* sender, Vector2 p, bool& handled)
{
	capture = this;

	OnTouchDown(sender, p, handled);

	OnTouchDownEvent(this, p, handled);
}

void Widget::_OnTouchUp(Component* sender, Vector2 p, bool& handled)
{
	if (capture != NULL) // and check movement
	{
		capture->_OnTouchClick(capture, p, handled);
	}

	capture = NULL;

	OnTouchUp(sender, p, handled);

	OnTouchUpEvent(this, p, handled);
}

void Widget::_OnTouchMove(Component* sender, Vector2 p, bool& handled)
{
	OnTouchMove(sender, p, handled);

	OnTouchMoveEvent(this, p, handled);
}

void Widget::_OnTouchClick(Component* sender, Vector2 p, bool& handled)
{
	OnTouchClick(sender, p, handled);

	OnTouchClickEvent(this, p, handled);
}

void Widget::Awake()
{
	gameObject->AddComponent<InputEventsReceiver>();

	rectTransform = GetComponent<RectTransform>();
	if (rectTransform == NULL)
		rectTransform = gameObject->AddComponent<RectTransform>();
}

void Widget::DoAddProperties()
{
	super::DoAddProperties();

	__add_property1(caption);
}

void Widget::OnPreRenderObject()
{

}

void Widget::OnRenderObject()
{

}

void Widget::OnPostRenderObject()
{

}

void Widget::OnTouchDown(Component*, Vector2 p, bool& handled)
{

}

void Widget::OnTouchUp(Component*, Vector2 p, bool& handled)
{

}

void Widget::OnTouchMove(Component*, Vector2 p, bool& handled)
{

}

void Widget::OnTouchClick(Component*, Vector2 p, bool& handled)
{

}