#ifndef __MORGANA_FMK_SCENE_MESHES_PRIMITIVES_H__
#define __MORGANA_FMK_SCENE_MESHES_PRIMITIVES_H__

#include "metypes.h"
#include "devices.h"
#include "memath.h"
#include "meresources.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Scene
		{
			namespace Meshes
			{
				class Primitive
				{
				public:
					template <class VertexDef>
					static MeshData*			GeneratePolygon(const int numCorners, const float angleVarianceFactor = 0.0f, const float radiusVarianceFactor = 0.0f);
					template <class VertexDef>
					static MeshData*			GenerateTriangle() { return GeneratePolygon<VertexDef>(3); }

					static MeshData*			ExtrudePolygon(const MeshData* src, const Vector3& dir = Vector3::UnitY, const int numSteps = 2);
					static void					Transform(MeshData* m, const Matrix* mat);

					template<class outputT>
					static void					ExtractComponents(const void* src, const int srcSize, VertexLayout* srcFormat, const int component, Array<outputT>& output);

					static void					AppendMesh(MeshData* dst, const MeshData* src, const Matrix* srcTransform = NULL);

					static MeshData*			Clone(const MeshData* src);

					static void					CreateNormals(MeshData* src);
					static void					ComputeBounds(MeshData* src);
					static void					SetVertexColors(MeshData* src, const Color c);

				};

				template<class outputT>
				void Primitive::ExtractComponents(const void* src, const int srcSize, VertexLayout* srcFormat, const int component, Array<outputT>& output)
				{
					const int compSize = srcFormat->GetComponentSize(component);
					const int compOfs = srcFormat->GetComponentOffset(component);
					ASSERT(sizeof(outputT) == compSize);

					const int stride = srcFormat->GetSize();
					const int numElems = srcSize / stride;
					output.SetLength(numElems);
					char* ptr =(char*)src;
					for(int i = 0; i < numElems; i++)
					{
						memcpy(output.ptr() + i, ptr + compOfs, compSize);
						ptr += stride;
					}
				}

				template<class VertexDef>
				MeshData* Primitive::GeneratePolygon(const int numCorners, const float angleVarianceFactor /*= 0.0f*/, const float radiusVarianceFactor /*= 0.0f*/)
				{
					MeshData* mesh = new MeshData();
					mesh->vertexLayout = &VertexDef::Layout;
					mesh->numVertices = numCorners + 1;
					mesh->AllocVertices();

					mesh->numIndices = 3 * (numCorners - 2);
					mesh->AllocIndices();

					const float stepAngle = Mathf::TwoPi / (float)numCorners;

					float angle = Mathf::PiOver4;
					float radius = 1.0f;

					VertexDef* vtx = (VertexDef*)mesh->vertices;

					for(int i = 0; i < numCorners + 1; i++, vtx++)
					{
						const float _radius = Mathf::RandAroundFactor(radius, radiusVarianceFactor);
						const Vector3 p = Mathf::Cos(angle) * _radius * Vector3::UnitX + Mathf::Sin(angle) * _radius * Vector3::UnitY;
						angle += Mathf::RandAroundFactor(stepAngle, angleVarianceFactor);

						vtx->pos = p;

						SET_COMPONENT_SAFE(vtx, COLOR, Color::red);
						SET_COMPONENT_SAFE(vtx, NORMAL, Vector3::UnitZ);

						const Vector3 uv = (p * (1.0f / radius) + Vector3::One) * 0.5f;
						SET_COMPONENT_SAFE(vtx, TEX0, uv);
					}

					for(int i = 0; i < numCorners - 2; i++)
					{
						mesh->indices[3 * i] = 0;
						mesh->indices[3 * i + 1] = i + 1;
						mesh->indices[3 * i + 2] = i + 2;
					}

					ComputeBounds(mesh);

					return mesh;
				}
			}
		}
	}
}


#endif