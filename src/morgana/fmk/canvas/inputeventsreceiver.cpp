#include "inputeventsreceiver.h"

using namespace MorganaEngine::Framework::Canvas;

__implement_component(InputEventsReceiver, Component);

InputEventsReceiver::InputEventsReceiver()
{
	inputManager = null;
}

InputEventsReceiver::~InputEventsReceiver()
{

}

void InputEventsReceiver::Start()
{
	inputManager = gameObject->GetComponentInParent<InputEventsManager>();
}

void InputEventsReceiver::Update()
{
	if (inputManager != null)
	{
		inputManager->AddReceiver(this);
	}
}