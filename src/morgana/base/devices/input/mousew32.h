#ifndef __MORGANA_BASE_DEVICES_INPUT_MOUSEW32_H__
#define __MORGANA_BASE_DEVICES_INPUT_MOUSEW32_H__

#include "platform.h"

#ifdef OS_WINDOWS_WIN32

#include "inputdevice.h"
#include "guiddef.h"

#include <Windows.h>

namespace MorganaEngine
{
	namespace Base
	{
		namespace Devices
		{
			namespace InputDevices
			{
				class MouseDeviceWin32 : public InputDevice
				{
					__declare_class(MouseDeviceWin32, InputDevice);

					int				m_iWheelDelta;
					virtual void	OnWindowMessage(unsigned int msg, unsigned int wParam, unsigned int lParam);
				public:

					MouseDeviceWin32();
					virtual ~MouseDeviceWin32();

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