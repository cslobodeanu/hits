#include "primitives.h"

using namespace MorganaEngine::Framework::Scene::Meshes;

void Primitive::Transform(MeshData* m, const Matrix* mat)
{
	VertexLayout* vd = m->vertexLayout;
	for(unsigned int i = 0; i < m->numVertices; i++)
	{
		if(vd->HasPosition())
		{
			Vector3* pos = (Vector3*)vd->GetPositionOffsetPtr(m->vertices, i);
			const Vector3 tmp = *pos;
			Vector3::Transform(&tmp, mat, pos);
		}

		if(vd->HasNormal())
		{
			Vector3* nrm = (Vector3*)vd->GetNormalOffsetPtr(m->vertices, i);
			const Vector3 tmp = *nrm;
			Vector3::TransformNormal(&tmp, mat, nrm);
			nrm->Normalize();
		}

		if(vd->HasTangent())
		{
			Vector3* tg = (Vector3*)vd->GetTangentOffsetPtr(m->vertices, i);
			const Vector3 tmp = *tg;
			Vector3::TransformNormal(&tmp, mat, tg);
			tg->Normalize();
		}

		if(vd->HasBinormal())
		{
			Vector3* bn = (Vector3*)vd->GetNormalOffsetPtr(m->vertices, i);
			const Vector3 tmp = *bn;
			Vector3::TransformNormal(&tmp, mat, bn);
			bn->Normalize();
		}
	}

	ComputeBounds(m);
}

MeshData* Primitive::ExtrudePolygon(const MeshData* src, const Vector3& dir /*= Vector3::UnitY*/, const int numSteps /*= 2*/)
{
	const float extLen = dir.Length();
	const Vector3 extDir = dir / extLen;

	ASSERT(numSteps >= 2);

	const int numVertices = (src->numVertices) * (unsigned int)numSteps;
	const float stepLen = extLen / (float)(numSteps - 1);

	MeshData* out = new MeshData();
	out->vertexLayout = src->vertexLayout;
	out->numVertices = numVertices;
	out->AllocVertices();

	out->numIndices = (src->numVertices - 1) * (numSteps - 1) * 6;
	out->AllocIndices();

	char* vtxPtr = out->vertices;
	for(int i = 0; i < numSteps; i++)
	{
		memcpy(vtxPtr, src->vertices, src->GetVerticesAllocSize());

		const float extOfs = i * stepLen;

		for(unsigned int j = 0; j < src->numVertices; j++)
		{
			if (out->vertexLayout->HasPosition())
 			{
				Vector3& pos = *((Vector3*)out->vertexLayout->GetPositionOffsetPtr(vtxPtr, j));
				pos += extDir * extOfs;
			}

			if (out->vertexLayout->HasTexCoord0())
			{
				Vector2& uv = *((Vector2*)out->vertexLayout->GetTexCoord0OffsetPtr(vtxPtr, j));
				uv.x = (float)j / (float)(src->numVertices - 1);
				uv.y = extOfs / extLen;
			}
		}

		vtxPtr += src->GetVerticesAllocSize();
	}

	IndexType32* indPtr = out->indices;
	for(int i = 0; i < numSteps - 1; i++)
	{
		for(unsigned int j = 0; j < src->numVertices - 1; j++)
		{
			const int c_row = i * src->numVertices;
			const int next_row = (i + 1) * src->numVertices;

			const int c_j = j + c_row;
			const int j_plus_one = (j + 1) % src->numVertices + c_row;
			const int next_row_c_j = j + next_row;
			const int next_row_j_plus_one = (j + 1) % src->numVertices + next_row;

			indPtr[0] = c_j;
			indPtr[1] = next_row_c_j;
			indPtr[2] = j_plus_one;

			indPtr[3] = j_plus_one;
			indPtr[4] = next_row_c_j;
			indPtr[5] = next_row_j_plus_one;

			indPtr += 6;
		}
	}

	if (out->vertexLayout->HasNormal())
		CreateNormals(out);

	ComputeBounds(out);

	return out;
}

