#ifndef __MORGANA_BASE_VIEWS_MEVIEWW32_H__
#define __MORGANA_BASE_VIEWS_MEVIEWW32_H__

#include "platform.h"

#ifdef OS_WINDOWS_WIN32

#include "meview.h"
#include <Windows.h> 

namespace MorganaEngine
{
	namespace Base
	{
		namespace Views
		{
			class MEViewWin32 : public MEView
			{
				__declare_class(MEViewWin32, MEView);

			protected:
				virtual void	Create();
				virtual void	Destroy();
				virtual void	ResizeReposition();

				static LRESULT	WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
				virtual void	OnAppRunFrameCallback(void);

			public:

				MEViewWin32();
				virtual ~MEViewWin32();

				virtual void Close();
				virtual void Show(const bool show = true);

				virtual void OnPropertySet(_propertyDef* p);

				static unsigned long	MECreateWindow(const char* title = "dummy");
				static void				MEDestroyWindow(unsigned long handle);
			};
		}
	}
}

#endif

#endif