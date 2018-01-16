#ifndef __MORGANA_FMK_RESOURCES_MODELS_MESH_DATA_H__
#define __MORGANA_FMK_RESOURCES_MODELS_MESH_DATA_H__

#include "metypes.h"
#include "devices.h"
#include "memath.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Resources
		{
			namespace Models
			{
				using namespace MorganaEngine::Base::Devices::Display;
				class MeshData
				{
				public:
					MeshData()
					{
						vertices = NULL;
						numVertices = 0;

						indices = NULL;
						numIndices = 0;

						vertexLayout = NULL;

						bounds = Box::Zero;
					}
					virtual ~MeshData()
					{
						SAFE_DEL_ARRAY(vertices);
						SAFE_DEL_ARRAY(indices);
					}

					VertexLayout*				vertexLayout;

					char*						vertices;
					unsigned int				numVertices;

					IndexType32*				indices;
					unsigned int				numIndices;

					inline const unsigned int	GetVerticesAllocSize() const { return numVertices * vertexLayout->GetSize(); }
					inline const unsigned int	GetIndicesAllocSize() const { return numIndices * sizeof(IndexType32); }

					inline const void			AllocVertices() { SAFE_DEL_ARRAY(vertices); vertices = VertexDefinition::Alloc(vertexLayout, numVertices); }
					inline const void			AllocIndices() { SAFE_DEL_ARRAY(indices); indices = new IndexType32[numIndices]; }

					inline const void			ReallocVertices(const unsigned int newNumVerts)
					{
						char* newVerts = new char[newNumVerts * vertexLayout->GetSize()];
						memcpy(newVerts, vertices, GetVerticesAllocSize());
						SAFE_DEL_ARRAY(vertices);
						vertices = newVerts;
						numVertices = newNumVerts;
					}

					inline const void			ReallocIndices(const unsigned int newNumInds)
					{
						IndexType32* newInds = new IndexType32[newNumInds];
						memcpy(newInds, indices, GetIndicesAllocSize());
						SAFE_DEL_ARRAY(indices);
						indices = newInds;
						numIndices = newNumInds;
					}

					Box bounds;

					inline const void			ComputeBounds()
					{
						Vector3 min = Vector3::MaxFloat;
						Vector3 max = Vector3::MinFloat;

						for (unsigned int i = 0; i < numVertices; i++)
						{
							Vector3* p = (Vector3*)vertexLayout->GetPositionOffsetPtr(vertices, i);
							min = Vector3::Min(min, *p);
							max = Vector3::Max(max, *p);
						}

						bounds = Box::Zero;
						if (numVertices > 0)
						{
							bounds = Box::FromMinMax(min, max);
						}
					}
				};
			}
		}
	}
}

#endif