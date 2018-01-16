#ifndef __MORGANA_FMK_SCENE_LIGHTS_LIGHT_H__
#define __MORGANA_FMK_SCENE_LIGHTS_LIGHT_H__

#include "metypes.h"
#include "gameobjects.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Scene
		{
			namespace Lights
			{
				class Light : public Component
				{
					__declare_component(Light, Component);

				public:
					Light();
					virtual ~Light();

					enum
					{
						AmbientLight = 0,
						DirectionalLight,
						PointLight,
						SpotLight
					};
					
					_property<Color>		color;
					_property<int>			priority;
					_property<float>		radius;
					_property<int>			type;
					_property<bool>			drawVolume;
					_property<Vector3>		attenuation;

				protected:
					virtual void	OnPropertySet(_propertyDef* atr);
					virtual void	DoAddProperties();
					void			UpdateRadius();
				};

				typedef Array<Light*> LightList;
			}
		}
	}
}

#endif