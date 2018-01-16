#include "displayobjectfactory.h"
#include "displayobject.h"
#include "rendepipelinedefs.h"
#include "displaydevice.h"
#include "../medevices.h"
#include "objects/texture.h"
#include "mememory.h"

using namespace MorganaEngine::Base::Devices::Display;
using namespace MorganaEngine::Base::Devices::Display::DisplayObjects;

__implement_class_abstract(DisplayObjectFactory, MEObject);

DisplayObjectFactory::DisplayObjectFactory()
{
	displayDevice = DisplayDevice::main;
}

DisplayObjectFactory::~DisplayObjectFactory()
{

}

void DisplayObjectFactory::GenerateHandle(DisplayObject* obj)
{
	Delete(obj);
	objects.Add(obj);
	unsigned int handle = API_GenerateName(obj);
	obj->SetHandle(handle);
	obj->display = displayDevice;
}

void DisplayObjectFactory::Delete(DisplayObject* obj)
{
	if(obj->IsValid())
	{
		if(obj->GetDisplayObjectType() == DISPLAY_OBJECT_BUFFER)
		{
			MemTracker::DecreaseAllocatedSize(((DisplayObjects::Buffer::CreateParams_t*)obj->createParams)->size, obj->GetBindingPoint() == 0 ? MemTracker::MEMORY_TYPE_GPU_VBO : MemTracker::MEMORY_TYPE_GPU_IBO);
		}

		if(obj->GetDisplayObjectType() == DISPLAY_OBJECT_TEXTURE)
		{
			MemTracker::DecreaseAllocatedSize(((DisplayObjects::Texture::CreateParams_t*)obj->createParams)->GetAllocSize(), MemTracker::MEMORY_TYPE_GPU_TEX);
		}
		
				objects.Remove(obj);
		API_Delete(obj);
		obj->SetHandle(0);
	}
}

void DisplayObjectFactory::DeleteAll(const int type /*= -1*/)
{
	if(type < 0)
	{
		while(objects.Length())
			SAFE_DEL(objects[0]);

		return;
	}

	for(int i = 0; i < objects.Length(); i++)
	{
		if(objects[i]->GetDisplayObjectType() == type)
		{
			SAFE_DEL(objects[i]);
			i--;
		}
	}
}

void DisplayObjectFactory::Create(DisplayObject* obj, void* createParams)
{
	const int type = obj->GetDisplayObjectType();

	displayDevice->BindObject(obj);

	obj->IncrementVersion();

	switch(type)
	{
	case DISPLAY_OBJECT_TEXTURE		:
		{
			obj->SetCreateParams(createParams, sizeof(DisplayObjects::Texture::CreateParams_t));
			API_CreateTexture(obj, (DisplayObjects::Texture::CreateParams_t*)createParams);
			MemTracker::IncreaseAllocatedSize(((DisplayObjects::Texture::CreateParams_t*)createParams)->GetAllocSize(), MemTracker::MEMORY_TYPE_GPU_TEX);
			break;
		}
	case DISPLAY_OBJECT_BUFFER			:
		{
			obj->SetCreateParams(createParams, sizeof(DisplayObjects::Buffer::CreateParams_t));
			API_CreateBuffer(obj, (DisplayObjects::Buffer::CreateParams_t*)createParams);
			MemTracker::IncreaseAllocatedSize(((DisplayObjects::Buffer::CreateParams_t*)createParams)->size, obj->GetBindingPoint() == 0 ? MemTracker::MEMORY_TYPE_GPU_VBO : MemTracker::MEMORY_TYPE_GPU_IBO);
			break;
		}
	case DISPLAY_OBJECT_RENDER_TARGET	:
		{
			DisplayObjects::RenderTarget::CreateParams_t* params = (DisplayObjects::RenderTarget::CreateParams_t*)createParams;
			DisplayObjects::Texture::CreateParams_t tp;
			tp.width = params->width;
			tp.height = params->height;
			tp.wrap = WrapMode::Clamp;
			if(params->attachTexturesBits & MDISPLAY_BUFFER_COLOR)
			{
				params->colorTexture = new Texture();
				tp.format = params->colorFormat;
				Create(params->colorTexture, &tp);
			}

			if(params->attachTexturesBits & MDISPLAY_BUFFER_DEPTH)
			{
				params->depthTexture = new Texture();
				tp.format = PixelFormat::FormatDepth;
				tp.pixelType = PixelType::UnsignedInt;
				Create(params->depthTexture, &tp);
			}

			obj->SetCreateParams(createParams, sizeof(DisplayObjects::RenderTarget::CreateParams_t));
			API_CreateRenderTarget(obj, params);
			break;
		}

	case DISPLAY_OBJECT_SHADER_PROGRAM	:
		{
			obj->SetCreateParams(createParams, sizeof(DisplayObjects::ShaderProgram::CreateParams_t));
			API_CreateShaderProgram(obj, (DisplayObjects::ShaderProgram::CreateParams_t*)createParams);
			break;
		}
	}
}

const void* DisplayObjectFactory::Lock(DisplayObject* obj)
{
	if(obj->IsLocked()) return obj->GetLockedPtr();
	displayDevice->BindObject(obj);
	const void* ptr = API_Lock(obj);

	ASSERT(ptr != null, "Cannot Lock.");

	obj->SetLockedPtr(ptr);
	return ptr;
}

void DisplayObjectFactory::Unlock(DisplayObject* obj)
{
	if(obj->IsLocked() == false) return;
	displayDevice->BindObject(obj);
	API_Unlock(obj);
	obj->SetLockedPtr(NULL);
}

void DisplayObjectFactory::UpdateSubresource(DisplayObject* obj, const Rect& rect, const int level, const void* data)
{
	displayDevice->BindObject(obj);
	API_UpdateSubresource(obj, rect, level, data);
}
