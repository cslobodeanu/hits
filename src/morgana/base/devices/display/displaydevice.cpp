#include "displaydevice.h"
#include "../../io/debug.h"
#include "../math/memath.h"
#include "objects/shaderprogram.h"
#include "objects/rendertarget.h"
#include "objects/texture.h"
#include "../../stats/framestats.h"

using namespace MorganaEngine::Base::Devices::Display;
using namespace MorganaEngine::Base::Stats;

__implement_class_abstract(DisplayDevice, MEDevice);

DisplayDevice* DisplayDevice::main = NULL;

DisplayDevice::DisplayDevice(const unsigned char subtype) : MEDevice()
{
	deviceType = MDEVICE_DISPLAY;
	deviceSubType = subtype;

	for(int i = 0; i < MDISPLAY_COMMANDS_COUNT; i++)
		commandsDictionary[i] = NULL;

	currentBlendState = NULL;
	currentRasterizerState = NULL;
	currentDepthState = NULL;
	currentStencilState = NULL;

	if(subtype == MDEVICE_DISPLAY_MAIN)
	{
		main = this;
	}
}

DisplayDevice::~DisplayDevice()
{
	for(int i = 0; i < MDISPLAY_COMMANDS_COUNT; i++)
	{
		DisplayCommand_t* dc = commandsDictionary[i];
		if(dc)
		{
			DeleteCommandParams(dc);
			delete dc;
		}
	}

	if(main == this)
	{
		main = NULL;
	}
}

void DisplayDevice::IssueCommand(const DisplayCommand_t* dc)
{
	if(dc->funcPtr)
		(this->*(dc->funcPtr))(dc->params);
}

DisplayDevice::DisplayCommand_t* DisplayDevice::MakeDisplayCommand( const DisplayCommandCodes_t& code )
{
	if(commandsDictionary[code] == NULL)
	{
		commandsDictionary[code] = new DisplayCommand_t();
		commandsDictionary[code]->code = code;
		commandsDictionary[code]->funcPtr = NULL;
		CreateCommandParams(commandsDictionary[code]);
	}

	return commandsDictionary[code];
}

void DisplayDevice::CreateCommandParams( DisplayCommand_t* dc )
{
	dc->params = NULL;

	switch(dc->code)
	{
	case MDISPLAY_COMMAND_COLOR_MASK:
		{
			dc->params = new DISPCMDPARAM_COLORMASK;
			break;
		}
	case MDISPLAY_COMMAND_SET_VIEWPORT:
		{
			dc->params = new DISPCMDPARAM_SETVIEWPORT;
			break;
		}
	case MDISPLAY_COMMAND_CLEAR_BUFFER:
		{
			dc->params = new DISPCMDPARAM_CLEARBUFFER;
			break;
		}
	case MDISPLAY_COMMAND_BIND_OBJECT:
		{
			dc->params = new DISPCMDPARAM_BINDOBJECT;
			break;
		}
	case MDISPLAY_COMMAND_SET_STATE:
		{
			dc->params = new DISPCMDPARAM_SETPIPELINESTATE;
			break;
		}
	case MDISPLAY_COMMAND_SET_INPUT_LAYOUT:
		{
			dc->params = new DISPCMDPARAM_SETINPUTLAYOUT;
			break;
		}
	case MDISPLAY_COMMAND_UPLOAD_SHADER_UNIFORM_DATA:
		{
			dc->params = new DISPCMDPARAM_UPLOADSHADERUNIFORMDATA;
			break;
		}
	case MDISPLAY_COMMAND_DRAW_INDEXED:
		{
			dc->params = new DISPCMDPARAM_DRAWINDEXED;
			break;
		}
	case MDISPLAY_COMMAND_SET_TEXTURE_FILTERING:
		{
			dc->params = new DISPCMDPARAM_SET_TEXTURE_FILTERING;
			break;
		}
	case MDISPLAY_COMMAND_SET_TEXTURE_WRAPPING:
		{
			dc->params = new DISPCMDPARAM_SET_TEXTURE_WRAPPING;
			break;
		}
	default:
		{
			NOT_IMPLEMENTED;
		}
	}

	AttachCommandCallback(dc);
}