void Primitive::AppendMesh(MeshData* dst, const MeshData* src, const Matrix* srcTransform /*= NULL*/)
{
	MeshData* srcMesh = (MeshData*)src;
	if(srcTransform)
	{
		MeshData* dup = Clone(src);
		Transform(dup, srcTransform);
		srcMesh = dup;
	}


	const unsigned int startVertex = dst->numVertices;
	const unsigned int startIndex = dst->numIndices;
	dst->ReallocVertices(dst->numVertices + srcMesh->numVertices);
	dst->ReallocIndices(dst->numIndices + srcMesh->numIndices);

	Array<int> components;
	for(int i = 0; i < VERTEX_COMPONENTS_COUNT; i++)
	{
		if (dst->vertexLayout->GetComponentSize(i) != srcMesh->vertexLayout->GetComponentSize(i) || dst->vertexLayout->GetComponentSize(i) == 0) continue;
		components.Add(i);
	}

	char* vtxPtr = dst->vertices + startVertex * dst->vertexLayout->GetSize();

	for(unsigned int j = 0; j < srcMesh->numVertices; j++)
	{
		for(int i = 0; i < components.Length(); i++)
		{
			const int comp = components[i];
			memcpy(dst->vertexLayout->GetComponentOffsetPtr(comp, vtxPtr, j), srcMesh->vertexLayout->GetComponentOffsetPtr(comp, srcMesh->vertices, j), srcMesh->vertexLayout->GetComponentSize(comp));
		}
	}

	IndexType32* idxPtr = dst->indices + startIndex;
	IndexType32* srcIdxPtr = srcMesh->indices;
	const IndexType32* idxEnd = dst->indices + dst->numIndices;
	for(; idxPtr < idxEnd; idxPtr++, srcIdxPtr++)
	{
		*idxPtr = *srcIdxPtr + startVertex;
	}

	if(srcMesh != src)
		SAFE_DEL(srcMesh);

	ComputeBounds(dst);
}

MeshData* Primitive::Clone(const MeshData* src)
{
	MeshData* out = new MeshData();

	out->vertexLayout = src->vertexLayout;
	out->numVertices = src->numVertices;
	out->numIndices = src->numIndices;
	out->AllocVertices();
	out->AllocIndices();

	memcpy(out->vertices, src->vertices, src->GetVerticesAllocSize());
	memcpy(out->indices, src->indices, src->GetIndicesAllocSize());

	out->bounds = src->bounds;

	return out;
}

void Primitive::CreateNormals(MeshData* src)
{
	if (src->vertexLayout->HasNormal() == false) return;

	const int numTris = src->numIndices / 3;

	for(unsigned int i = 0; i < src->numVertices; i++)
	{
		Vector3* nrm = (Vector3*)src->vertexLayout->GetNormalOffsetPtr(src->vertices, i);
		*nrm = Vector3::Zero;
	}

	for(int i = 0; i < numTris; i++)
	{
		const IndexType32& i1 = src->indices[3 * i];
		const IndexType32& i2 = src->indices[3 * i + 1];
		const IndexType32& i3 = src->indices[3 * i + 2];

		Vector3* p1 = (Vector3*)src->vertexLayout->GetPositionOffsetPtr(src->vertices, i1);
		Vector3* p2 = (Vector3*)src->vertexLayout->GetPositionOffsetPtr(src->vertices, i2);
		Vector3* p3 = (Vector3*)src->vertexLayout->GetPositionOffsetPtr(src->vertices, i3);

		const Vector3 e1 = *p1 - *p2;
		const Vector3 e2 = *p2 - *p3;

		const Vector3 triN = Vector3::Cross(&e1, &e2);

		Vector3* n1 = (Vector3*)src->vertexLayout->GetNormalOffsetPtr(src->vertices, i1);
		Vector3* n2 = (Vector3*)src->vertexLayout->GetNormalOffsetPtr(src->vertices, i2);
		Vector3* n3 = (Vector3*)src->vertexLayout->GetNormalOffsetPtr(src->vertices, i3);

		*n1 += triN;
		*n2 += triN;
		*n3 += triN;
	}

	for(unsigned int i = 0; i < src->numVertices; i++)
	{
		Vector3* nrm = (Vector3*)src->vertexLayout->GetNormalOffsetPtr(src->vertices, i);
		nrm->Normalize();
	}

}

void Primitive::SetVertexColors(MeshData* src, const Color c)
{
	if (src->vertexLayout->HasColor() == false) return;

	for(unsigned int i = 0; i < src->numVertices; i++)
	{
		Color* col = (Color*)src->vertexLayout->GetColorOffsetPtr(src->vertices, i);
		*col = c;
	}
}

void Primitive::ComputeBounds(MeshData* src)
{
	src->bounds = Box::Zero;
	Vector3 min = Vector3::One * Mathf::MaxFloat;
	Vector3 max = -min;
	for (unsigned int i = 0; i < src->numVertices; i++)
	{
		Vector3* pos = (Vector3*)src->vertexLayout->GetPositionOffsetPtr(src->vertices, i);
		min = Vector3::Min(pos, &min);
		max = Vector3::Max(pos, &max);
	}

	src->bounds = Box::FromMinMax(min, max);
}
