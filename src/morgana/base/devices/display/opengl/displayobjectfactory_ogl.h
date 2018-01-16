#ifndef __MORGANA_BASE_DEVICES_DISPLAY_OPENGL_DISPLAY_OBJECT_FACTORY_H__
#define __MORGANA_BASE_DEVICES_DISPLAY_OPENGL_DISPLAY_OBJECT_FACTORY_H__

#include "../displayobjectfactory.h"

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
					class DisplayObjectFactory_OpenGL : public DisplayObjectFactory
					{
						__declare_class(DisplayObjectFactory_OpenGL, DisplayObjectFactory);

					private:

						unsigned int	CompileShader(const char* source, const unsigned int shaderType);
						

					protected:

						virtual unsigned int API_GenerateName(DisplayObject* obj);
						virtual void API_Delete(DisplayObject* obj);

						virtual void API_CreateTexture(DisplayObject* obj, DisplayObjects::Texture::CreateParams_t *p);
						virtual void API_CreateBuffer(DisplayObject* obj, DisplayObjects::Buffer::CreateParams_t *p);
						virtual void API_CreateRenderTarget(DisplayObject* obj, DisplayObjects::RenderTarget::CreateParams_t *p);
						virtual void API_CreateShaderProgram(DisplayObject* obj, DisplayObjects::ShaderProgram::CreateParams_t *p);

						virtual const void* API_Lock(DisplayObject* obj);
						virtual void API_Unlock(DisplayObject* obj);
						virtual void API_UpdateSubresource(DisplayObject* obj, const Rect& rect, const int level, const void* data);
					};
				}
			}
		}
	}
}

#endif