#include "shaderparser_glsl.h"
#include "../objects/shaderprogram.h"
#include "meparsers.h"

using namespace MorganaEngine::Base::Devices::Display::OpenGL;
using namespace MorganaEngine::Base::Devices::Display;
using namespace MorganaEngine::Base::Devices::Display::DisplayObjects;

int FromStringToType(const char* str)
{
	if(strcmp(str, "float") == 0) return UNIFORM_float;
	if(strcmp(str, "vec2") == 0) return UNIFORM_Vector2;
	if(strcmp(str, "vec3") == 0) return UNIFORM_Vector3;
	if(strcmp(str, "vec4") == 0) return UNIFORM_Vector4;
	if(strcmp(str, "mat4") == 0) return UNIFORM_Matrix;

	if(strcmp(str, "int") == 0) return UNIFORM_int;
	if(strcmp(str, "sampler2D") == 0) return UNIFORM_int;

	return -1;
}

void ShaderParserGLSL::FillUniforms(DisplayObject* shader, const char* source)
{
	ShaderProgram* sh = (ShaderProgram*)shader;
	sh->ClearUniformEntries();
	
	String _source(source);
	Array<String> lines, words, temp;
	_source.Split("\n\r;", lines);
	for(int i = 0; i < lines.Length(); i++)
	{
		lines[i].Split(" \t", words);
		if(words.Length() < 3) continue;
		if (words[0].Equals("uniform") == false && words[0].Equals("sampler2D") == false) continue;

		int numItems = 1;
		String uName = words.Last();
		if(uName.Find("[") > 0)
		{
			uName.Split("[ ]", temp);
			uName = temp.First();

			if(temp.Last().IsNumber())
				numItems = temp.Last().ToInt();
			else
				numItems = 256;
		}

		sh->AddUniformEntry(uName, FromStringToType(words[words.Length() - 2]), numItems);
	}

	for (int i = 0; i < 16; i++)
	{
		String s = String("tex") + i;
		if (sh->HasParameter(s))
			sh->SetInt(s, i);
	}
}

void ShaderParserGLSL::SplitShaderSource(const char* source, String& out_vertex, String& out_pixel, String& out_geometry)
{
	Array<CodeBlockParser::Block_t> blocks;
	CodeBlockParser cbp;
	cbp.Parse(source, blocks);

	for(int i = 0; i < blocks.Length(); i++)
	{
		if(blocks[i].name.EqualsNC("vertexshader"))
		{
			out_vertex = blocks[i].contents;
		}

		if(blocks[i].name.EqualsNC("pixelshader") || blocks[i].name.EqualsNC("fragmentshader"))
		{
			out_pixel = blocks[i].contents;
		}

		if(blocks[i].name.EqualsNC("geometryshader"))
		{
			out_geometry = blocks[i].contents;
		}
	}
}