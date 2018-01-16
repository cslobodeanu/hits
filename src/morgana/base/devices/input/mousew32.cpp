#include "mousew32.h"


#ifdef OS_WINDOWS_WIN32

#include <devguid.h>

#include "../../views/mevieww32.h"
#include "meio.h"

#include "mousedef.h"

using namespace MorganaEngine::Base::Devices::InputDevices;
using namespace MorganaEngine::Base::Views;

__implement_class(MouseDeviceWin32, InputDevice);

extern String	DevWin32_GetDeviceDescOfType(const GUID& guid);
extern void		DevWin32_ResetDevicesInterator();

MouseDeviceWin32::MouseDeviceWin32() : InputDevice(MDEVICE_INPUT_SINGLE_POINTING)
{
	m_iWheelDelta = 0;
}

MouseDeviceWin32::~MouseDeviceWin32()
{

}

void MouseDeviceWin32::OnUpdate()
{
	super::OnUpdate();

	POINT p;
	::GetCursorPos(&p);
	
	if(MEView::main)
	{
		if (::GetActiveWindow() != (HWND)MEView::main->GetHandle())
			return;
		::ScreenToClient((HWND)MEView::main->GetHandle(), &p);
	}

	data->xAxis[0] = ((float)p.x / (float)MEView::main->width) * 2.0f - 1.0f;
	data->yAxis[0] = ((float)p.y / (float)MEView::main->height) * 2.0f - 1.0f;

	data->buttonsState[MouseButtons::MouseLeft] = (::GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
	data->buttonsState[MouseButtons::MouseRight] = (::GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
	data->buttonsState[MouseButtons::MouseMiddle] = (::GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0;

	if(m_iWheelDelta != 0)
	{
		data->buttonsState[MouseButtons::MouseWheel] = (char)m_iWheelDelta;
		m_iWheelDelta = 0;
	}
	else
	{
		data->buttonsState[MouseButtons::MouseWheel] = 0;
	}
}

bool MouseDeviceWin32::IsAvailable()
{
	DevWin32_ResetDevicesInterator();
	String tmp = DevWin32_GetDeviceDescOfType(GUID_DEVCLASS_MOUSE);
	int count = 0;
	while (!tmp.IsEmpty())
	{
		deviceString = tmp;
		//DEBUG_OUT("Mouse #%d: %s", i, tmp.c_str());
		count++;
		tmp = DevWin32_GetDeviceDescOfType(GUID_DEVCLASS_MOUSE);
	}
	DevWin32_ResetDevicesInterator();

	if (count > 1)
		deviceString = "Multiple Mice";

	return deviceString.Length() > 0;
}

String	MouseDeviceWin32::GetDescription()
{
	return "Win32 Mouse Device";
}

void MouseDeviceWin32::OnWindowMessage(unsigned int msg, unsigned int wParam, unsigned int lParam)
{
	if(msg == WM_MOUSEWHEEL)
	{
		m_iWheelDelta = (short)HIWORD(wParam) / WHEEL_DELTA;
	}
}


#endif