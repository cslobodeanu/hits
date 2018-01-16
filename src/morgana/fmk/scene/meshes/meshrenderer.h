#ifndef __MORGANA_FRAMEWORK_RENDERERS_MESH_RENDERER_H__
#define __MORGANA_FRAMEWORK_RENDERERS_MESH_RENDERER_H__

#include "../../renderers/renderer.h"

namespace MorganaEngine
{
	namespace Framework
	{
		using namespace Renderers;
		namespace Scene
		{
			namespace Meshes
			{
				class MeshRenderer : public Renderer
				{
					__declare_class(MeshRenderer, Renderer);
				public:

					MeshRenderer();
					virtual ~MeshRenderer();
				protected:
					virtual void UpdateRenderables();
				};
			}
		}
	}
}

#endif