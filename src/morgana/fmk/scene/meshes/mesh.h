#ifndef __MORGANA_FMK_SCENE_MESHES_MESH_H__
#define __MORGANA_FMK_SCENE_MESHES_MESH_H__

#include "metypes.h"
#include "mememory.h"
#include "devices.h"
#include "../../resources/models/meshdata.h"

namespace MorganaEngine
{
	namespace Framework
	{
		using namespace Resources::Models;
		namespace Scene
		{
			namespace Meshes
			{
				class Mesh : public IsNamed
				{
					bool keepDataOnCPU;
				public:

					friend class MeshCollection;

					inline const void	KeepDataOnCPU(const bool keep) {keepDataOnCPU = keep;}

					MemoryPage::Block_t						*cpuDataVertices, *cpuDataIndices;
					MemoryPage::Block_t						*gpuDataVertices, *gpuDataIndices;

					VertexLayout*							vertexLayout;
					unsigned int							numIndices, numVertices;

					const MeshData*							source;
					Box										bounds;

					int										cacheHandle;

				protected:
					Mesh()
					{
						cpuDataVertices = NULL;
						cpuDataIndices = NULL;
						gpuDataVertices = NULL;
						gpuDataIndices = NULL;

						keepDataOnCPU = true;

						bounds = Box::Zero;

						numIndices = 0;
						numVertices = 65535;

						source = NULL;

						cacheHandle = -1;
					}
					virtual ~Mesh(){}
				};
			}
		}
	}
}


#endif