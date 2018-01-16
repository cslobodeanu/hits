#include "medevices.h"
#include "meio.h"
#include "input/inputdevice.h"
#include "display/displaydevice.h"
#include "sound/sounddevice.h"

using namespace MorganaEngine::Base::Devices;

__implement_class(MEDevices, MEObject);

MEDevices::MEDevices() : MEObject()
{
	for(int i = 0; i < MEDevice::MDEVICE_TYPES_COUNT; i++)
		baseClasses[i] = NULL;
	baseClasses[MEDevice::MDEVICE_INPUT] = (_CLASSDESC*)InputDevices::InputDevice::AsClassDesc;
	baseClasses[MEDevice::MDEVICE_DISPLAY] = (_CLASSDESC*)Display::DisplayDevice::AsClassDesc;
	baseClasses[MEDevice::MDEVICE_SOUND] = (_CLASSDESC*)Sound::SoundDevice::AsClassDesc;
}

MEDevices::~MEDevices()
{
	DisconnectAllDevices();
}

void MEDevices::EnumerateAllDevices( char* list /*= NULL*/ )
{
	DEBUG_OUT("---- Devices ----");
		for(int i = 0; i < MEDevice::MDEVICE_TYPES_COUNT; i++)
			EnumerateDevices(i);
	DEBUG_OUT("---- Devices end ----");
}

void MEDevices::EnumerateDevices( const unsigned char type )
{
	CLASSDESC baseCD = baseClasses[type];
	if(baseCD == NULL) return;

	Array<CLASSDESC>	my_devices_classes = _Reflection.GetClassesOfType(baseCD);

	if(my_devices_classes.Length())
		DEBUG_OUT("---- %s list ----", baseCD->className);

	for(int i = 0; i < my_devices_classes.Length(); i++)
	{
		CLASSDESC dev_cd = my_devices_classes[i];
		MEDevice* dev = (MEDevice*)MEObject::Instantiate(dev_cd);
		if(dev->IsAvailable())
		{
			DEBUG_OUT("[%s] as [%s]", dev_cd->className, dev->GetDeviceString());
		}
		SAFE_DEL(dev);
	}
}

void MEDevices::ConnectAllDevices(const MEDevice::DeviceContext_t& dc)
{
	DEBUG_OUT("\nConnecting all devices ...");

	for(int i = 0; i < MEDevice::MDEVICE_TYPES_COUNT; i++)
		ConnectDevices(dc, i);
}

void MEDevices::ConnectDevices(const MEDevice::DeviceContext_t& dc, const unsigned char type)
{
	DisconnectDevices(type);

	_CLASSDESC* baseCD = baseClasses[type];
	if(baseCD == NULL) return;

	Array<CLASSDESC>	my_devices_classes = _Reflection.GetClassesOfType(baseCD);

	for(int i = 0; i < my_devices_classes.Length(); i++)
	{
		CLASSDESC dev_cd = my_devices_classes[i];
		MEDevice* dev = (MEDevice*)MEObject::Instantiate(dev_cd);
		if(dev->IsAvailable())
		{
			dev->Connect(dc);
			dev->isConnected = true;
			DEBUG_OUT("[%s] connected.", dev->GetDeviceString().c_str());
			connectedDevices[type].Add(dev);

			OnDeviceAdded(dev);

			dev->PostConnect(dc);
		}
	}
}

void MEDevices::DisconnectDevices( const unsigned char type )
{
	for(int i = 0; i < connectedDevices[type].Length(); i++)
	{
		MEDevice* dev = connectedDevices[type][i];

		OnDeviceLost(dev);

		String cd = dev->GetClassDesc()->className;
		SAFE_DEL(dev);
		DEBUG_OUT("[%s] disconnected.", cd.c_str());
	}

	connectedDevices[type].Alloc(0);
}

void MEDevices::DisconnectAllDevices()
{
	for(int i = 0; i < MEDevice::MDEVICE_TYPES_COUNT; i++)
		DisconnectDevices(i);
}

void MEDevices::UpdateAllDevices()
{
	for(int i = 0; i < MEDevice::MDEVICE_TYPES_COUNT; i++)
	{
		for(int j = 0; j < connectedDevices[i].Length(); j++)
		{
			MEDevice* dev = connectedDevices[i][j];
			dev->OnUpdate();
		}
	}
}

MEDevice* MEDevices::GetDevice( const unsigned char& type, const unsigned char& subtype ) // eventually faster
{
	for(int i = 0; i < connectedDevices[type].Length(); i++)
	{
		MEDevice* dev = connectedDevices[type][i];
		if(dev->GetSubType() == subtype)
			return dev;
	}

	return NULL;
}
