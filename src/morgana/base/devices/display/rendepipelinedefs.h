#ifndef __MORGANA_BASE_DEVICES_DISPLAY_RENDER_PIPELINE_DEFS_H__
#define __MORGANA_BASE_DEVICES_DISPLAY_RENDER_PIPELINE_DEFS_H__

#include "metypes.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Devices
		{
			namespace Display
			{
				enum RenderPipelineBufferTypes_t
				{
					MDISPLAY_BUFFER_COLOR = 1,
					MDISPLAY_BUFFER_DEPTH = 2,
					MDISPLAY_BUFFER_STENCIL = 4,
				};

				enum PipelineObjectType_t
				{
					DISPLAY_OBJECT_UNDEFINED = -1,
					DISPLAY_OBJECT_TEXTURE = 0,
					DISPLAY_OBJECT_BUFFER = 1,
					DISPLAY_OBJECT_SHADER_PROGRAM = 3,
					DISPLAY_OBJECT_UNIFORM_BUFFER = 4,
					DISPLAY_OBJECT_RENDER_TARGET = 5,
					DISPLAY_OBJECTS_TYPES_COUNT = 6
				};

				enum PipelineObjectBindingPoint_t
				{
					BINDING_TEXTURE_2D = 0,
					BINDING_TEXTURE_1D = 1,
					BINDING_TEXTURE_3D = 2,
					BINDING_TEXTURE_CUBEMAP = 3,
					BINDING_ARRAY_BUFFER = 4,
					BINDING_ELEMENT_ARRAY_BUFFER = 5
				};

				enum BufferObjectAccessType_t
				{
					BUFFER_ACCESS_STATIC = 0,
					BUFFER_ACCESS_DYNAMIC = 1,
					BUFFER_ACCESS_STREAM = 2,
				};

				struct UniformBufferCreateParams_t
				{
					void	*nothing;
				};

				enum VertexComponents_t
				{
					VERTEX_COMPONENT_POS = 0,
					VERTEX_COMPONENT_NORMAL = 1,
					VERTEX_COMPONENT_COLOR = 2,
					VERTEX_COMPONENT_TEX0 = 3,
					VERTEX_COMPONENT_TEX1 = 4,
					VERTEX_COMPONENT_TEX2 = 5,
					VERTEX_COMPONENT_TEX3 = 6,
					VERTEX_COMPONENT_TEX4 = 7,
					VERTEX_COMPONENT_TEX5 = 8,
					VERTEX_COMPONENT_TEX6 = 9,
					VERTEX_COMPONENT_TEX7 = 10,
					VERTEX_COMPONENT_TANGENT = 11,
					VERTEX_COMPONENT_BINORMAL = 12,
					VERTEX_COMPONENT_BONE_WEIGHT = 13,
					VERTEX_COMPONENT_BONE_MATRIX_INDEX = 14,
					VERTEX_COMPONENT_COLOR2 = 15,
					VERTEX_COMPONENTS_COUNT = 16
				};

				enum
				{
					VERTEX_COMP_FLOAT,
					VERTEX_COMP_BYTE
				};

				enum
				{
					VERTEX_COMP_NOT_NORMALIZED,
					VERTEX_COMP_NORMALIZED
				};

				const String VertexAttribNames[VERTEX_COMPONENTS_COUNT] = {"vertex_pos",
																		"vertex_normal",
																		"vertex_color",
																		"vertex_uv0",
																		"vertex_uv1",
																		"vertex_uv2",
																		"vertex_uv3",
																		"vertex_uv4",
																		"vertex_uv5",
																		"vertex_uv6",
																		"vertex_uv7",
																		"vertex_tangent",
																		"vertex_binormal",
																		"vertex_bone_weight",
																		"vertex_bone_matrix",
																		"vertex_color2"};


				enum UniformDataType_t
				{
					UNIFORM_float,
					UNIFORM_Vector2,
					UNIFORM_Vector3,
					UNIFORM_Vector4,
					UNIFORM_Matrix,
					UNIFORM_int
				};

				static const int UniformDataSizes_t[] = { sizeof(float) };

				enum
				{
					DRAWMODE_TRIANGLES,
					DRAWMODE_TRISTRIP,
					DRAWMODE_LINES
				};

				enum
				{
					VERTEX_SHADER,
					FRAGMENT_SHADER
				};

				enum
				{
					MATRIX_WORLD = 0,
					MATRIX_VIEW,
					MATRIX_PROJECTION,
				};

				typedef enum
				{
					Point,
					Bilinear,
					Trilinear,
				} Filtering;

				typedef enum
				{
					Clamp,
					Repeat
				} WrapMode;

				typedef enum
				{
					FormatUnknown = 0,
					FormatAlpha,
					FormatGrey,
					FormatGreyAlpha,
					FormatDepth,
					FormatRGB,
					FormatRGBA,
					FormatRGBA4,
					FormatRGB565,
					FormatRGBA5551,
					FormatRGBPVR4,
					FormatRGBPVR2,
					FormatRGBAPVR4,
					FormatRGBAPVR2,
					FormatRGBS3TCDXT1,
					FormatRGBAS3TCDXT1,
					FormatRGBAS3TCDXT3,
					FormatRGBAS3TCDXT5,
					FormatRGBATC,
					FormatRGBAEATC,
					FormatRGBAIATC,
					FormatRGBETC
				} PixelFormat;

				enum PixelType
				{
					UnsignedByte,
					UnsignedInt
				};

				typedef unsigned short		IndexType16;
				typedef unsigned int		IndexType32;
			}
		}
	}
}

#endif