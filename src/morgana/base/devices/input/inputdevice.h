#ifndef __MORGANA_BASE_DEVICES_INPUT_INPUTDEVICE_H__
#define __MORGANA_BASE_DEVICES_INPUT_INPUTDEVICE_H__

#include "../medevice.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Devices
		{
			namespace InputDevices
			{
				class InputDevice : public MEDevice
				{
					__declare_class_abstract(InputDevice, MEDevice);

				public:

					typedef enum
					{
						MDEVICE_INPUT_KEYBOARD,
						MDEVICE_INPUT_SINGLE_POINTING,
						MDEVICE_INPUT_SINGLE_POINTING_PRESSURE,
						MDEVICE_INPUT_MULTI_POINTING,
						MDEVICE_INPUT_MULTI_POINTING_PRESSURE,
						MDEVICE_INPUT_JOYSTICK,
						MDEVICE_INPUT_GAMEPAD,
						MDEVICE_INPUT_ACCELEROMETER,
						MDEVICE_INPUT_GYROSCOPE,
						MDEVICE_INPUT_OCULUS_RIFT // :P
					} InputDeviceSubType_t;

					InputDevice(const unsigned char subtype);
					virtual ~InputDevice();

					typedef struct
					{
						static const int		MAX_POINTERS = 16;
						static const int		MAX_BUTTONS = 32;
						static const int		MAX_KEYS = 256;
						unsigned char			pointersCount;
						float					xAxis[MAX_POINTERS];
						float					yAxis[MAX_POINTERS];
						float					zAxis[MAX_POINTERS];
						short					buttonsState[MAX_BUTTONS];
						bool					keysState[MAX_KEYS];
					} InputDeviceData_t;

					const InputDeviceData_t*	GetData() const;
					const InputDeviceData_t*	GetPrevData() const;

					virtual void				OnUpdate();

					virtual void				Connect(const DeviceContext_t& dc);

				protected:

					InputDeviceData_t			*data, *prevData;
					void						CopyDataToPrevious();

					void						ResetButtons();

					virtual void				OnWindowMessage(unsigned int msg, unsigned int wParam, unsigned int lParam);
				};
			}
		}
	}
}

#endif