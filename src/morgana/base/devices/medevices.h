#ifndef __MORGANA_BASE_DEVICES_MDEVICES_H__
#define __MORGANA_BASE_DEVICES_MDEVICES_H__

#include "medevice.h"
#include "reflection.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Devices
		{
			class MEDevices : public MEObject
			{
				__declare_class(MEDevices, MEObject);
			public:
				MEDevices();
				virtual ~MEDevices();

				void			EnumerateAllDevices(char* list = NULL);
				void			EnumerateDevices(const unsigned char type);
				void			ConnectAllDevices(const MEDevice::DeviceContext_t& dc);
				void			ConnectDevices(const MEDevice::DeviceContext_t& dc, const unsigned char type);
				void			DisconnectAllDevices();
				void			DisconnectDevices(const unsigned char type);

				MEDevice*		GetDevice(const unsigned char& type, const unsigned char& subtype); // eventually faster

				Action<MEDevice*> OnDeviceLost;
				Action<MEDevice*> OnDeviceAdded;

				void			UpdateAllDevices();
			protected:

				_CLASSDESC*		baseClasses[MEDevice::MDEVICE_TYPES_COUNT];

				Array<MEDevice*>	connectedDevices[MEDevice::MDEVICE_TYPES_COUNT];
			};
		}
	}
}

#endif