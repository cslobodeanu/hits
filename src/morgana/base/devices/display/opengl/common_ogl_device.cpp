#include "common_ogl_device.h"
#include "oglheaders.h"
#include "../types/color255.h"
#include "meio.h"
#include "../displayobject.h"
#include "../objects/shaderprogram.h"
#include "../objects/rendertarget.h"
#include "../objects/texture.h"
#include "../../../stats/framestats.h"

using namespace MorganaEngine::Base::Devices::Display;
using namespace MorganaEngine::Base::Devices::Display::DisplayObjects;
using namespace MorganaEngine::Base::Devices::Display::OpenGL;
using namespace MorganaEngine::Base::Stats;

__implement_class_abstract(CommonOpenGLDevice, DisplayDevice);

CommonOpenGLDevice::CommonOpenGLDevice() : DisplayDevice(MDEVICE_DISPLAY_MAIN)
{
	for(int i = 0; i < STATES_COUNT; i++)
		states[i] = false;

	for(int i = 0; i < VERTEX_COMPONENTS_COUNT; i++)
	{
		vertexAttribsStates[i] = false;
		glDisableVertexAttribArray(i);
	}

	currentInputLayout = NULL;

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
}

CommonOpenGLDevice::~CommonOpenGLDevice()
{
}

void CommonOpenGLDevice::AttachCommandCallback(DisplayCommand_t* dc)
{
	switch(dc->code)
	{
	case MDISPLAY_COMMAND_CLEAR_BUFFER:
		{
			dc->funcPtr = (RUNDISPLAYCOMMANDFUNC)(&CommonOpenGLDevice::__ClearBuffer);
			break;
		}
	case MDISPLAY_COMMAND_SET_VIEWPORT:
		{
			dc->funcPtr = (RUNDISPLAYCOMMANDFUNC)(&CommonOpenGLDevice::__SetViewport);
			break;
		}
	case MDISPLAY_COMMAND_BIND_OBJECT:
		{
			dc->funcPtr = (RUNDISPLAYCOMMANDFUNC)(&CommonOpenGLDevice::__BindObject);
			break;
		}

	case MDISPLAY_COMMAND_SET_STATE:
		{
			dc->funcPtr = (RUNDISPLAYCOMMANDFUNC)(&CommonOpenGLDevice::__SetPipelineState);
			break;
		}
	case MDISPLAY_COMMAND_SET_INPUT_LAYOUT:
		{
			dc->funcPtr = (RUNDISPLAYCOMMANDFUNC)(&CommonOpenGLDevice::__SetInputLayout);
			break;
		}
	case MDISPLAY_COMMAND_UPLOAD_SHADER_UNIFORM_DATA:
		{
			dc->funcPtr = (RUNDISPLAYCOMMANDFUNC)(&CommonOpenGLDevice::__UploadShaderUniformData);
			break;
		}
	case MDISPLAY_COMMAND_DRAW_INDEXED:
		{
			dc->funcPtr = (RUNDISPLAYCOMMANDFUNC)(&CommonOpenGLDevice::__DrawIndexed);
			break;
		}
	case MDISPLAY_COMMAND_SET_TEXTURE_FILTERING:
		{
			dc->funcPtr = (RUNDISPLAYCOMMANDFUNC)(&CommonOpenGLDevice::__SetTextureFiltering);
			break;
		}
	case MDISPLAY_COMMAND_SET_TEXTURE_WRAPPING:
		{
			dc->funcPtr = (RUNDISPLAYCOMMANDFUNC)(&CommonOpenGLDevice::__SetTextureWrapping);
			break;
		}
	default:
		NOT_IMPLEMENTED;
	}
}

