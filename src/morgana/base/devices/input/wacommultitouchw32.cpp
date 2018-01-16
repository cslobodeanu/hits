#include "wacommultitouchw32.h"

#ifdef OS_WINDOWS_WIN32

#include "../../views/mview.h"
#include "mio.h"

#include "wacom/msgpack.h"
#include "wacom/WINTAB.H"
#define PACKETDATA	(PK_X | PK_Y | PK_BUTTONS | PK_NORMAL_PRESSURE | PK_CURSOR)
#define PACKETMODE	PK_BUTTONS
#include "wacom/pktdef.h"
#include "wacom/WacomUtils.h"
#include "wacom/WacomMultiTouch.h"


using namespace Morgana::Base::Devices::InputDevices;
using namespace Morgana::Base::Views;

__implement_class(WacomMultiTouchWin32, InputDevice);

extern void InitializeWacom();
extern void ReleaseWacom();
extern String wacomDevName;
extern HCTX wacomContext;

WacomMultiTouchWin32::WacomMultiTouchWin32() : InputDevice(MDEVICE_INPUT_MULTI_POINTING)
{
	inited = false;
	isAvailable = false;
}

WacomMultiTouchWin32::~WacomMultiTouchWin32()
{
	ReleaseWacom();
}

void WacomMultiTouchWin32::OnUpdate()
{
	if (data->pointersCount > 0)
		DEBUG_OUT("Pointers count %d", data->pointersCount);
	Super::OnUpdate();
	data->pointersCount = 0;
}

bool WacomMultiTouchWin32::IsAvailable()
{
	Initialize();
	return isAvailable;
}

String	WacomMultiTouchWin32::GetDescription()
{
	return deviceString;
}

void WacomMultiTouchWin32::Connect()
{
	Initialize();
	Super::Connect();
}

void WacomMultiTouchWin32::OnWindowMessage(unsigned int msg, unsigned int wParam, unsigned int lParam)
{
	switch (msg)
	{
		case WM_ACTIVATE:

		/* if switching in the middle, disable the region */
		/*if (hCtx2)
		{
			gpWTEnable(hCtx2, GET_WM_ACTIVATE_STATE(wParam, lParam));
			if (hCtx2 && GET_WM_ACTIVATE_STATE(wParam, lParam))
			{
				gpWTOverlap(hCtx2, TRUE);
			}
		}*/
		break;

		case WM_FINGERDATA:
		{
			int count = ((WacomMTFingerCollection*)lParam)->FingerCount;
			WacomMTFinger* fingers = ((WacomMTFingerCollection*)lParam)->Fingers;

			for (int i = 0; i < count; i++)
			{
				POINT p = { (LONG)fingers[i].X, (LONG)fingers[i].Y };
				::ScreenToClient((HWND)MView::main->GetHandle(), &p);

				int id = fingers[i].FingerID;

				data->xAxis[id] = ((float)p.x / (float)MView::main->width) * 2.0f - 1.0f;
				data->yAxis[id] = ((float)p.y / (float)MView::main->height) * 2.0f - 1.0f;

				data->buttonsState[id] = false;

				if(fingers[i].TouchState == WacomMTFingerState::WMTFingerStateDown)
					data->buttonsState[id] = true;

				if (fingers[i].TouchState == WacomMTFingerState::WMTFingerStateUp)
					data->buttonsState[id] = false;

				if (fingers[i].TouchState == WacomMTFingerState::WMTFingerStateHold)
					data->buttonsState[id] = true;
			}

			data->pointersCount = count;

			break;
		}
	}
}

void AttachCallback(WacomMTCapability deviceInfo, void *userRef)
{
	WacomMTRegisterFingerReadHWND(deviceInfo.DeviceID, WMTProcessingModeNone, (HWND)MView::main->GetHandle(), 5);
}

void DetachCallback(int deviceID, void *userRef)
{
	WacomMTUnRegisterFingerReadHWND((HWND)MView::main->GetHandle());
}

void WacomMultiTouchWin32::Initialize()
{
	if (inited) return;

	InitializeWacom();
	isAvailable = wacomContext != NULL;

	if (isAvailable)
	{
		deviceString = wacomDevName + " MultiTouch";
	}

	WacomMTError res = WacomMTInitialize(WACOM_MULTI_TOUCH_API_VERSION);
	if (res == WMTErrorSuccess)
	{
		int deviceCount = WacomMTGetAttachedDeviceIDs(NULL, 0);
		if (deviceCount)
		{
			isAvailable = true;
			WacomMTRegisterAttachCallback(AttachCallback, NULL);
			WacomMTRegisterDetachCallback(DetachCallback, NULL);
		}
	}

	inited = true;
}

#endif
