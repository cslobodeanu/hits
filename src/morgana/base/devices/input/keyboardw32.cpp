#include "keyboardw32.h"


#ifdef OS_WINDOWS_WIN32

#include "meio.h"

#include <devguid.h>


using namespace MorganaEngine::Base::Devices::InputDevices;

__implement_class(KeyboardDeviceWin32, InputDevice);

KeyboardDeviceWin32::KeyboardDeviceWin32() : InputDevice(MDEVICE_INPUT_KEYBOARD)
{

}

KeyboardDeviceWin32::~KeyboardDeviceWin32()
{

}

void KeyboardDeviceWin32::OnUpdate()
{
	super::OnUpdate();
}


extern String	DevWin32_GetDeviceDescOfType(const GUID& guid);
extern void		DevWin32_ResetDevicesInterator();
bool KeyboardDeviceWin32::IsAvailable()
{
	DevWin32_ResetDevicesInterator();
	String tmp = DevWin32_GetDeviceDescOfType(GUID_DEVCLASS_KEYBOARD);
	int count = 0;
	while(!tmp.IsEmpty())
	{
		deviceString = tmp;
		//DEBUG_OUT("Keyboard #%d: %s", i, tmp.c_str());
		count++;
		tmp = DevWin32_GetDeviceDescOfType(GUID_DEVCLASS_KEYBOARD);
	}
	DevWin32_ResetDevicesInterator();

	if(count > 1)
		deviceString = "Multiple Keyboards";

	return deviceString.Length() > 0;
}

String	KeyboardDeviceWin32::GetDescription()
{
	return "Win32 Keyboard Device";
}

void KeyboardDeviceWin32::OnWindowMessage(unsigned int msg, unsigned int wParam, unsigned int lParam)
{
	switch(msg)
	{
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		{
			if(wParam >= 0 && wParam <= 255)
				data->keysState[wParam] = true;
			break;
		}

	case WM_SYSKEYUP:
	case WM_KEYUP:
		{
			if(wParam >= 0 && wParam <= 255)
				data->keysState[wParam] = false;
			break;
		}
	}
}

#endif
