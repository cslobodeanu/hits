#ifndef __MORGANA_FMK_SCENE_COMPONENTS_MATERIAL_COMPONENT_H__
#define __MORGANA_FMK_SCENE_COMPONENTS_MATERIAL_COMPONENT_H__

#include "gameobjects.h"
#include "meresources.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Resources
		{
			namespace Materials
			{
				class MaterialComponent : public Component
				{
					__declare_component(MaterialComponent, Component);
				public:
					MaterialComponent();
					virtual ~MaterialComponent();

					Material*	material;
				};
			}
		}
	}
}

#endif