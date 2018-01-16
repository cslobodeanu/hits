#include "sounddevice.h"

using namespace MorganaEngine::Base::Devices::Sound;

__implement_class_abstract(SoundDevice, MEDevice);

SoundDevice::SoundDevice(const unsigned char subtype) : MEDevice()
{
	deviceType = MDEVICE_SOUND;
	deviceSubType = subtype;
}

SoundDevice::~SoundDevice()
{

}