void CommonOpenGLDevice::__ClearBuffer( void* param )
{
	DISPCMDPARAM_CLEARBUFFER* p = (DISPCMDPARAM_CLEARBUFFER*)param;

	if(p->set_value_bits & MDISPLAY_BUFFER_COLOR)
	{
		glClearColor(p->clear_color_value.r, p->clear_color_value.g, p->clear_color_value.b, p->clear_color_value.a);
	}

	if(p->set_value_bits & MDISPLAY_BUFFER_DEPTH)
	{
		glClearDepth(p->clear_depth_value);
	}

	if(p->set_value_bits & MDISPLAY_BUFFER_STENCIL)
	{
		glClearStencil(p->clear_stencil_value);
	}

	int clearBits = 0;
	if(p->clear_bits & MDISPLAY_BUFFER_COLOR)
	{
		clearBits |= GL_COLOR_BUFFER_BIT;
	}
	if(p->clear_bits & MDISPLAY_BUFFER_DEPTH)
	{
		clearBits |= GL_DEPTH_BUFFER_BIT;
		__SetPipelineState(DepthState::LessEqual);
	}
	if(p->clear_bits & MDISPLAY_BUFFER_STENCIL)
	{
		clearBits |= GL_STENCIL_BUFFER_BIT;
	}

	glClear(clearBits);
}

void CommonOpenGLDevice::__SetViewport( void* param )
{
	DISPCMDPARAM_SETVIEWPORT* p = (DISPCMDPARAM_SETVIEWPORT*)param;
	glViewport(p->rect.x, p->rect.y, p->rect.width, p->rect.height);
}

void CommonOpenGLDevice::__BindObject(void* param)
{
	DISPCMDPARAM_BINDOBJECT* p = (DISPCMDPARAM_BINDOBJECT*)param;

	const int objType = p->GetType();

	switch(objType)
	{
	case DISPLAY_OBJECT_TEXTURE:
		{
			const GLenum texBindingPoints[] = { GL_TEXTURE_2D, GL_TEXTURE_1D, GL_TEXTURE_3D, GL_TEXTURE_CUBE_MAP };
			const GLenum texBP = texBindingPoints[p->GetBindingPoint() - BINDING_TEXTURE_2D];

			if (p->stage > 0)
			{
				glActiveTexture(GL_TEXTURE0 + p->stage);
			}
			glBindTexture(texBP, p->GetHandle());

			if (p->stage > 0)
				glActiveTexture(GL_TEXTURE0);

			FrameStats::current.numTexChanges++;
			break;
		}
	case DISPLAY_OBJECT_SHADER_PROGRAM:
		{
			glUseProgram(p->GetHandle());		
			FrameStats::current.numShaderChanges++;
			break;
		}
	case DISPLAY_OBJECT_BUFFER:
		{
			const GLenum bufBindingPoints[] = {GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER};
			const GLenum bufBP = bufBindingPoints[p->GetBindingPoint() - BINDING_ARRAY_BUFFER];

			glBindBuffer(bufBP, p->GetHandle());

			if (bufBP == GL_ARRAY_BUFFER)
				FrameStats::current.numVBOChanges++;
			else
				FrameStats::current.numIBOChanges++;

			break;
		}
	case DISPLAY_OBJECT_RENDER_TARGET:
		{
			glBindFramebuffer(GL_FRAMEBUFFER, p->GetHandle());
			break;
		}

	default:
		{
			NOT_IMPLEMENTED;
		}
	}
}

void CommonOpenGLDevice::__SetPipelineState(void* param)
{
	DISPCMDPARAM_SETPIPELINESTATE* p = (DISPCMDPARAM_SETPIPELINESTATE*)param;
	PipelineState* state = (PipelineState*)p->state;
	__SetPipelineState(state);
}

