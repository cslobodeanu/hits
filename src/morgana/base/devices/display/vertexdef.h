#ifndef __MORGANA_BASE_DEVICES_DISPLAY_VERTEXDEF_H__
#define __MORGANA_BASE_DEVICES_DISPLAY_VERTEXDEF_H__

#include "../../math/vector3.h"
#include "../../math/vector2.h"
#include "../../types/color255.h"
#include "rendepipelinedefs.h"

#include <memory.h>

using namespace MorganaEngine::Base::Math;
using namespace MorganaEngine::Base::Types;


#define DECLARE_VERTEX_TYPE(classname) \
	static VertexLayout Layout; \
	const int GetSize() const { return sizeof(classname); } \
	const int GetStride() const { return sizeof(classname); } \
	classname() \

#define OFFSET_OF(varname) ((unsigned long)&varname - (unsigned long)this)
#define SET_LAYOUT_ROW(name, varname, count, type, normalize) \
{ \
	Layout.defs[VERTEX_COMPONENT_##name * VertexLayout::NUM_COMPONENTS + VertexLayout::OFFSET] = OFFSET_OF(varname); \
	Layout.defs[VERTEX_COMPONENT_##name * VertexLayout::NUM_COMPONENTS + VertexLayout::COUNT] = count; \
	Layout.defs[VERTEX_COMPONENT_##name * VertexLayout::NUM_COMPONENTS + VertexLayout::TYPE] = VERTEX_COMP_##type; \
	Layout.defs[VERTEX_COMPONENT_##name * VertexLayout::NUM_COMPONENTS + VertexLayout::NORMALIZE] = normalize; \
	Layout.defs[VERTEX_COMPONENT_##name * VertexLayout::NUM_COMPONENTS + VertexLayout::SIZE] = sizeof(varname); \
} \

namespace MorganaEngine
{
	namespace Base
	{
		namespace Devices
		{
			namespace Display
			{
				struct VertexLayout
				{
				private:
					int		totalSize;
				public:
					enum ComponentDef_t
					{
						OFFSET = 0,
						COUNT,
						TYPE,
						NORMALIZE,
						SIZE,
						NUM_COMPONENTS
					};
					int	defs[VERTEX_COMPONENTS_COUNT * NUM_COMPONENTS];
					VertexLayout()
					{
						memset(defs, 0, sizeof(defs));
						totalSize = 0;
					}

					VertexLayout(const VertexLayout& other)
					{
						memcpy(defs, other.defs, sizeof(defs));
						totalSize = other.totalSize;
					}

					inline const unsigned int	GetSize()
					{
						if (totalSize == 0)
						{
							for (int i = 0; i < VERTEX_COMPONENTS_COUNT; i++)
								totalSize += defs[i * NUM_COMPONENTS + SIZE];
						}

						return totalSize;
					}

					inline const bool			HasComponent(const int which) const { return defs[which * NUM_COMPONENTS + SIZE] > 0; }
					inline const bool			HasPosition() const { return HasComponent(VERTEX_COMPONENT_POS); }
					inline const bool			HasNormal() const { return HasComponent(VERTEX_COMPONENT_NORMAL); }
					inline const bool			HasTangent() const { return HasComponent(VERTEX_COMPONENT_TANGENT); }
					inline const bool			HasBinormal() const { return HasComponent(VERTEX_COMPONENT_BINORMAL); }
					inline const bool			HasColor() const { return HasComponent(VERTEX_COMPONENT_COLOR); }
					inline const bool			HasTexCoord0() const { return HasComponent(VERTEX_COMPONENT_TEX0); }

					inline const int			GetComponentType(const int which) const { return defs[which * NUM_COMPONENTS + TYPE]; }
					inline const int			GetNumComponents(const int which) const { return defs[which * NUM_COMPONENTS + COUNT]; }
					inline const int			GetComponentSize(const int which) const { return defs[which * NUM_COMPONENTS + SIZE]; }
					inline const int			GetComponentOffset(const int which) const { return defs[which * NUM_COMPONENTS + OFFSET]; }
					inline const bool			GetComponentNormalize(const int which) const { return defs[which * NUM_COMPONENTS + NORMALIZE] > 0; }

					inline void*				GetComponentOffsetPtr(const int component, const void* base, const int index)
					{
						const char* byteBuf = (char*)base + index * GetSize() + defs[component * NUM_COMPONENTS + OFFSET];
						return (void*)byteBuf;
					}

					inline void*				GetPositionOffsetPtr(const void* base, const int index)	{ return GetComponentOffsetPtr(VERTEX_COMPONENT_POS, base, index);}
					inline void*				GetNormalOffsetPtr(const void* base, const int index) { return GetComponentOffsetPtr(VERTEX_COMPONENT_NORMAL, base, index); }
					inline void*				GetTangentOffsetPtr(const void* base, const int index) { return GetComponentOffsetPtr(VERTEX_COMPONENT_TANGENT, base, index); }
					inline void*				GetBinormalOffsetPtr(const void* base, const int index)	{ return GetComponentOffsetPtr(VERTEX_COMPONENT_BINORMAL, base, index); }
					inline void*				GetTexCoord0OffsetPtr(const void* base, const int index)	{ return GetComponentOffsetPtr(VERTEX_COMPONENT_TEX0, base, index); }
					inline void*				GetColorOffsetPtr(const void* base, const int index)	{ return GetComponentOffsetPtr(VERTEX_COMPONENT_COLOR, base, index); }

				};
				class VertexDefinition
				{
				public:

					DECLARE_VERTEX_TYPE(VertexDefinition)
					{
					}
					~VertexDefinition() {}

					static char* Alloc(const VertexDefinition* def, const unsigned int numVerts);
					static char* Alloc(VertexLayout* layout, const unsigned int numVerts);
				};

				class VertexPos : public VertexDefinition
				{
				public:

					Vector3 pos;

					DECLARE_VERTEX_TYPE(VertexPos)
					{
						SET_LAYOUT_ROW(POS, pos, 3, FLOAT, 0);
					}
				};

				class VertexPosColor : public VertexDefinition
				{
				public:

					Vector3 pos;
					Color255 color;

					DECLARE_VERTEX_TYPE(VertexPosColor)
					{
						SET_LAYOUT_ROW(POS, pos, 3, FLOAT, 0);
						SET_LAYOUT_ROW(COLOR, color, 4, BYTE, VERTEX_COMP_NORMALIZED);
					}
				};

				class VertexPosColorTex : public VertexDefinition
				{
				public:

					Vector3 pos;
					Vector2 uv;
					Color255 color;

					DECLARE_VERTEX_TYPE(VertexPosColorTex)
					{
						SET_LAYOUT_ROW(POS, pos, 3, FLOAT, 0);
						SET_LAYOUT_ROW(TEX0, uv, 2, FLOAT, 0);
						SET_LAYOUT_ROW(COLOR, color, 4, BYTE, VERTEX_COMP_NORMALIZED);
					}
				};

				class VertexPosColorTexTex2 : public VertexDefinition
				{
				public:

					Vector3 pos;
					Vector2 uv;
					Vector2 uv2;
					Color255 color;

					DECLARE_VERTEX_TYPE(VertexPosColorTexTex2)
					{
						SET_LAYOUT_ROW(POS, pos, 3, FLOAT, 0);
						SET_LAYOUT_ROW(TEX0, uv, 2, FLOAT, 0);
						SET_LAYOUT_ROW(TEX1, uv2, 2, FLOAT, 0);
						SET_LAYOUT_ROW(COLOR, color, 4, BYTE, VERTEX_COMP_NORMALIZED);
					}
				};

				class VertexPosColorColor2TexTex2 : public VertexDefinition
				{
				public:

					Vector3 pos;
					Vector2 uv;
					Vector2 uv2;
					Color255 color;
					Color255 color2;

					DECLARE_VERTEX_TYPE(VertexPosColorColor2TexTex2)
					{
						SET_LAYOUT_ROW(POS, pos, 3, FLOAT, 0);
						SET_LAYOUT_ROW(TEX0, uv, 2, FLOAT, 0);
						SET_LAYOUT_ROW(TEX1, uv2, 2, FLOAT, 0);
						SET_LAYOUT_ROW(COLOR, color, 4, BYTE, VERTEX_COMP_NORMALIZED);
						SET_LAYOUT_ROW(COLOR2, color2, 4, BYTE, VERTEX_COMP_NORMALIZED);
					}
				};

				class VertexPosNormalTex : public VertexDefinition
				{
				public:

					Vector3 pos;
					Vector2 uv;
					Vector3 normal;

					DECLARE_VERTEX_TYPE(VertexPosNormalTex)
					{
						SET_LAYOUT_ROW(POS, pos, 3, FLOAT, 0);
						SET_LAYOUT_ROW(TEX0, uv, 2, FLOAT, 0);
						SET_LAYOUT_ROW(NORMAL, normal, 3, FLOAT, 0);
					}
				};
			}
		}
	}
}

#define SET_COMPONENT_SAFE(vtx, comp, val) \
	if(vtx->Layout.HasComponent(VERTEX_COMPONENT_##comp)) \
	{ \
		memcpy(vtx->Layout.GetComponentOffsetPtr(VERTEX_COMPONENT_##comp, vtx, 0), &val, vtx->Layout.GetComponentSize(VERTEX_COMPONENT_##comp)); \
	}

#endif