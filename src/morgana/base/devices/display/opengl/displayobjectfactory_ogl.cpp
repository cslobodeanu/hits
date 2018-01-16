#include "displayobjectfactory_ogl.h"
#include "../displayobject.h"
#include "../objects/texture.h"
#include "../displaydevice.h"
#include "../rendepipelinedefs.h"
#include "oglheaders.h"
#include "meio.h"
#include "../memory/mememory.h"
#include "../objects/shaderprogram.h"
#include "shaderparser_glsl.h"
#include "cbp.h"
#include <stdio.h>

using namespace MorganaEngine::Base::Devices::Display;
using namespace MorganaEngine::Base::Devices::Display::OpenGL;
using namespace MorganaEngine::Base::Memory;
using namespace MorganaEngine::Base::Parsers;

__implement_class(DisplayObjectFactory_OpenGL, DisplayObjectFactory);


unsigned int DisplayObjectFactory_OpenGL::API_GenerateName(DisplayObject* obj)
{
	const char type = obj->GetDisplayObjectType();
	unsigned int handle = 0;
	switch(type)
	{
		case DISPLAY_OBJECT_TEXTURE			: glGenTextures(1, &handle); break;
		case DISPLAY_OBJECT_BUFFER			: glGenBuffers(1, &handle); break;
		case DISPLAY_OBJECT_SHADER_PROGRAM	: handle = glCreateProgram(); break;
		case DISPLAY_OBJECT_RENDER_TARGET	: glGenFramebuffers(1, &handle); break;
	};
	return handle;
}

void DisplayObjectFactory_OpenGL::API_Delete(DisplayObject* obj)
{
	const char type = obj->GetDisplayObjectType();
	unsigned int handle = obj->GetHandle();

	switch(type)
	{
		case DISPLAY_OBJECT_TEXTURE			: glDeleteTextures(1, &handle); break;
		case DISPLAY_OBJECT_BUFFER			: glDeleteBuffers(1, &handle); break;
		case DISPLAY_OBJECT_SHADER_PROGRAM	: glDeleteProgram(handle); break;
		case DISPLAY_OBJECT_RENDER_TARGET	: glDeleteFramebuffers(1, &handle);
	}
}

const GLenum pixFormats[] = {
	GL_ZERO,
	GL_ALPHA,
	GL_LUMINANCE,
	GL_LUMINANCE_ALPHA,
	GL_DEPTH_COMPONENT,
	GL_RGB,
	GL_RGBA,
	GL_RGBA4,
	GL_RGB5,
	GL_RGB5_A1,
	0x8C00,//GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG,
	0x8C01,//GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG,
	0x8C02,//GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG,
	0x8C03,//GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG,
	0x83F0,//GL_COMPRESSED_RGB_S3TC_DXT1,
	0x83F1,//GL_COMPRESSED_RGBA_S3TC_DXT1,
	0x83F2,//GL_COMPRESSED_RGBA_S3TC_DXT3,
	0x83F3,//GL_COMPRESSED_RGBA_S3TC_DXT5,
	0x8C92,//GL_ATC_RGB_AMD,
	0x8C93,//GL_ATC_RGBA_EXPLICIT_ALPHA_AMD,
	0x87EE,//GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD,
	0x8D64,//GL_ETC1_RGB8,
};

const GLenum internalFormats[] = {
	GL_ZERO,
	GL_ALPHA,
	GL_LUMINANCE,
	GL_LUMINANCE_ALPHA,
	GL_DEPTH_COMPONENT,
	GL_RGB,
	GL_RGBA,
	GL_RGBA4,
	GL_RGB5,
	GL_RGB5_A1,
	0x8C00,//GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG,
	0x8C01,//GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG,
	0x8C02,//GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG,
	0x8C03,//GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG,
	0x83F0,//GL_COMPRESSED_RGB_S3TC_DXT1,
	0x83F1,//GL_COMPRESSED_RGBA_S3TC_DXT1,
	0x83F2,//GL_COMPRESSED_RGBA_S3TC_DXT3,
	0x83F3,//GL_COMPRESSED_RGBA_S3TC_DXT5,
	0x8C92,//GL_ATC_RGB_AMD,
	0x8C93,//GL_ATC_RGBA_EXPLICIT_ALPHA_AMD,
	0x87EE,//GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD,
	0x8D64,//GL_ETC1_RGB8,
};

const GLenum pixelTypes[] = {
	GL_UNSIGNED_BYTE,
	GL_UNSIGNED_INT
};