void CommonOpenGLDevice::__SetPipelineState(const PipelineState* state)
{
	CLASSDESC cd = state->GetClassDesc();
	if(cd == BlendState::AsClassDesc)
	{
		const BlendState* bs = (BlendState*)state;
		if(state == BlendState::Opaque)
		{
			__DisableState(STATE_BLEND);
		}
		else
		{
			const GLenum factors[] = { GL_ZERO,
								GL_ONE,
								GL_SRC_COLOR,
								GL_ONE_MINUS_SRC_COLOR,
								GL_SRC_ALPHA,
								GL_ONE_MINUS_SRC_ALPHA,
								GL_DST_ALPHA,
								GL_ONE_MINUS_DST_ALPHA,
								GL_DST_COLOR,
								GL_ONE_MINUS_DST_COLOR,
								GL_SRC_ALPHA_SATURATE};
			__EnableState(STATE_BLEND);
			glBlendFunc(factors[bs->srcFactor], factors[bs->dstFactor]);
		}
	}
	else
	if(cd == DepthState::AsClassDesc)
	{
		const DepthState* ds = (DepthState*)state;
		if(state == DepthState::Disabled)
		{
			__DisableState(STATE_DEPTH_TEST);
		}
		else
		{
			const GLenum compareFuncs[] = { GL_NEVER,
											GL_LESS,
											GL_EQUAL,
											GL_LEQUAL,
											GL_GREATER,
											GL_NOTEQUAL,
											GL_GEQUAL,
											GL_ALWAYS};
			__EnableState(STATE_DEPTH_TEST);
			glDepthMask(ds->writeEnabled ? GL_TRUE : GL_FALSE);
			glDepthFunc(compareFuncs[ds->compareFunc]);
		}
	}
	else if(cd == StencilState::AsClassDesc)
	{
		const StencilState* ss = (StencilState*)state;
		if (ss->enabled == false)
		{
			__DisableState(STATE_STENCIL_TEST);
		}
		else
		{
			__EnableState(STATE_STENCIL_TEST);
			glStencilMask(ss->mask);
			glStencilFunc(ss->func, ss->ref, ss->funcMask);
			glStencilOp(ss->stencilFail, ss->depthFail, ss->depthPass);
		}
	}
	else
	if(cd == RasterizerState::AsClassDesc)
	{
		const RasterizerState* rs = (RasterizerState*)state;
		if(rs == RasterizerState::CullNone)
		{
			__DisableState(STATE_CULL_FACE);
		}
		else
		{
			const GLenum cull[] = {GL_BACK, GL_FRONT};
			__EnableState(STATE_CULL_FACE);
			glCullFace(cull[rs->cullFace]);
		}
	}
}

void CommonOpenGLDevice::__EnableDisableState(const int state, const bool enable /*= true*/)
{
	ASSERT(state < STATES_COUNT);

	if(enable == states[state]) return;

	states[state] = enable;

	const GLenum glStates[] = {	GL_DEPTH_TEST, GL_BLEND, GL_CULL_FACE, GL_STENCIL_TEST };
	if(enable)
		glEnable(glStates[state]);
	else
		glDisable(glStates[state]);
}

void CommonOpenGLDevice::__EnableState(const int state)
{
	__EnableDisableState(state, true);
}

void CommonOpenGLDevice::__DisableState(const int state)
{
	__EnableDisableState(state, false);
}

void CommonOpenGLDevice::__SetInputLayout(void* param)
{
	DISPCMDPARAM_SETINPUTLAYOUT* p = (DISPCMDPARAM_SETINPUTLAYOUT*)param;
	currentInputLayout = p->vertexLayout;
}

void CommonOpenGLDevice::__SetInputLayout(VertexLayout* layout, const unsigned int offset)
{
	const GLenum types[] = {GL_FLOAT, GL_UNSIGNED_BYTE};
	const int stride = layout->GetSize();

	for (int index = VERTEX_COMPONENT_POS; index < VERTEX_COMPONENTS_COUNT; index++)
	{
		if (layout->GetComponentSize(index) > 0)
		{
			__EnableVertexAttribArray(index);
			glVertexAttribPointer(index, layout->GetNumComponents(index), types[layout->GetComponentType(index)], layout->GetComponentNormalize(index) ? GL_TRUE : GL_FALSE, stride, (const GLvoid*)(layout->GetComponentOffset(index) + offset));
		}
		else
		{
			__EnableVertexAttribArray(index, false);
		}
	}
}

void CommonOpenGLDevice::__EnableVertexAttribArray(const int which, bool enable /*= true*/)
{
	if(vertexAttribsStates[which] == enable) return;

	if(enable)
		glEnableVertexAttribArray(which);
	else
		glDisableVertexAttribArray(which);

	vertexAttribsStates[which] = enable;
}

