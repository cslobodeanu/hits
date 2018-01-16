#ifndef __MORGANA_FMK_SCENE_SCENE_ROOT_H__
#define __MORGANA_FMK_SCENE_SCENE_ROOT_H__

#include "gameobjects.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Scene
		{
			class SceneRoot : public GameObject
			{
				__declare_class(SceneRoot, GameObject);
			public:

				SceneRoot();
				virtual ~SceneRoot();

				Box			bounds;

			protected:

				virtual void Update();
			};
		}
	}
}

#endif