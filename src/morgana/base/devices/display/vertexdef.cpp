#include "vertexdef.h"
#include "rendepipelinedefs.h"
#include "meio.h"

using namespace MorganaEngine::Base::Devices::Display;

#define IMPLEMENT_VERTEX_DEF(name) \
	name dummy_##name; \
	VertexLayout	name::Layout(dummy_##name.Layout);

IMPLEMENT_VERTEX_DEF(VertexPosColor);
IMPLEMENT_VERTEX_DEF(VertexPosNormalTex);
IMPLEMENT_VERTEX_DEF(VertexPosColorTex);
IMPLEMENT_VERTEX_DEF(VertexPosColorTexTex2);
IMPLEMENT_VERTEX_DEF(VertexPosColorColor2TexTex2);

char* VertexDefinition::Alloc(const VertexDefinition* def, const unsigned int numVerts)
{
	return new char[numVerts * def->GetSize()];
}

char* VertexDefinition::Alloc(VertexLayout* layout, const unsigned int numVerts)
{
	return new char[numVerts * layout->GetSize()];
}