void CommonOpenGLDevice::__UploadShaderUniformData(void* param)
{
	DISPCMDPARAM_UPLOADSHADERUNIFORMDATA* p = (DISPCMDPARAM_UPLOADSHADERUNIFORMDATA*)param;
	ShaderProgram::UniformEntry_t* uniform = (ShaderProgram::UniformEntry_t*)p->uniform;

	if (uniform->locationInt == INVALID_UNIFORM_NAME)
		return;

	const DisplayObject* program = GetBoundObject(DISPLAY_OBJECT_SHADER_PROGRAM);
	ASSERT(program);

	ShaderProgram* sp = (ShaderProgram*)program;
	
	if(uniform->locationInt == UNIFORM_NAME_NOT_PARSED)
	{
		ASSERT(uniform->locationStr[0]);
		uniform->locationInt = glGetUniformLocation(program->GetHandle(), uniform->locationStr);
		if (uniform->locationInt < 0)
			uniform->locationInt = INVALID_UNIFORM_NAME;
	}

	const int loc = uniform->locationInt;
	if(loc < 0) return;
	const int type = uniform->dataType;
	const int elems = p->numElements;
	const GLfloat* data = (const GLfloat*)p->data;

	switch(type)
	{
	case UNIFORM_int :
		{
			glUniform1iv(loc, elems, (const GLint*)data);
			break;
		}
	case UNIFORM_float :
		{
			glUniform1fv(loc, elems, data);
			break;
		}
	case UNIFORM_Vector2 :
		{
			glUniform2fv(loc, elems, data);
			break;
		}
	case UNIFORM_Vector3 :
		{
			glUniform3fv(loc, elems, data);
			break;
		}
	case UNIFORM_Vector4 :
		{
			glUniform4fv(loc, elems, data);
			break;
		}
	case UNIFORM_Matrix :
		{
			glUniformMatrix4fv(loc, elems, false, data);
			break;
		}
	default:
		{
			NOT_IMPLEMENTED;
		}
	};
}

void CommonOpenGLDevice::__DrawIndexed(void* param)
{
	DISPCMDPARAM_DRAWINDEXED* p = (DISPCMDPARAM_DRAWINDEXED*)param;
	__SetInputLayout(currentInputLayout, p->vertexOffset);

	const GLenum drawModes[] = {GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_LINES};

	glDrawElements(drawModes[p->drawMode], p->elementsCount, GL_UNSIGNED_INT, (const GLvoid*)(p->indexOffset));
}

void CommonOpenGLDevice::__SetTextureFiltering(void* param)
{
	DISPCMDPARAM_SET_TEXTURE_FILTERING* p = (DISPCMDPARAM_SET_TEXTURE_FILTERING*)param;

	Texture* t = (Texture*)GetBoundObject(DISPLAY_OBJECT_TEXTURE);

	if (t == NULL) return;

	const GLenum minFilters[] = { GL_NEAREST, GL_LINEAR, GL_LINEAR };
	const GLenum maxFilters[] = { GL_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_LINEAR };

	if (t->HasMipmaps())
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilters[p->filtering]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxFilters[p->filtering]);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, p->filtering == Filtering::Bilinear ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, p->filtering == Filtering::Bilinear ? GL_LINEAR : GL_NEAREST);
	}

	t->SetFiltering(p->filtering);
}

void CommonOpenGLDevice::__SetTextureWrapping(void* param)
{
	DISPCMDPARAM_SET_TEXTURE_WRAPPING* p = (DISPCMDPARAM_SET_TEXTURE_WRAPPING*)param;

	Texture* t = (Texture*)GetBoundObject(DISPLAY_OBJECT_TEXTURE);

	if (t == NULL) return;

	const GLenum wrapModes[] = { GL_CLAMP_TO_EDGE, GL_REPEAT };
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapModes[p->wrapMode]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapModes[p->wrapMode]);

	t->SetWrapMode(p->wrapMode);

	
}