void DisplayDevice::DeleteCommandParams( DisplayCommand_t* dc )
{
	switch(dc->code)
	{
	case MDISPLAY_COMMAND_COLOR_MASK:
		{
			DISPCMDPARAM_COLORMASK* p = (DISPCMDPARAM_COLORMASK*)dc->params;
			SAFE_DEL(p);
			break;
		}
	case MDISPLAY_COMMAND_SET_VIEWPORT:
		{
			DISPCMDPARAM_SETVIEWPORT* p = (DISPCMDPARAM_SETVIEWPORT*)dc->params;
			SAFE_DEL(p);
			break;
		}
	case MDISPLAY_COMMAND_CLEAR_BUFFER:
		{
			DISPCMDPARAM_CLEARBUFFER* p = (DISPCMDPARAM_CLEARBUFFER*)dc->params;
			SAFE_DEL(p);
			break;
		}
	case MDISPLAY_COMMAND_SET_STATE:
		{
			DISPCMDPARAM_SETPIPELINESTATE* p = (DISPCMDPARAM_SETPIPELINESTATE*)dc->params;
			SAFE_DEL(p);
			break;
		}
	case MDISPLAY_COMMAND_SET_INPUT_LAYOUT:
		{
			DISPCMDPARAM_SETINPUTLAYOUT* p = (DISPCMDPARAM_SETINPUTLAYOUT*)dc->params;
			SAFE_DEL(p);
			break;
		}
	case MDISPLAY_COMMAND_UPLOAD_SHADER_UNIFORM_DATA:
		{
			DISPCMDPARAM_UPLOADSHADERUNIFORMDATA* p = (DISPCMDPARAM_UPLOADSHADERUNIFORMDATA*)dc->params;
			SAFE_DEL(p);
			break;
		}
	case MDISPLAY_COMMAND_DRAW_INDEXED:
		{
			DISPCMDPARAM_DRAWINDEXED* p = (DISPCMDPARAM_DRAWINDEXED*)dc->params;
			SAFE_DEL(p);
			break;
		}
	case MDISPLAY_COMMAND_BIND_OBJECT:
		{
			DISPCMDPARAM_BINDOBJECT* p = (DISPCMDPARAM_BINDOBJECT*)dc->params;
			SAFE_DEL(p);
			break;
		}
	case MDISPLAY_COMMAND_SET_TEXTURE_FILTERING:
		{
			DISPCMDPARAM_SET_TEXTURE_FILTERING* p = (DISPCMDPARAM_SET_TEXTURE_FILTERING*)dc->params;
			SAFE_DEL(p);
			break;
		}
	case MDISPLAY_COMMAND_SET_TEXTURE_WRAPPING:
		{
			DISPCMDPARAM_SET_TEXTURE_WRAPPING* p = (DISPCMDPARAM_SET_TEXTURE_WRAPPING*)dc->params;
			SAFE_DEL(p);
			break;
		}
	default:
		{
			NOT_IMPLEMENTED;
		}
	}
}

void DisplayDevice::AttachCommandCallback( DisplayCommand_t* dc )
{
	dc->funcPtr = NULL;
}

void DisplayDevice::PresentBuffer()
{

}

void DisplayDevice::ClearBuffer(int bits, Color clearColor /*= Color::TRANSPARENT_BLACK*/, float clearDepth /*= 0.0f*/, unsigned char clearStencil /*= 0*/)
{
	DisplayCommand_t* dc = MakeDisplayCommand(MDISPLAY_COMMAND_CLEAR_BUFFER);
	DISPCMDPARAM_CLEARBUFFER* p = (DISPCMDPARAM_CLEARBUFFER*)dc->params;
	p->clear_bits = bits;
	p->set_value_bits = 0;
	p->clear_color_value = clearColor;
	p->clear_stencil_value = clearStencil;
	p->clear_depth_value = clearDepth;

	if(clearColor != clearBufferParam.clear_color_value)
	{
		p->set_value_bits |= MDISPLAY_BUFFER_COLOR;
		clearBufferParam.clear_color_value = clearColor;
	}

	if(clearDepth != clearBufferParam.clear_depth_value)
	{
		p->set_value_bits |= MDISPLAY_BUFFER_DEPTH;
		clearBufferParam.clear_depth_value = clearDepth;
	}
	
	if(clearStencil != clearBufferParam.clear_stencil_value)
	{
		p->set_value_bits |= MDISPLAY_BUFFER_STENCIL;
		clearBufferParam.clear_stencil_value = clearStencil;
	}

	IssueCommand(dc);
}

