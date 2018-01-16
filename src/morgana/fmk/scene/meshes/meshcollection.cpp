#include "meshcollection.h"
#include "mesh.h"
#include "meio.h"
#include "mescene.h"

using namespace MorganaEngine::Framework::Scene::Meshes;

MeshCollection::MeshCollection()
{
}

MeshCollection::~MeshCollection()
{
	DeleteAll();
}

Mesh* MeshCollection::Get(const int handle) const
{
	return allMeshes.IsValidObject(handle) ? allMeshes[handle] : NULL;
}

const int MeshCollection::GetHandleForSource(const char* source) const
{
	const int hash = String::ComputeHashCode(source);
	for(int i = 0; i < allMeshes.GetObjectsCount(); i++)
	{
		if(allMeshes.IsValidObject(i) == false) continue;
		if(allMeshes[i]->HasName(source, hash))
			return i;
	}

	return -1;
}

void MeshCollection::DeleteAll()
{
	for(int i = 0; i < allMeshes.GetObjectsCount(); i++)
	{
		if(allMeshes.IsValidObject(i) == false) continue;
		SAFE_DEL(allMeshes[i]);
	}

	allMeshes.Clear();
}

const int MeshCollection::GenerateMesh(const char* source)
{
	ASSERT(GetHandleForSource(source) < 0, "Source already exists");

	Mesh* m = new Mesh();
	m->SetName(source);

	const int handle = allMeshes.GenerateObject();
	allMeshes[handle] = m;

	m->cacheHandle = handle;

	return handle;
}

const void MeshCollection::DeleteMesh(const char* source)
{
	const int h = GetHandleForSource(source);
	if(h >= 0)
		DeleteMesh(h);
}

const void MeshCollection::DeleteMesh(const int handle)
{
	Mesh* m = allMeshes[handle];
	allMeshes.DeleteObject(handle);

	cpuMemory.Free(m->cpuDataVertices);
	cpuMemory.Free(m->cpuDataIndices);

	gpuMemoryVertices.Free(m->gpuDataVertices);
	gpuMemoryIndices.Free(m->gpuDataIndices);

	SAFE_DEL(m);
}

void MeshCollection::Import(const MeshData* src, Mesh* dst)
{
	const unsigned int vertSz = src->GetVerticesAllocSize();
	const unsigned int indSz = src->GetIndicesAllocSize();

	if(dst->keepDataOnCPU)
	{
		dst->cpuDataVertices = cpuMemory.Alloc(vertSz);
		dst->cpuDataIndices = cpuMemory.Alloc(indSz);

		memcpy(dst->cpuDataVertices->Lock(), src->vertices, vertSz);
		memcpy(dst->cpuDataIndices->Lock(), src->indices, indSz);

		dst->cpuDataVertices->Unlock();
		dst->cpuDataIndices->Unlock();
	}

	dst->gpuDataVertices = gpuMemoryVertices.Alloc(vertSz);
	dst->gpuDataIndices = gpuMemoryIndices.Alloc(indSz);
	
	memcpy(dst->gpuDataVertices->Lock(), src->vertices, vertSz);
	memcpy(dst->gpuDataIndices->Lock(), src->indices, indSz);

	dst->gpuDataVertices->Unlock();
	dst->gpuDataIndices->Unlock();

	dst->vertexLayout = src->vertexLayout;
	dst->numVertices = src->numVertices;
	dst->numIndices = src->numIndices;

	dst->source = src;

	dst->bounds = src->bounds;

	//DEBUG_OUT("Allocated %d+%d bytes for %s", vertSz, indSz, dst->name.c_str());
}

Mesh* MeshCollection::Import(const MeshData* src, const char* name)
{
	int handle = GenerateMesh(name);
	Mesh* m = Get(handle);
	Import(src, m);
	return m;
}
