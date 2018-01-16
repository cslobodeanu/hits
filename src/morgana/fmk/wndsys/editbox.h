#ifndef __MORGANA_FRAMEWORK_WNDSYS_EDIT_BOX_H__
#define __MORGANA_FRAMEWORK_WNDSYS_EDIT_BOX_H__

#include "widget.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace WindowSystem
		{
			class EditBox : public Widget
			{
				__declare_morgana_script(EditBox, Widget);
			public:
				EditBox();
				virtual ~EditBox();
			protected:

				virtual void Start();
				virtual void Update();
			};
		}
	}
}

#endif