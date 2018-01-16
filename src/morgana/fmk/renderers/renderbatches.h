#ifndef __MORGANA_FMK_RENDERERS_RENDER_BATCHES_H__
#define __MORGANA_FMK_RENDERERS_RENDER_BATCHES_H__

#include "renderer.h"
#include "streamingbuffers.h"
#include "genericbatch.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Renderers
		{
			template <class vertexT, class indexT, int maxVertices, int maxIndices, bool useQuadIndices>
			class RenderBatches
			{
				typedef GenericBatch_t<vertexT, indexT> Batch_t;

				Array<Matrix>	worldMatricesStack;

			public:
				RenderBatches()
				{
					currentBatch = -1;

					if (useQuadIndices)
					{
						int numQuads = maxVertices / 4;
						indexT* inds = ibos.Alloc(numQuads * 6);
						for (int i = 0; i < numQuads; i++)
						{
							inds[6 * i + 0] = 4 * i + 0;
							inds[6 * i + 1] = 4 * i + 1;
							inds[6 * i + 2] = 4 * i + 2;

							inds[6 * i + 3] = 4 * i + 0;
							inds[6 * i + 4] = 4 * i + 2;
							inds[6 * i + 5] = 4 * i + 3;
						}
						ibos.End();
					}
				}

				virtual ~RenderBatches()
				{
					Cleanup();
				}

				void SetContext(void* context)
				{
					this->context = context;
				}

				void Begin()
				{
					currentBatch = -1;
					lastVertexOffset = 0;
					vbos.Begin();
					ibos.Begin();
				}

				void End()
				{
					vbos.End();
					ibos.End();
				}

				void PushWorldMatrix(const Matrix* wm)
				{
					worldMatricesStack.Add(*wm);
					Batch_t* newOne = NewBatch(true);
				}

				void PopWorldMatrix()
				{
					worldMatricesStack.Pop();
					Batch_t* newOne = NewBatch(true);
				}

				const int Count() const
				{
					return currentBatch + 1;
				}

				Batch_t* operator [] (const int index)
				{
					return batches[index];
				}

				void SetMaterial(const Material* mat)
				{
					Batch_t* l = LastBatch();
					if (l != NULL && l->material == NULL)
					{
						l->material = new Material(*mat);
					}
					else
					if (l == NULL || l->material->Equals(mat) == false)
					{
						NewBatch(false)->material = new Material(*mat);
						return;
					}
				}

				void Alloc(const int numVertices, const int numIndices, vertexT** outVerts, indexT** outInds, const Material* mat = NULL)
				{
					ASSERT(context, "Context not set. Set it to RendersToCanvas");

					if (mat != NULL)
						SetMaterial(mat);

					Batch_t* l = LastBatch();

					vbos.TryAlloc(numVertices);
					if(!useQuadIndices)
						ibos.TryAlloc(numIndices);

					if (l->vbo != vbos.GetBuffer() || (!useQuadIndices && l->ibo != ibos.GetBuffer()))
					{
						Batch_t* newOne = NewBatch(true);
					}

					vbos.Alloc(numVertices);
					if (!useQuadIndices)
						ibos.Alloc(numIndices);

					l = LastBatch();

					lastVertexOffset = l->vertexOffset;

					*outVerts = l->VertexPtr();
					*outInds = useQuadIndices ? NULL : l->IndexPtr();

					l->vertexOffset += numVertices;
					l->indexOffset += numIndices;
				}

				void AdjustIndices(int count)
				{
					if (useQuadIndices)
						return;

					Batch_t* l = LastBatch();
					if (l == null) return;

					if (lastVertexOffset == 0) return;

					indexT* indices = l->IndexPtr() - count;
					indexT* last = indices + count;
					for (; indices < last; indices++)
					{
						*indices += lastVertexOffset;
					}
				}

				Array<Renderer::Drawable_t>&	BuildDrawablesList()
				{
					tmpDrawables.Clear();
					tmpMaterials.Clear();
					tmpWorldMatrices.Clear();

					for (int i = 0; i < Count(); i++)
					{
						const Batch_t* b = batches[i];

						if (b->vertexOffset == 0 || b->indexOffset == 0)
							continue;

						Renderer::Drawable_t r;

						r.vbo = b->vbo;
						r.ibo = b->ibo;
						r.vertexOffset = b->vboOffsetBytes;
						r.indexOffset = b->iboOffsetBytes;
						r.vertexCount = b->vertexOffset;
						r.indexCount = b->indexOffset;
						r.vertexLayout = &vertexT::Layout;

						tmpDrawables.Add(r);

						tmpMaterials.Add(b->material);

						if (b->worldMatrixIsIdentity == false)
							tmpWorldMatrices.Add(&b->worldMatrix);
						else
							tmpWorldMatrices.Add(NULL);
					}
					return tmpDrawables;
				}

				Array<const Material*>&			GetMaterialList()
				{
					return tmpMaterials;
				}

				Array<const Matrix*>&			GetWorldMatricesList()
				{
					return tmpWorldMatrices;
				}

			protected:

				StreamingBuffers<vertexT, maxVertices, BINDING_ARRAY_BUFFER> vbos;
				StreamingBuffers<indexT, maxIndices, BINDING_ELEMENT_ARRAY_BUFFER> ibos;

				Array<Batch_t*> batches;
				int				currentBatch;


				Array<Renderer::Drawable_t> tmpDrawables;
				Array<const Material*>		tmpMaterials;
				Array<const Matrix*>		tmpWorldMatrices;

				int lastVertexOffset;

				void*			context;

				void			Cleanup()
				{
					for (int i = 0; i < batches.Length(); i++)
						SAFE_DEL(batches[i]);
				}

				Batch_t*		NewBatch(bool copyLastMaterial)
				{
					currentBatch++;

					if (currentBatch >= batches.Length())
					{
						batches.Add(new Batch_t());
					}
					
					Batch_t* b = batches[currentBatch];
					b->Reset();
					b->context = context;
					b->vbo = vbos.GetBuffer();
					b->ibo = ibos.GetBuffer();
					b->vboOffsetBytes = vbos.GetBufferOffset();
					b->iboOffsetBytes = ibos.GetBufferOffset();
					if (worldMatricesStack.Length() > 0)
					{
						b->worldMatrix = worldMatricesStack.Last();
						b->worldMatrixIsIdentity = false;
					}

					Batch_t* last = LastBatch();

					if (last != NULL && copyLastMaterial && last->material != NULL)
					{
						b->material = new Material(*last->material);
					}

					return b;
				}

				Batch_t* LastBatch()
				{
					return currentBatch < 0 ? NULL : batches[currentBatch];
				}
			};	
		}
	}
}

#endif