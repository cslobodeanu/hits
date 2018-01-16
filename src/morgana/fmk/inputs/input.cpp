#include "input.h"
#include "medevices.h"
#include "../canvas/screen.h"

using namespace MorganaEngine::Framework::Inputs;
using namespace MorganaEngine::Framework::Canvas;

InputDevice*	Input::kb = NULL;
InputDevice*	Input::mouse = NULL;

void Input::Initialize()
{
	kb = (InputDevice*)singleton<MEDevices>()->GetDevice(InputDevice::MDEVICE_INPUT, InputDevice::MDEVICE_INPUT_KEYBOARD);
	mouse = (InputDevice*)singleton<MEDevices>()->GetDevice(InputDevice::MDEVICE_INPUT, InputDevice::MDEVICE_INPUT_SINGLE_POINTING);

	singleton<MEDevices>()->OnDeviceLost += CallbackStatic(Input::OnDeviceLost, MEDevice*);
}

bool Input::GetKeyDown(int key)
{
	if (kb == NULL)
	{
		Initialize();
		if (kb == NULL)
			return false;
	}
	return kb->GetData()->keysState[key];
}

bool Input::GetKeyPressed(int key)
{
	const bool b = GetKeyDown(key);
	if (kb == NULL) return false;
	return b && !kb->GetPrevData()->keysState[key];
}

bool Input::GetKeyReleased(int key)
{
	const bool b = GetKeyDown(key);
	if (kb == NULL) return false;
	return !b && kb->GetPrevData()->keysState[key];
}

Vector2 Input::GetMousePos()
{
	if (mouse == NULL)
	{
		Initialize();
		if (mouse == NULL)
			return Vector2::Zero;
	}

	return Vector2(mouse->GetData()->xAxis[0], mouse->GetData()->yAxis[0]);
}

Vector2 Input::GetMousePosScr()
{
	return (GetMousePos() + Vector2::One) * Screen::size * 0.5f;
}

bool Input::GetMouseButtonDown(int button)
{
	if (mouse == NULL)
	{
		Initialize();
		if (mouse == NULL)
			return false;
	}

	return mouse->GetData()->buttonsState[button] != 0;
}

bool Input::GetMouseButtonPressed(int button)
{
	const bool b = GetMouseButtonDown(button);
	if (mouse == NULL) return false;
	return b && mouse->GetPrevData()->buttonsState[button] == 0;
}

bool Input::GetMouseButtonReleased(int button)
{
	const bool b = GetMouseButtonDown(button);
	if (mouse == NULL) return false;
	return !b && mouse->GetPrevData()->buttonsState[button] != 0;
}

int Input::GetMouseWheel()
{
	if (mouse == NULL)
	{
		Initialize();
		if (mouse == NULL)
			return 0;
	}

	return mouse->GetData()->buttonsState[MouseButtons::MouseWheel];
}

void Input::OnDeviceLost(MEDevice* dev)
{
	if (dev == kb)
	{
		kb = NULL;
	}

	if (dev == mouse)
	{
		mouse = NULL;
	}
}