void DisplayObjectFactory_OpenGL::API_CreateTexture(DisplayObject* obj, DisplayObjects::Texture::CreateParams_t *p)
{
	const GLenum texBindingPoints[] = { GL_TEXTURE_2D, GL_TEXTURE_1D, GL_TEXTURE_3D, GL_TEXTURE_CUBE_MAP };
	const GLenum texBP = texBindingPoints[obj->GetBindingPoint() - BINDING_TEXTURE_2D];

	const GLenum format = pixFormats[p->format];
	
	const GLenum internalFormat = internalFormats[p->format];

	const GLenum wrapModes[] = { GL_CLAMP_TO_EDGE, GL_REPEAT };

	bool isCompressed = false;
	LocalMemoryBlock buffer(p->width * p->height * 4);
	buffer.FillZero();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	if(isCompressed)
	{
		ASSERT(false, "NOT IMPLEMENTED");
	}
	else
	{
		int level = 0;
		int w = p->width;
		int h = p->height;
		while(w > 0 && h > 0)
		{
			if(obj->GetBindingPoint() == BINDING_TEXTURE_2D)
				glTexImage2D(texBP, level, internalFormat, w, h, 0, format, pixelTypes[p->pixelType], buffer);
			if(!p->hasMipmaps) break;
			w >>= 1;
			h >>= 1;
			level++;
		}

		displayDevice->SetTextureFiltering(p->filtering, true);
		displayDevice->SetTextureWrapping(p->wrap, true);
	}
}

void DisplayObjectFactory_OpenGL::API_CreateBuffer(DisplayObject* obj, DisplayObjects::Buffer::CreateParams_t *p)
{
	const GLenum bufBindingPoints[] = {GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER};
	const GLenum bufBP = bufBindingPoints[obj->GetBindingPoint() - BINDING_ARRAY_BUFFER];

	const GLenum bufAccessTypes[] = {GL_STATIC_DRAW, GL_DYNAMIC_DRAW, GL_STREAM_DRAW};
	const GLenum bufAccess = bufAccessTypes[p->accessMode];

	LocalMemoryBlock buf(p->size);
	glBufferData(bufBP, p->size, buf, bufAccess);
}

void DisplayObjectFactory_OpenGL::API_CreateRenderTarget(DisplayObject* obj, DisplayObjects::RenderTarget::CreateParams_t *p)
{
	if(p->createBuffersBits & MDISPLAY_BUFFER_COLOR)
	{
		if(p->colorTexture)
		{
			displayDevice->BindObject(obj);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, p->colorTexture->GetHandle(), 0);
		}
	}

	if(p->createBuffersBits & MDISPLAY_BUFFER_DEPTH)
	{
		if(p->depthTexture)
		{
			displayDevice->BindObject(obj);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, p->depthTexture->GetHandle(), 0);
		}
		else
		{
			glGenRenderbuffers(1, &p->depthBuffer);
			glBindRenderbuffer(GL_RENDERBUFFER, p->depthBuffer);

			if((p->createBuffersBits & MDISPLAY_BUFFER_STENCIL) && p->stencilTexture == NULL)
			{
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, p->width, p->height);
			}
			else
			{
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, p->width, p->height);
			}
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, p->depthBuffer);
		}
	}

	if(p->createBuffersBits & MDISPLAY_BUFFER_STENCIL)
	{
		if(p->stencilTexture)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, p->stencilTexture->GetHandle(), 0);
		}
		else if(p->depthTexture)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, p->depthTexture->GetHandle(), 0);
		}
		else
		{
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, p->depthBuffer);
		}
	}

#if defined(DEBUG) || defined(_DEBUG)
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(status != GL_FRAMEBUFFER_COMPLETE)
	{
		DEBUG_OUT("Incomplete framebuffer object: 0x%x\n", status);
	}
#endif

}

