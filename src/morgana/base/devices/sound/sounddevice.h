#ifndef __MORGANA_BASE_DEVICES_SOUND_SOUNDDEVICE_H__
#define __MORGANA_BASE_DEVICES_SOUND_SOUNDDEVICE_H__

#include "../medevice.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Devices
		{
			namespace Sound
			{
				class SoundDevice : public MEDevice
				{
					__declare_class_abstract(SoundDevice, MEDevice);

				public:

					typedef enum
					{
						MDEVICE_SOUND_SPEAKER,
						MDEVICE_SOUND_MICROPHONE
					} SoundDeviceSubType_t;

					SoundDevice(const unsigned char subtype);
					virtual ~SoundDevice();

				protected:

				};
			}
		}
	}
}

#endif