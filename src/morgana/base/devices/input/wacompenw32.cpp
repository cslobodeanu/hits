#include "wacompenw32.h"


#ifdef OS_WINDOWS_WIN32

#include "../../views/mview.h"
#include "mio.h"

#include "wacom/msgpack.h"
#include "wacom/WINTAB.H"
#define PACKETDATA	(PK_X | PK_Y | PK_BUTTONS | PK_NORMAL_PRESSURE | PK_CURSOR)
#define PACKETMODE	PK_BUTTONS
#include "wacom/pktdef.h"
#include "wacom/WacomUtils.h"


using namespace Morgana::Base::Devices::InputDevices;
using namespace Morgana::Base::Views;

__implement_class(WacomPenWin32, InputDevice);

LOGCONTEXT	glogContext = { 0 };
char* gpszProgramName = "MorganaWacomSupport";

HCTX	wacomContext = null;
int		wacomReferences = 0;
String	wacomDevName = "NotAvailable";

void InitializeWacom();
void ReleaseWacom();

WacomPenWin32::WacomPenWin32() : InputDevice(MDEVICE_INPUT_SINGLE_POINTING_PRESSURE)
{
	inited = false;
	isAvailable = false;
	maxPressure = 0;
}

WacomPenWin32::~WacomPenWin32()
{
	ReleaseWacom();
}

void WacomPenWin32::OnUpdate()
{
	Super::OnUpdate();
}

bool WacomPenWin32::IsAvailable()
{
	Initialize();
	return isAvailable;
}

String	WacomPenWin32::GetDescription()
{
	return deviceString;
}

void WacomPenWin32::Connect()
{
	Initialize();
	Super::Connect();
}

void WacomPenWin32::OnWindowMessage(unsigned int msg, unsigned int wParam, unsigned int lParam)
{
	PACKET pkt;

	switch (msg)
	{
	case WM_ACTIVATE:

		/* if switching in the middle, disable the region */
		if (wacomContext)
		{
			gpWTEnable(wacomContext, GET_WM_ACTIVATE_STATE(wParam, lParam));
			if (wacomContext && GET_WM_ACTIVATE_STATE(wParam, lParam))
			{
				gpWTOverlap(wacomContext, TRUE);
			}
		}
		break;
	case WT_PACKET:
		if (gpWTPacket((HCTX)lParam, wParam, &pkt))
		{
			POINT p;
			p.x = pkt.pkX;
			p.y = pkt.pkY;

			::ScreenToClient((HWND)MView::main->GetHandle(), &p);
			
			data->xAxis[0] = ((float)p.x / (float)MView::main->width) * 2.0f - 1.0f;
			data->yAxis[0] = ((float)p.y / (float)MView::main->height) * 2.0f - 1.0f;

			data->buttonsState[0] = (1023 * pkt.pkNormalPressure) / maxPressure; // HIWORD(pkt.pkButtons) == TBN_DOWN;

			DEBUG_OUT("Wacom (%d, %d) pressure: %d", pkt.pkX, pkt.pkY, data->buttonsState[0]);
		}
		break;
	}
}

HCTX _initTablet(HWND hWnd)
{
	HCTX hctx = NULL;
	UINT wDevice = 0;
	UINT wExtX = 0;
	UINT wExtY = 0;
	UINT wWTInfoRetVal = 0;
	AXIS TabletX = { 0 };
	AXIS TabletY = { 0 };

	// Set option to move system cursor before getting default system context.
	glogContext.lcOptions |= CXO_SYSTEM;

	// Open default system context so that we can get tablet data
	// in screen coordinates (not tablet coordinates).
	wWTInfoRetVal = gpWTInfoA(WTI_DEFSYSCTX, 0, &glogContext);
	WACOM_ASSERT(wWTInfoRetVal == sizeof(LOGCONTEXT));

	WACOM_ASSERT(glogContext.lcOptions & CXO_SYSTEM);

	// modify the digitizing region
	wsprintf(glogContext.lcName, "PrsTest Digitizing %x", ::GetModuleHandle(NULL));

	// We process WT_PACKET (CXO_MESSAGES) messages.
	glogContext.lcOptions |= CXO_MESSAGES;

	// What data items we want to be included in the tablet packets
	glogContext.lcPktData = PACKETDATA;

	// Which packet items should show change in value since the last
	// packet (referred to as 'relative' data) and which items
	// should be 'absolute'.
	glogContext.lcPktMode = PACKETMODE;

	// This bitfield determines whether or not this context will receive
	// a packet when a value for each packet field changes.  This is not
	// supported by the Intuos Wintab.  Your context will always receive
	// packets, even if there has been no change in the data.
	glogContext.lcMoveMask = PACKETDATA;

	// Which buttons events will be handled by this context.  lcBtnMask
	// is a bitfield with one bit per button.
	glogContext.lcBtnUpMask = glogContext.lcBtnDnMask;

	// Set the entire tablet as active
	wWTInfoRetVal = gpWTInfoA(WTI_DEVICES + 0, DVC_X, &TabletX);
	WACOM_ASSERT(wWTInfoRetVal == sizeof(AXIS));

	wWTInfoRetVal = gpWTInfoA(WTI_DEVICES, DVC_Y, &TabletY);
	WACOM_ASSERT(wWTInfoRetVal == sizeof(AXIS));

	glogContext.lcInOrgX = 0;
	glogContext.lcInOrgY = 0;
	glogContext.lcInExtX = TabletX.axMax;
	glogContext.lcInExtY = TabletY.axMax;

	// Guarantee the output coordinate space to be in screen coordinates.  
	glogContext.lcOutOrgX = GetSystemMetrics(SM_XVIRTUALSCREEN);
	glogContext.lcOutOrgY = GetSystemMetrics(SM_YVIRTUALSCREEN);
	glogContext.lcOutExtX = GetSystemMetrics(SM_CXVIRTUALSCREEN); //SM_CXSCREEN );

	// In Wintab, the tablet origin is lower left.  Move origin to upper left
	// so that it coincides with screen origin.
	glogContext.lcOutExtY = -GetSystemMetrics(SM_CYVIRTUALSCREEN);	//SM_CYSCREEN );

	// Leave the system origin and extents as received:
	// lcSysOrgX, lcSysOrgY, lcSysExtX, lcSysExtY

	// open the region
	// The Wintab spec says we must open the context disabled if we are 
	// using cursor masks.  
	hctx = gpWTOpenA(hWnd, &glogContext, FALSE);

	WacomTrace("HCTX: %i\n", hctx);

	return hctx;
}

void InitializeWacom()
{
	if (wacomReferences == 0)
	{
		if (LoadWintab())
		{
			if (gpWTInfoA(0, 0, NULL))
			{
				wacomContext = _initTablet((HWND)MView::main->GetHandle());

				if (wacomContext != NULL)
				{
					char TabletName[50] = "";
					gpWTInfoA(WTI_DEVICES, DVC_NAME, TabletName);
					wacomDevName = TabletName;
				}
			}
		}
	}
	wacomReferences++;
}

void ReleaseWacom()
{
	if (wacomReferences == 0) return;

	wacomReferences--;

	if (wacomReferences == 0)
	{
		gpWTClose(wacomContext);
		UnloadWintab();
	}
}

void WacomPenWin32::Initialize()
{
	if (inited) return;

	InitializeWacom();
	isAvailable = wacomContext != NULL;

	if (isAvailable)
	{
		deviceString = wacomDevName + " Pen";
		AXIS axis;
		gpWTInfoA(WTI_DEVICES, DVC_NPRESSURE, &axis);
		maxPressure = axis.axMax;
	}

	inited = true;
}

#endif