void DisplayObjectFactory_OpenGL::API_CreateShaderProgram(DisplayObject* obj, DisplayObjects::ShaderProgram::CreateParams_t *p)
{
	String vshSource;
	String fshSource;
	String geomSource;
	ShaderParserGLSL sp;
	sp.SplitShaderSource(p->shaderSource, vshSource, fshSource, geomSource);

#ifdef OS_WINDOWS
	String _stdDefines = "#version 120\n#define lowp\n#define mediump\n#define highp\n";
	vshSource = _stdDefines + vshSource;
	fshSource = _stdDefines + fshSource;
#endif

	const unsigned int vsh = CompileShader(vshSource.c_str(), GL_VERTEX_SHADER);
	const unsigned int fsh = CompileShader(fshSource.c_str(), GL_FRAGMENT_SHADER);

	if (vsh == 0 || fsh == 0)
	{
		obj->valid = false;
		return;
	}

	const unsigned int program = obj->GetHandle();

	// delete existing shaders
	GLuint existingShaders[255];
	GLsizei count;
	glGetAttachedShaders(program, 255, &count, existingShaders);
	for (int i = 0; i < count; i++)
		glDetachShader(program, existingShaders[i]);

	glAttachShader(program, vsh);
	glAttachShader(program, fsh);

	glDeleteShader(vsh);
	glDeleteShader(fsh);
	
	for(int i = 0; i < VERTEX_COMPONENTS_COUNT; i++)
		glBindAttribLocation(program, i, VertexAttribNames[i].c_str());

	glLinkProgram(program);

#if defined(DEBUG) || defined(_DEBUG)
	GLint logLength = 0;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 1)
	{
		LocalMemoryBlock log(logLength);
		glGetProgramInfoLog(program, logLength, &logLength, log);
		DEBUG_OUT("Program link log:\n%s", log);
	}
#endif

	GLint state;
	glGetProgramiv(program, GL_LINK_STATUS, &state);
	obj->valid = state == GL_TRUE;

	DisplayDevice::main->BindObjectNull(DISPLAY_OBJECT_SHADER_PROGRAM);
	sp.FillUniforms(obj, p->shaderSource);
}

unsigned int DisplayObjectFactory_OpenGL::CompileShader(const char* source, const unsigned int shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	const GLint size = strlen(source) ;
	glShaderSource(shader, 1, &source, &size);
	glCompileShader(shader);

#if defined(DEBUG) || defined(_DEBUG)
	GLint logLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 1)
	{
		LocalMemoryBlock log(logLength + 1);
		glGetShaderInfoLog(shader, logLength, &logLength, log);
		char* str = (char*)log.ptr();
		str[logLength] = 0;
		CodeBlockParser::PrintSource(source);
		DEBUG_OUT("\nSHADER COMPILE LOG:\n%s", str);
	}
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == 0)
	{
		glDeleteShader(shader);
		return 0;
	}
#endif
	return shader;
}

const void* DisplayObjectFactory_OpenGL::API_Lock(DisplayObject* obj)
{
	if(obj->GetDisplayObjectType() == DISPLAY_OBJECT_BUFFER)
	{
		const GLenum bufBindingPoints[] = {GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER};
		const GLenum bufBP = bufBindingPoints[obj->GetBindingPoint() - BINDING_ARRAY_BUFFER];
		return glMapBuffer(bufBP, GL_WRITE_ONLY);
	}
	else
	{
		NOT_PERMITTED;
	}

	return NULL;
}

void DisplayObjectFactory_OpenGL::API_Unlock(DisplayObject* obj)
{
	if(obj->GetDisplayObjectType() == DISPLAY_OBJECT_BUFFER)
	{
		const GLenum bufBindingPoints[] = {GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER};
		const GLenum bufBP = bufBindingPoints[obj->GetBindingPoint() - BINDING_ARRAY_BUFFER];
		glUnmapBuffer(bufBP);
	}
	else
	{
		NOT_PERMITTED;
	}
}

void DisplayObjectFactory_OpenGL::API_UpdateSubresource(DisplayObject* obj, const Rect& rect, const int level, const void* data)
{
	if(obj->GetDisplayObjectType() == DISPLAY_OBJECT_BUFFER)
	{
		const GLenum bufBindingPoints[] = {GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER};
		const GLenum bufBP = bufBindingPoints[obj->GetBindingPoint() - BINDING_ARRAY_BUFFER];
		glBufferSubData(bufBP, rect.x, rect.y, data);
	}
	else
	if(obj->GetDisplayObjectType() == DISPLAY_OBJECT_TEXTURE)
	{
		const GLenum texBindingPoints[] = { GL_TEXTURE_2D, GL_TEXTURE_1D, GL_TEXTURE_3D, GL_TEXTURE_CUBE_MAP };
		const GLenum texBP = texBindingPoints[obj->GetBindingPoint() - BINDING_TEXTURE_2D];

		DisplayObjects::Texture::CreateParams_t* p = (DisplayObjects::Texture::CreateParams_t*)obj->GetCreateParams();
		glTexSubImage2D(texBP, level, rect.x, rect.y, rect.width, rect.height, internalFormats[p->format], pixelTypes[p->pixelType], data);
	}
	else
	{
		NOT_PERMITTED;
	}
}
