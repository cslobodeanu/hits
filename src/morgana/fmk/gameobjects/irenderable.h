#ifndef __MORGANA_FMK_GAME_OBJECTS_RENDERABLE_INTERFACE_H__
#define __MORGANA_FMK_GAME_OBJECTS_RENDERABLE_INTERFACE_H__

#include "reflection.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace GameObjects
		{
			class IRenderable
			{
				__declare_class_abstract1(IRenderable);
			public:
				IRenderable();

				virtual void OnPreRenderObject() = 0;
				virtual void OnRenderObject() = 0;
				virtual void OnPostRenderObject() = 0;

				unsigned long rendererFlags;
			};
		}
	}
}

#endif