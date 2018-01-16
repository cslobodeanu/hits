#ifndef __MORGANA_FMK_SCENE_MESHES_MESH_COMPONENT_H__
#define __MORGANA_FMK_SCENE_MESHES_MESH_COMPONENT_H__

#include "gameobjects.h"
#include "mesh.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Scene
		{
			namespace Meshes
			{
				class MeshComponent : public Component
				{
					__declare_component(MeshComponent, Component);
				public:
					MeshComponent();
					virtual ~MeshComponent();

					Mesh*	mesh;
					Mesh*	sharedMesh;
				};
			}
		}
	}
}

#endif