#ifndef __MORGANA_BASE_DEVICES_DISPLAY_COMMON_OGL_DEVICE_H__
#define __MORGANA_BASE_DEVICES_DISPLAY_COMMON_OGL_DEVICE_H__

#include "../displaydevice.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Devices
		{
			namespace Display
			{
				class DisplayObject;
				namespace OpenGL
				{
					class CommonOpenGLDevice : public DisplayDevice
					{
						__declare_class_abstract(CommonOpenGLDevice, DisplayDevice);

					public:
						CommonOpenGLDevice();
						virtual ~CommonOpenGLDevice();

					protected:

						virtual void	AttachCommandCallback(DisplayCommand_t* dc);

					private:
						void			__ClearBuffer(void* param);
						void			__SetViewport(void* param);				
						void			__BindObject(void* param);
						void			__SetPipelineState(void* param);
						void			__SetPipelineState(const PipelineState* state);
						void			__SetInputLayout(void* param);
						void			__UploadShaderUniformData(void* param);
						void			__DrawIndexed(void* param);
						void			__SetTextureFiltering(void* param);
						void			__SetTextureWrapping(void* param);


						enum States_t
						{
							STATE_DEPTH_TEST,
							STATE_BLEND,
							STATE_CULL_FACE,
							STATE_STENCIL_TEST,
							STATES_COUNT
						};

						bool			states[STATES_COUNT];
						bool			vertexAttribsStates[VERTEX_COMPONENTS_COUNT];

						VertexLayout*	currentInputLayout;

						void			__EnableState(const int state);
						void			__DisableState(const int state);
						void			__EnableDisableState(const int state, const bool enable);

						void			__EnableVertexAttribArray(const int which, bool enable = true);

						void			__SetInputLayout(VertexLayout* def, const unsigned int offset);
					};
				}
			}
		}
	}
}

#endif