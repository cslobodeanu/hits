#include "common_oglw32_device.h"

#ifdef OS_WINDOWS_WIN32

#include "oglheaders.h"
#include "../../../views/mevieww32.h"

#pragma comment(lib, "opengl32.lib")

using namespace MorganaEngine::Base::Devices::Display::OpenGL;
using namespace MorganaEngine::Base::Devices::Display;
using namespace MorganaEngine::Base::Views;

__implement_class(CommonOpenGLWin32Device, CommonOpenGLDevice);

CommonOpenGLWin32Device::CommonOpenGLWin32Device() : CommonOpenGLDevice()
{
	memset(&contextInfo, 0, sizeof(ContextInfo_t));
}

CommonOpenGLWin32Device::~CommonOpenGLWin32Device()
{
	DestroyContext(&contextInfo);
}

void CommonOpenGLWin32Device::Connect(const DeviceContext_t& dc)
{
	contextInfo.wnd = dc.viewHandle;
	CreateContext(&contextInfo);
}

bool CommonOpenGLWin32Device::IsAvailable()
{
	ContextInfo_t dummy;
	dummy.wnd = MEViewWin32::MECreateWindow("CommonOpenGLWin32Device");
	CreateContext(&dummy);
	const bool ret = dummy.hrc != 0 && dummy.hdc != 0;
	if(ret)
	{
		deviceString = String((const char*)glGetString(GL_RENDERER));
	}
	DestroyContext(&dummy);
	MEViewWin32::MEDestroyWindow(dummy.wnd);
	return ret;
}

void CommonOpenGLWin32Device::CreateContext(ContextInfo_t* ci)
{
	if (ci->wnd == 0) return;
	ci->hdc = 0;
	ci->hrc = 0;

	ci->hdc = (unsigned long)::GetDC((HWND)ci->wnd);
	if(ci->hdc == 0) return;

	PIXELFORMATDESCRIPTOR pfd =											// pfd Tells Windows How We Want Things To Be
	{
		sizeof (PIXELFORMATDESCRIPTOR),									// Size Of This Pixel Format Descriptor
		1,																// Version Number
		PFD_DRAW_TO_WINDOW |											// Format Must Support Window
		PFD_SUPPORT_OPENGL |											// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,												// Must Support Double Buffering
		PFD_TYPE_RGBA,													// Request An RGBA Format
		32,																// Select Our Color Depth
		0, 0, 0, 0, 0, 0,												// Color Bits Ignored
		0,																// No Alpha Buffer
		0,																// Shift Bit Ignored
		0,																// No Accumulation Buffer
		0, 0, 0, 0,														// Accumulation Bits Ignored
		24,																// 24Bit Z-Buffer (Depth Buffer)  
		8,																// 8Bit Stencil Buffer
		0,																// No Auxiliary Buffer
		PFD_MAIN_PLANE,													// Main Drawing Layer
		0,																// Reserved
		0, 0, 0															// Layer Masks Ignored
	};

	GLuint pf = ::ChoosePixelFormat((HDC)ci->hdc, &pfd);
	if(pf == 0)
	{
		DestroyContext(ci);
		return;
	}

	if(::SetPixelFormat((HDC)ci->hdc, pf, &pfd) == FALSE)		// Try To Set The Pixel Format
	{
		DestroyContext(ci);
		return;
	}

	ci->hrc = (unsigned long)wglCreateContext ((HDC)ci->hdc);					// Try To Get A Rendering Context
	if(ci->hrc == 0)												// Did We Get A Rendering Context?
	{
		DestroyContext(ci);
		return;
	}

	if(wglMakeCurrent((HDC)ci->hdc, (HGLRC)ci->hrc) == FALSE)
	{
		DestroyContext(ci);
		return;
	}

}

void CommonOpenGLWin32Device::DestroyContext(ContextInfo_t* ci)
{
	if (ci->hdc != 0)
	{
		wglMakeCurrent((HDC)ci->hdc, 0);						// Set The Current Active Rendering Context To Zero
		if (ci->hrc != 0)										// Does The Window Have A Rendering Context?
		{
			wglDeleteContext((HGLRC)ci->hrc);					// Release The Rendering Context
			ci->hrc = 0;										// Zero The Rendering Context

		}
		::ReleaseDC((HWND)ci->wnd, (HDC)ci->hdc);
		ci->hdc = 0;
	}

}

void CommonOpenGLWin32Device::PresentBuffer()
{
	::SwapBuffers((HDC)contextInfo.hdc);
}

#endif
