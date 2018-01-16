#ifndef __MORGANA_BASE_DEVICES_DISPLAY_OPENGL_SHADER_PARSER_GLSL_H__
#define __MORGANA_BASE_DEVICES_DISPLAY_OPENGL_SHADER_PARSER_GLSL_H__

#include "../displayobject.h"
namespace MorganaEngine
{
	namespace Base
	{
		namespace Devices
		{
			namespace Display
			{
				namespace OpenGL
				{
					class ShaderParserGLSL
					{
					public:
						void FillUniforms(DisplayObject* shader, const char* source);
						void SplitShaderSource(const char* source, String& out_vertex, String& out_pixel, String& out_geometry);
					};
				}
			}
		}
	}
}

#endif