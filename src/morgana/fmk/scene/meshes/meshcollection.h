#ifndef __MORGANA_FMK_SCENE_MESH_COLLECTION_H__
#define __MORGANA_FMK_SCENE_MESH_COLLECTION_H__

#include "metypes.h"
#include "mememory.h"
#include "mesh.h"
#include "devices.h"
#include "meresources.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Scene
		{
			namespace Meshes
			{
				class Mesh;
				class MeshCollection
				{
				public:
					MeshCollection();
					virtual ~MeshCollection();

					const int		GenerateMesh(const char* source);
					const void		DeleteMesh(const char* source);
					const void		DeleteMesh(const int handle);
					Mesh*			Get(const int handle) const;
					const int		GetHandleForSource(const char* source) const;

					void			Import(const MeshData* src, Mesh* dst);
					Mesh*			Import(const MeshData* src, const char* name);
				private:
					MEPool<Mesh*>	allMeshes;
					void			DeleteAll();

					ManagedCPUMemory<4 * 1024 * 1024>	cpuMemory; // 4MB
					ManagedVBO<4 * 1024 * 1024>	gpuMemoryVertices; // 4MB
					ManagedIBO<4 * 1024 * 1024>	gpuMemoryIndices; // 4MB
				};
			}
		}
	}
}

#endif