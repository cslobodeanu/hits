#ifndef __MORGANA_BASE_DEVICES_INPUT_WACOM_PEN_W32_H__
#define __MORGANA_BASE_DEVICES_INPUT_WACOM_PEN_W32_H__

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
				class WacomPenWin32 : public InputDevice
				{
					__declare_class(WacomPenWin32, InputDevice);

					virtual void    OnWindowMessage(unsigned int msg, unsigned int wParam, unsigned int lParam);
					bool			inited;
					bool			isAvailable;
					int				maxPressure;
					void			Initialize();
				public:

					WacomPenWin32();
					virtual ~WacomPenWin32();

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