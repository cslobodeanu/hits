#include "inputeventsmanager.h"
#include "canvassurface.h"
#include "recttransform.h"
#include "inputeventsreceiver.h"
#include "../inputs/input.h"
#include "screen.h"

using namespace MorganaEngine::Framework::Canvas;
using namespace MorganaEngine::Framework::Inputs;

__implement_component(InputEventsManager, Component);

InputEventsManager::InputEventsManager()
{
	touchState = 0;
}

InputEventsManager::~InputEventsManager()
{
}

void InputEventsManager::Start()
{
	super::Start();

	CanvasSurface* cv = GetComponent<CanvasSurface>();

	ASSERT(cv != null, "InputEventsManager must be along with a CanvasSurface");

	MEObject::OnObjectDestroy += Callback(OnObjectDestroyed, MEObject*);

	capture = null;
}

void InputEventsManager::OnDestroy()
{
	MEObject::OnObjectDestroy -= Callback(OnObjectDestroyed, MEObject*);
}

void InputEventsManager::OnObjectDestroyed(MEObject* obj)
{
	if (capture == obj)
		capture = null;
}

void InputEventsManager::UpdateEvents()
{
	bool callTouchDown = Input::GetMouseButtonDown(0) && touchState == 0;
	bool callTouchUp = Input::GetMouseButtonDown(0) == false && touchState == 1;
	bool callTouchMove = Input::GetMouseButtonDown(0) && touchState == 1;

	if (Input::GetMouseButtonDown(0))
		touchState = 1;
	if (Input::GetMouseButtonDown(0) == false)
		touchState = 0;

	if (!callTouchUp && !callTouchDown && !callTouchMove) return;

	Vector2 touchPos = Input::GetMousePosScr();

	if (callTouchMove && capture != null)
	{
		const Matrix* wm = capture->GetInvertedWorldMatrixPtr();
		Vector2 p = (*wm) * touchPos;
		bool handled = true;
		capture->GetComponent<InputEventsReceiver>()->OnTouchMove(capture, p, handled);
		return;
	}

	if (callTouchUp && capture != null)
	{
		const Matrix* wm = capture->GetInvertedWorldMatrixPtr();
		Vector2 p = (*wm) * touchPos;
		bool handled = true;
		capture->GetComponent<InputEventsReceiver>()->OnTouchUp(capture, p, handled);
		DEBUG_OUT("Touch up on [%s]", capture->gameObject->GetName().c_str());
		capture = null;
	}

	for (int i = receivers.Length() - 1; i >= 0; i--)
	{
		InputEventsReceiver* ier = receivers[i];
		RectTransform* rt = ier->GetComponent<RectTransform>();

		const Matrix* wm = rt->GetInvertedWorldMatrixPtr();
		Vector2 p = (*wm) * touchPos;

		Rectf rr = rt->rect->ToOrigin();
		if (rr.Contains(p))
		{
			if (callTouchDown)
			{
				bool handled = true;
				ier->OnTouchDown(receivers[i], p, handled);

				if (handled)
				{
					capture = ier->GetComponent<RectTransform>();
					DEBUG_OUT("Touch down on [%s]", ier->gameObject->GetName().c_str());
					break;
				}
			}
		}
	}
}

void InputEventsManager::Update()
{
	UpdateEvents();
	receivers.Clear();
}

void InputEventsManager::AddReceiver(InputEventsReceiver* ier)
{
	receivers.Add(ier);
}