void DisplayDevice::SetViewport(const Rect& rect)
{
	if(rect != viewportParam.rect)
	{
		viewportParam.rect = rect;

		DisplayCommand_t* dc = MakeDisplayCommand(MDISPLAY_COMMAND_SET_VIEWPORT);
		DISPCMDPARAM_SETVIEWPORT* p = (DISPCMDPARAM_SETVIEWPORT*)dc->params;
		memcpy(p, &viewportParam, sizeof(DISPCMDPARAM_SETVIEWPORT));
		IssueCommand(dc);
	}
}

void DisplayDevice::BindObject(DisplayObject* obj, int stage /*= 0*/)
{
	obj->GenerateHandleEventually();
	
	DisplayObject* current = boundDisplayObjects[obj->GetDisplayObjectType()].bindingPoints[obj->GetBindingPoint()].stages[stage];
 	if(current == obj) return;

	boundDisplayObjects[obj->GetDisplayObjectType()].bindingPoints[obj->GetBindingPoint()].stages[stage] = obj;

	DisplayCommand_t* dc = MakeDisplayCommand(MDISPLAY_COMMAND_BIND_OBJECT);
	DISPCMDPARAM_BINDOBJECT* p = (DISPCMDPARAM_BINDOBJECT*)dc->params;
	p->object = obj;
	p->stage = stage;
	IssueCommand(dc);

	p->object->OnObjectBound(p->GetBindingPoint(), p->stage);
}

DisplayObject* DisplayDevice::GetBoundObject(const int type, const int bindingPoint /*= 0*/, int stage /*= 0*/) const
{
	return boundDisplayObjects[type].bindingPoints[bindingPoint].stages[stage];
}

void DisplayDevice::BindObjectNull(const int type, const int bindingPoint /*= 0*/, int stage /*= 0*/)
{
	boundDisplayObjects[type].bindingPoints[bindingPoint].stages[stage] = NULL;

	DisplayCommand_t* dc = MakeDisplayCommand(MDISPLAY_COMMAND_BIND_OBJECT);
	DISPCMDPARAM_BINDOBJECT* p = (DISPCMDPARAM_BINDOBJECT*)dc->params;
	p->object = NULL;
	p->type = type;
	p->bindingPoint = bindingPoint;
	p->stage = stage;
	IssueCommand(dc);
}

void DisplayDevice::SetPipelineState(const PipelineState* state)
{
	PipelineState* st = (PipelineState*)state;
	if(st->IsKindOf(BlendState::AsClassDesc))
	{
		if(st == currentBlendState) return;
		currentBlendState = (BlendState*)state;
	}
	else
	if(st->IsKindOf(DepthState::AsClassDesc))
	{
		if(st == currentDepthState) return;
		currentDepthState = (DepthState*)state;
	}
	else
	if (st->IsKindOf(StencilState::AsClassDesc))
	{
		StencilState* ss = (StencilState*)state;
		if (ss->Equals(currentStencilState)) return;
		currentStencilState = ss;
	}
	else
	if(st->IsKindOf(RasterizerState::AsClassDesc))
	{
		if(st == currentRasterizerState) return;
		currentRasterizerState = (RasterizerState*)state;
	}

	DisplayCommand_t* dc = MakeDisplayCommand(MDISPLAY_COMMAND_SET_STATE);
	DISPCMDPARAM_SETPIPELINESTATE* p = (DISPCMDPARAM_SETPIPELINESTATE*)dc->params;
	p->state = state;
	IssueCommand(dc);
}

void DisplayDevice::SetInputLayout(VertexLayout* def)
{
	if (setInputLayoutParam.vertexLayout == def) return;

	setInputLayoutParam.vertexLayout = def;

	DisplayCommand_t* dc = MakeDisplayCommand(MDISPLAY_COMMAND_SET_INPUT_LAYOUT);
	DISPCMDPARAM_SETINPUTLAYOUT* p = (DISPCMDPARAM_SETINPUTLAYOUT*)dc->params;
	p->vertexLayout = def;
	IssueCommand(dc);
} 

