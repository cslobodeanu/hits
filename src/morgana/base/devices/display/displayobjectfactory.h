#ifndef __MORGANA_BASE_DEVICES_DISPLAY_DISPLAY_OBJECT_FACTORY_H__
#define __MORGANA_BASE_DEVICES_DISPLAY_DISPLAY_OBJECT_FACTORY_H__

#include "reflection.h"
#include "metypes.h"
#include "displayobject.h"
#include "rendepipelinedefs.h"
#include "objects/texture.h"
#include "objects/buffer.h"
#include "objects/rendertarget.h"
#include "objects/shaderprogram.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Devices
		{
			namespace Display
			{
				class DisplayDevice;

				class DisplayObjectFactory : public MEObject
				{
					Array<DisplayObject*>			objects;
					__declare_class_abstract(DisplayObjectFactory, MEObject);

				protected:
					DisplayDevice* displayDevice;
				public:

					DisplayObjectFactory();
					virtual ~DisplayObjectFactory();

					void GenerateHandle(DisplayObject* obj);
					void Delete(DisplayObject* obj);
					void DeleteAll(const int type = -1);

					void Create(DisplayObject* obj, void* createParams);

					const void*	Lock(DisplayObject* obj);
					void	Unlock(DisplayObject* obj);
					void	UpdateSubresource(DisplayObject *obj, const Rect& rect, const int level, const void* data);

				protected:

					virtual unsigned int API_GenerateName(DisplayObject* obj) = 0; 
					virtual void API_Delete(DisplayObject* dobj) = 0;

					virtual void API_CreateTexture(DisplayObject* obj, DisplayObjects::Texture::CreateParams_t *p) = 0;
					virtual void API_CreateBuffer(DisplayObject* obj, DisplayObjects::Buffer::CreateParams_t *p) = 0;
					virtual void API_CreateRenderTarget(DisplayObject* obj, DisplayObjects::RenderTarget::CreateParams_t *p) = 0;
					virtual void API_CreateShaderProgram(DisplayObject* obj, DisplayObjects::ShaderProgram::CreateParams_t *p) = 0;

					virtual const void* API_Lock(DisplayObject* obj) = 0;
					virtual void API_Unlock(DisplayObject* obj) = 0;
					virtual void API_UpdateSubresource(DisplayObject* obj, const Rect& rect, const int level, const void* data) = 0;
				};
			}
		}
	}
};


#endif