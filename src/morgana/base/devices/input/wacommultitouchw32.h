#ifndef __MORGANA_BASE_DEVICES_INPUT_WACOM_MULTITOUCH_W32_H__
#define __MORGANA_BASE_DEVICES_INPUT_WACOM_MULTITOUCH_W32_H__

#include "platform.h"

#ifdef OS_WINDOWS_WIN32

#include "guiddef.h"
#include "inputdevice.h"
#include <Windows.h>

namespace Morgana
{
	namespace Base
	{
		namespace Devices
		{
			namespace InputDevices
			{
				class WacomMultiTouchWin32 : public InputDevice
				{
					__declare_class(WacomMultiTouchWin32, InputDevice);

					virtual void	OnWindowMessage(unsigned int msg, unsigned int wParam, unsigned int lParam);
					bool			inited;
					bool			isAvailable;
					void			Initialize();
				public:

					WacomMultiTouchWin32();
					virtual ~WacomMultiTouchWin32();

					virtual void	OnUpdate();
					virtual bool	IsAvailable();

					virtual void	Connect();

					virtual String	GetDescription();
				};
			}
		}
	}
}

#endif

#endif