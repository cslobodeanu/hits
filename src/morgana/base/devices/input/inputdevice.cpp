#include "inputdevice.h"

#include "../../views/meview.h"

using namespace MorganaEngine::Base::Devices::InputDevices;
using namespace MorganaEngine::Base::Views;


__implement_class_abstract(InputDevice, MEDevice);

InputDevice::InputDevice(const unsigned char subtype) : MEDevice()
{
	deviceType = MDEVICE_INPUT;
	deviceSubType = subtype;

	data = new InputDeviceData_t();
	memset(data, 0, sizeof(InputDeviceData_t));

	prevData = new InputDeviceData_t();
	memset(prevData, 0, sizeof(InputDeviceData_t));
}

InputDevice::~InputDevice()
{
	SAFE_DEL(data);
	SAFE_DEL(prevData);
}

const InputDevice::InputDeviceData_t* InputDevice::GetData() const
{
	return data;
}

const InputDevice::InputDeviceData_t* InputDevice::GetPrevData() const
{
	return prevData;
}

void InputDevice::CopyDataToPrevious()
{
	memcpy(prevData, data, sizeof(InputDeviceData_t));
}

void InputDevice::OnUpdate()
{
	CopyDataToPrevious();
}

void InputDevice::ResetButtons()
{
	memset(data->buttonsState, 0, InputDeviceData_t::MAX_BUTTONS * sizeof(data->buttonsState[0]));
}

void InputDevice::OnWindowMessage(unsigned int msg, unsigned int wParam, unsigned int lParam)
{

}

void InputDevice::Connect(const DeviceContext_t& dc)
{
	super::Connect(dc);

	MEView::main->OnWindowMessage += Callback(OnWindowMessage, unsigned int, unsigned int, unsigned int);
}

