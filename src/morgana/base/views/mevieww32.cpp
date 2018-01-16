#include "platform.h"

#ifdef OS_WINDOWS_WIN32

#include "mevieww32.h"
#include "../../fmk/application/meapplication.h"

using namespace MorganaEngine::Framework::Application;

using namespace MorganaEngine::Base::Views;

__implement_class(MEViewWin32, MEView);

#define __my_handle		((HWND)handle)

MEViewWin32::MEViewWin32() : MEView()
{
	Create();
	MEApplication::main->OnRunFrame += Callback(OnAppRunFrameCallback);
}

MEViewWin32::~MEViewWin32()
{

};

void MEViewWin32::Close()
{

}

void MEViewWin32::Show(const bool show /*= true*/)
{
	super::Show(show);
	if(isMaximized && show)
	{
		::ShowWindow(__my_handle, SW_SHOWMAXIMIZED);
	}
	else
	{
		::ShowWindow(__my_handle, show ? SW_SHOW : SW_HIDE);
	}
}

void MEViewWin32::ResizeReposition()
{
	if (isMaximized == false)
	{
		RECT client;
		client.left = 0;
		client.right = width;
		client.top = 0;
		client.bottom = height;

		const int style = ::GetWindowLongPtr(__my_handle, GWL_STYLE);
		::AdjustWindowRect(&client, style, 0);

		::SetWindowPos(__my_handle, NULL, left, top, client.right - client.left, client.bottom - client.top, 0);
	}
	else
		::ShowWindow(__my_handle, SW_SHOWMAXIMIZED);
}

LRESULT MEViewWin32::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	MEViewWin32* myself = (MEViewWin32*)::GetWindowLongPtr(hwnd, GWL_USERDATA);

	if(msg == WM_SIZE)
	{
		if(wParam == SIZE_MAXIMIZED)
		{
			myself->isMaximized = true;
		}

		if(wParam == SIZE_RESTORED)
		{
			myself->isMaximized = false;
		}

// 		myself->m_iWidth = (int)LOWORD(lParam);
// 		myself->m_iHeight = (int)HIWORD(lParam);
	}

	if(msg == WM_WINDOWPOSCHANGED)
	{
		WINDOWPOS* wp = (WINDOWPOS*)lParam;
		if(myself->isMaximized == false)
		{
			myself->left = wp->x;
			myself->top = wp->y;

			RECT cr;
			::GetClientRect(hwnd, &cr);
			myself->width = cr.right - cr.left;
			myself->height = cr.bottom - cr.top;

			//DEBUG_OUT("Window width %d", (int)myself->m_iWidth);
		}
	}

	if(msg == WM_CLOSE)
	{
		MEApplication::main->NotifyShutdown();
		return 0;
	}

	if (myself)
		myself->OnWindowMessage(msg, wParam, lParam);

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

#define __MORGANA_WND_CLASS_WIN32		"MorganaWindowClass"

void MEViewWin32::Create()
{
	WNDCLASSEX windowClass;
	ZeroMemory (&windowClass, sizeof (WNDCLASSEX));
	windowClass.cbSize			= sizeof (WNDCLASSEX);
	windowClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
	windowClass.lpfnWndProc		= (WNDPROC)(WndProc);
	windowClass.hInstance		= GetModuleHandle(NULL);				// Set The Instance
	windowClass.hbrBackground	= (HBRUSH)(COLOR_BACKGROUND);			// Class Background Brush Color
	windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	windowClass.lpszClassName	= __MORGANA_WND_CLASS_WIN32;				// Sets The Applications Classname
	windowClass.hIcon			= (HICON)LoadImage(NULL, "res/morgana.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
	RegisterClassEx (&windowClass);
	
	DWORD windowStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_SIZEBOX | WS_MAXIMIZEBOX;
	DWORD windowExtendedStyle = 0;

	RECT windowRect = {0, 0, width, height};

	AdjustWindowRectEx (&windowRect, windowStyle, 0, windowExtendedStyle);
	
	handle = (unsigned long)CreateWindowEx (windowExtendedStyle,
		__MORGANA_WND_CLASS_WIN32,
		caption.ToString(),
		windowStyle,
		0, 0,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		HWND_DESKTOP,						
		0,									
		GetModuleHandle(NULL),
		NULL);

	::SetWindowLongPtr(__my_handle, GWL_USERDATA, (LONG_PTR)this);
}

unsigned long MEViewWin32::MECreateWindow(const char* title /*= "dummy"*/)
{
	WNDCLASSEX windowClass;
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
	windowClass.lpfnWndProc = NULL;
	windowClass.hInstance = GetModuleHandle(NULL);				// Set The Instance
	windowClass.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);			// Class Background Brush Color
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	windowClass.lpszClassName = __MORGANA_WND_CLASS_WIN32;				// Sets The Applications Classname
	windowClass.hIcon = (HICON)LoadImage(NULL, "res/morgana.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
	RegisterClassEx(&windowClass);

	DWORD windowStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_SIZEBOX | WS_MAXIMIZEBOX;
	DWORD windowExtendedStyle = 0;

	RECT windowRect = { 0, 0, 10, 10 };

	AdjustWindowRectEx(&windowRect, windowStyle, 0, windowExtendedStyle);

	HWND handle = CreateWindowEx(windowExtendedStyle,
		__MORGANA_WND_CLASS_WIN32,
		title,
		windowStyle,
		0, 0,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		HWND_DESKTOP,
		0,
		GetModuleHandle(NULL),
		NULL);
	return (unsigned long)handle;
}

void MEViewWin32::MEDestroyWindow(unsigned long handle)
{
	::DestroyWindow((HWND)handle);
}

void MEViewWin32::OnPropertySet(_propertyDef* p)
{
	super::OnPropertySet(p);

	if(p == &caption)
	{
		::SetWindowText(__my_handle, caption.ToString());
	}

	if(p == &left || p == &top || p == &width || p == &height)
	{
		WriteToRegistry();
	}
}

#endif

void MEViewWin32::Destroy()
{
	::DestroyWindow(__my_handle);
}

void MEViewWin32::OnAppRunFrameCallback(void)
{
	MSG	msg;

	if (PeekMessage (&msg, __my_handle, 0, 0, PM_REMOVE) != 0)
	{
		// Check For WM_QUIT Message
		if (msg.message != WM_QUIT)						// Is The Message A WM_QUIT Message?
		{
			TranslateMessage(&msg);
			DispatchMessage (&msg);						// If Not, Dispatch The Message
		}
	}
}
