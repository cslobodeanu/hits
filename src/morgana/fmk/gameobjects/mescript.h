#ifndef __MORGANA_FMK_GAME_OBJECTS_MESCRIPT_H__
#define __MORGANA_FMK_GAME_OBJECTS_MESCRIPT_H__

#include "component.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace GameObjects
		{
			class MEScript : public Component
			{
				__declare_class_abstract(MEScript, Component);
			public:

				static bool customScript;

				MEScript();
				virtual ~MEScript();
			};
		}
	}
}

#endif