void DisplayDevice::UploadShaderUniformData(const DisplayObjects::ShaderProgram::UniformEntry_t* uniform, const void* data, const int numElems /*= 1*/)
{
 	DisplayCommand_t* dc = MakeDisplayCommand(MDISPLAY_COMMAND_UPLOAD_SHADER_UNIFORM_DATA);
	DISPCMDPARAM_UPLOADSHADERUNIFORMDATA* p = (DISPCMDPARAM_UPLOADSHADERUNIFORMDATA*)dc->params;
	p->uniform = uniform;
	p->data = data;
	p->numElements = numElems;
	IssueCommand(dc);
}

void DisplayDevice::DrawIndexed(const int drawMode, const unsigned int count, const unsigned int verticesOffset /*= 0*/, const unsigned int indicesOffset /*= 0*/)
{
	DisplayObjects::ShaderProgram* program = (DisplayObjects::ShaderProgram*)GetBoundObject(DISPLAY_OBJECT_SHADER_PROGRAM);
	if(!program)
	{
		return;
	}

	FrameStats::current.numDrawCalls++;

	if (drawMode == DRAWMODE_TRIANGLES)
		FrameStats::current.numTriangles += count / 3;
	else if (drawMode == DRAWMODE_TRISTRIP)
		FrameStats::current.numTriangles += count - 2;

	DisplayCommand_t* dc = MakeDisplayCommand(MDISPLAY_COMMAND_DRAW_INDEXED);
	DISPCMDPARAM_DRAWINDEXED* p = (DISPCMDPARAM_DRAWINDEXED*)dc->params;
	p->drawMode = drawMode;
	p->elementsCount = count;
	p->vertexOffset = verticesOffset;
	p->indexOffset = indicesOffset;
	IssueCommand(dc);
}

void MorganaEngine::Base::Devices::Display::DisplayDevice::ResetBoundObjects()
{
	for(int i = 0; i < DISPLAY_OBJECTS_TYPES_COUNT; i++)
		boundDisplayObjects[i].Reset();
}

void DisplayDevice::SetTextureFiltering(Filtering filtering, bool force /*= false*/)
{
	DisplayObjects::Texture* tex = (DisplayObjects::Texture*)GetBoundObject(DISPLAY_OBJECT_TEXTURE);
	if (tex != NULL && tex->GetFiltering() == filtering && !force)
	{
		return;
	}

	DisplayCommand_t* dc = MakeDisplayCommand(MDISPLAY_COMMAND_SET_TEXTURE_FILTERING);
	DISPCMDPARAM_SET_TEXTURE_FILTERING* p = (DISPCMDPARAM_SET_TEXTURE_FILTERING*)dc->params;
	p->filtering = filtering;
	IssueCommand(dc);
}

void DisplayDevice::SetTextureWrapping(WrapMode wrapMode, bool force /*= false*/)
{
	DisplayObjects::Texture* tex = (DisplayObjects::Texture*)GetBoundObject(DISPLAY_OBJECT_TEXTURE);
	if (tex != NULL && tex->GetWrapMode() == wrapMode && !force)
	{
		return;
	}

	DisplayCommand_t* dc = MakeDisplayCommand(MDISPLAY_COMMAND_SET_TEXTURE_WRAPPING);
	DISPCMDPARAM_SET_TEXTURE_WRAPPING* p = (DISPCMDPARAM_SET_TEXTURE_WRAPPING*)dc->params;
	p->wrapMode = wrapMode;
	IssueCommand(dc);
}

void DisplayDevice::PostConnect(const DeviceContext_t& dc)
{
	DisplayObjects::Texture::InitializeDefaultTextures();
}

void DisplayDevice::DrawIndexed(const int drawMode, const unsigned int count, const Managed::MemoryPage::Block_t* vertices, const Managed::MemoryPage::Block_t* indices)
{
	DrawIndexed(drawMode, count, vertices->GetOffset(), indices->GetOffset());
}
