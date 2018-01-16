#ifndef __MORGANA_BASE_DEVICES_DISPLAY_COMMON_OGLW32_DEVICE_H__
#define __MORGANA_BASE_DEVICES_DISPLAY_COMMON_OGLW32_DEVICE_H__

#include "platform.h"

#ifdef OS_WINDOWS_WIN32

#include "common_ogl_device.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Devices
		{
			namespace Display
			{
				namespace OpenGL
				{
					class CommonOpenGLWin32Device : public CommonOpenGLDevice
					{
						__declare_class(CommonOpenGLWin32Device, CommonOpenGLDevice);

						typedef struct 
						{
							unsigned long	wnd;
							unsigned long	hdc;
							unsigned long	hrc;
						} ContextInfo_t;

						ContextInfo_t contextInfo;
						void		CreateContext(ContextInfo_t* ci);
						void		DestroyContext(ContextInfo_t* ci);

					public:

						CommonOpenGLWin32Device();
						virtual ~CommonOpenGLWin32Device();

						virtual void Connect(const DeviceContext_t& dc);
						virtual bool IsAvailable();

						virtual void PresentBuffer();
					};
				}
			}
		}
	}
}

#endif

#endif