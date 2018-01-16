#ifndef __MORGANA_BASE_DEVICES_INPUT_KEYBOARDW32_H__
#define __MORGANA_BASE_DEVICES_INPUT_KEYBOARDW32_H__

#include "platform.h"

#ifdef OS_WINDOWS_WIN32

#include "guiddef.h"
#include "inputdevice.h"
#include <Windows.h>

namespace MorganaEngine
{
	namespace Base
	{
		namespace Devices
		{
			namespace InputDevices
			{
				class KeyboardDeviceWin32 : public InputDevice
				{
					__declare_class(KeyboardDeviceWin32, InputDevice);
					virtual void	OnWindowMessage(unsigned int msg, unsigned int wParam, unsigned int lParam);
				public:

					KeyboardDeviceWin32();
					virtual ~KeyboardDeviceWin32();

					virtual void	OnUpdate();
					virtual bool	IsAvailable();

					virtual String	GetDescription();
				};
			}
		}
	}
}

#endif

#endif