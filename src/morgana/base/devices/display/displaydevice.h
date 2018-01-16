#ifndef __MORGANA_BASE_DEVICES_DISPLAY_DISPLAYDEVICE_H__
#define __MORGANA_BASE_DEVICES_DISPLAY_DISPLAYDEVICE_H__

#include "../medevice.h"
#include "dispcmd.h"
#include "rendepipelinedefs.h"
#include "displayobject.h"
#include "pipelinestates.h"
#include "objects/shaderprogram.h"
#include "../medevices.h"
#include "mememory.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Devices
		{
			namespace Display
			{
				class DisplayDevice: public MEDevice
				{
					__declare_class_abstract(DisplayDevice, MEDevice);
				protected:

					typedef void(DisplayDevice::*RUNDISPLAYCOMMANDFUNC)(void*);

					typedef struct 
					{
						DisplayCommandCodes_t		code;
						void						*params;
						RUNDISPLAYCOMMANDFUNC		funcPtr;
					} DisplayCommand_t;

					virtual void				IssueCommand(const DisplayCommand_t* dc);
					virtual DisplayCommand_t*	MakeDisplayCommand(const DisplayCommandCodes_t& code);

					DisplayCommand_t*			commandsDictionary[MDISPLAY_COMMANDS_COUNT];

					void						CreateCommandParams(DisplayCommand_t* dc);
					void						DeleteCommandParams(DisplayCommand_t* dc);
					virtual void				AttachCommandCallback(DisplayCommand_t* dc);

				public:

					typedef enum
					{
						MDEVICE_DISPLAY_MAIN,
						MDEVICE_DISPLAY_STEREO_LEFT,
						MDEVICE_DISPLAY_STEREO_RIGHT
					} DisplayDeviceSubType_t;

					static DisplayDevice*		main;

					DisplayDevice(const unsigned char subtype);
					virtual ~DisplayDevice();

					virtual void				PresentBuffer();

					void						ClearBuffer(int bits, Color clearColor = Color::transparent, float clearDepth = 1.0f, unsigned char clearStencil = 0);
					void						SetViewport(const Rect& rect);
					void						BindObject(DisplayObject* obj, int stage = 0);
					void						BindObjectNull(const int type, const int bindingPoint = 0, int stage = 0);
					DisplayObject*				GetBoundObject(const int type, const int bindingPoint = 0, int stage = 0) const;
					void						SetPipelineState(const PipelineState* state);
					void						SetInputLayout(VertexLayout* layout);

					void						SetTextureFiltering(Filtering filtering, bool force = false);
					void						SetTextureWrapping(WrapMode wrapMode, bool force = false);

					void						DrawIndexed(const int drawMode, const unsigned int count, const unsigned int verticesOffset = 0, const unsigned int indicesOffset = 0);
					void						DrawIndexed(const int drawMode, const unsigned int count, const Managed::MemoryPage::Block_t* vertices, const Managed::MemoryPage::Block_t* indices);

					virtual void				PostConnect(const DeviceContext_t& dc);

				protected:
					friend class DisplayObjects::ShaderProgram;
					void						UploadShaderUniformData(const DisplayObjects::ShaderProgram::UniformEntry_t* uniform, const void* data, const int numElems = 1);

				private:

					// cached values

					const	BlendState*			currentBlendState;
					const	RasterizerState*	currentRasterizerState;
					const	DepthState*			currentDepthState;
					const	StencilState*		currentStencilState;

					DISPCMDPARAM_CLEARBUFFER	clearBufferParam;
					DISPCMDPARAM_SETVIEWPORT	viewportParam;
					DISPCMDPARAM_SETINPUTLAYOUT	setInputLayoutParam;

					struct BoundObjectStages_t
					{
						DisplayObject*	stages[8];
						BoundObjectStages_t()
						{
							Reset();
						}

						void Reset()
						{
							for (int i = 0; i < 8; i++)
							{
								stages[i] = NULL;
							}
						}
					};

					struct BoundObjectBindingPoints_t
					{
						BoundObjectStages_t	bindingPoints[8];
						BoundObjectBindingPoints_t()
						{
							Reset();
						}
						void Reset()
						{
							for(int i = 0; i < 8; i++)
								bindingPoints[i].Reset();
						}
					};

					BoundObjectBindingPoints_t	boundDisplayObjects[DISPLAY_OBJECTS_TYPES_COUNT];
					void ResetBoundObjects();
				};
			}
		}
	}
}

#endif