#ifndef __MORGANA_BASE_VIEWS_MVIEW_H__
#define __MORGANA_BASE_VIEWS_MVIEW_H__

#include "reflection.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Views
		{
			class MEView : public MEObject
			{
				__declare_class_abstract(MEView, MEObject);
			protected:
				unsigned long	handle;
				virtual String	GetRegistryKey();
				virtual void	ReadFromRegistry();
				virtual void	WriteToRegistry();
				virtual void	ResizeReposition();
			public:
				MEView();
				virtual ~MEView();

				void DoAddProperties();

				virtual unsigned long GetHandle();

				_property<int>		left, top, width, height;
				_property<bool>		isMaximized;
				_property<String>	caption;
				_property<String>	keyIdentifier;

				virtual void OnPropertySet(_propertyDef* p);

				virtual void Close();
				virtual void Show(const bool show = true);

				static MEView* main;

				Action<unsigned int, unsigned int, unsigned int> OnWindowMessage;
			};
		}
	}
}

#endif