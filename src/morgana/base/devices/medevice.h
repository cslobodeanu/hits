#ifndef __MORGANA_BASE_DEVICES_MDEVICE_H__
#define __MORGANA_BASE_DEVICES_MDEVICE_H__

#include "reflection.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Devices
		{
			class MEDevice : public MEObject
			{
				__declare_class_abstract(MEDevice, MEObject);
			protected:

				String			deviceString;

				unsigned char	deviceType;
				unsigned char	deviceSubType;

			public:

				struct DeviceContext_t
				{
					void*			dummy;
					unsigned long	viewHandle;
				};

				typedef enum
				{
					MDEVICE_INPUT,
					MDEVICE_NETWORK,
					MDEVICE_DISPLAY,
					MDEVICE_SOUND,
					MDEVICE_TYPES_COUNT
				} DeviceType_t;

				MEDevice();
				virtual ~MEDevice();

				virtual void	Connect(const DeviceContext_t& dc);
				virtual void	PostConnect(const DeviceContext_t& dc);

				virtual bool	IsAvailable();
				virtual void	OnUpdate();

				virtual String	GetDescription();
				String			GetDeviceString();

				const unsigned char GetType();
				const unsigned char GetSubType();

				bool			isConnected;
			};
		}
	}
}

#endif