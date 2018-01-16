#ifndef __MORGANA_FMK_SCENE_LIGHTS_LIGHT_COLLECTION_H__
#define __MORGANA_FMK_SCENE_LIGHTS_LIGHT_COLLECTION_H__

#include "light.h"

namespace Morgana
{
	namespace Framework
	{
		namespace Scene
		{
			namespace Lights
			{
				class LightCollection
				{
				public:
					LightCollection();
					virtual ~LightCollection();

					void				Clear();
					void				PushLight(const Light* l);
					Array<const Light*>	GetLightsForBox(const Box& b, const int maxCount = 8);
					const Light*		GetGlobalDirectionalLight();
					const Light*		GetGlobalAmbientLight();
				private:

					const Light*		ambientLight;
					const Light*		directionalLight;

					struct LightNode_t 
					{
						const Light*		light;
						LightNode_t*		parent;
						Array<LightNode_t*>	children;
						Box					bounds;

						LightNode_t(const Light* l)
						{
							parent = NULL;
							light = l;
							bounds = Box(l->transform->position, l->radius);
						}

						~LightNode_t()
						{
							for (int i = 0; i < children.Length(); i++)
								SAFE_DEL(children[i]);
						}

						void AddChild(LightNode_t* ln)
						{
							ln->parent = this;
							children.Add(ln);
							Box::Combine(bounds, ln->bounds, bounds);
							LightNode_t* p = parent;
							while (p)
							{
								p->RecalculateBounds();
								p = p->parent;
							}
						}

						void RecalculateBounds()
						{
							bounds = Box(light->transform->position, light->radius);
							for (LightNode_t** lns = children.ptr(); lns < children.ptr(); lns++)
								Box::Combine(bounds, (*lns)->bounds, bounds);
						}
					};

					Array<LightNode_t*>	roots;

					const LightNode_t*	GetClosest(const LightNode_t* ln, const LightNode_t* root);
					void				GetLightsForBox(const Box& b, LightNode_t* root, Array<const Light*>& results, const int maxResults = 8);
				};
			}
		}
	}
}

#endif