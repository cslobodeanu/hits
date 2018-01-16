#ifndef __MORGANA_FRAMEWORK_RENDERERS_GENERIC_BATCH_H__
#define __MORGANA_FRAMEWORK_RENDERERS_GENERIC_BATCH_H__

#include "meresources.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Renderers
		{
			template <class vertexT, class indexT>
			struct GenericBatch_t
			{
			public:
				const Material* material;
				Buffer			*vbo, *ibo;
				unsigned int	vboOffsetBytes, iboOffsetBytes;
				unsigned int	vertexOffset, indexOffset;

				Matrix			worldMatrix;
				bool			worldMatrixIsIdentity;

				void* context;

				GenericBatch_t()
				{
					material = NULL;
					Reset();
				}

				~GenericBatch_t()
				{
				}

				void Reset()
				{
					vbo = NULL;
					ibo = NULL;

					vertexOffset = 0;
					indexOffset = 0;

					vboOffsetBytes = 0;
					iboOffsetBytes = 0;

					worldMatrix = Matrix::Identity;
					worldMatrixIsIdentity = true;

					SAFE_DEL(material);

					context = NULL;
				}

				vertexT* VertexPtr()
				{
					byte* ptr = (byte*)vbo->Lock() + vboOffsetBytes;
					return (vertexT*)ptr + vertexOffset;
				}

				indexT* IndexPtr()
				{
					byte* ptr = (byte*)ibo->Lock() + iboOffsetBytes;
					return (indexT*)ptr + indexOffset;
				}
			};
		}
	}
}

#endif