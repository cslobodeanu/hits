#include "medevice.h"
#include "meio.h"
#include "input/inputdevice.h"

using namespace MorganaEngine::Base::Devices;

__implement_class_abstract(MEDevice, MEObject);

MEDevice::MEDevice() : MEObject()
{
	deviceString = "";
	deviceType = 0xff;
	deviceSubType = 0xff;
	isConnected = false;
}

MEDevice::~MEDevice()
{

}

void MEDevice::OnUpdate()
{

}

bool MEDevice::IsAvailable()
{
	return false;
}

String MEDevice::GetDescription()
{
	return "ME Device";
}

String MEDevice::GetDeviceString()
{
	return deviceString;
}

const unsigned char MEDevice::GetType()
{
	return deviceType;
}

const unsigned char MEDevice::GetSubType()
{
	return deviceSubType;
}

void MEDevice::Connect(const DeviceContext_t& dc)
{

}

void MEDevice::PostConnect(const DeviceContext_t& dc)
{

}

