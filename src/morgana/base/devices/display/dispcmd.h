#ifndef __MORGANA_BASE_DEVICES_DISPLAY_DISPCMD_H__
#define __MORGANA_BASE_DEVICES_DISPLAY_DISPCMD_H__

#include "vertexdef.h"
#include "rendepipelinedefs.h"
#include "displayobject.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Devices
		{
			namespace Display
			{
				typedef enum
				{
					MDISPLAY_COMMAND_BIND_OBJECT,
					MDISPLAY_COMMAND_CLEAR_BUFFER,
					MDISPLAY_COMMAND_SET_STATE,
					MDISPLAY_COMMAND_SET_SCISSOR,
					MDISPLAY_COMMAND_SET_VIEWPORT,
					MDISPLAY_COMMAND_COLOR_MASK,
					MDISPLAY_COMMAND_DRAW_INDEXED,
					MDISPLAY_COMMAND_DRAW_ARRAY,
					MDISPLAY_COMMAND_SET_INPUT_LAYOUT,
					MDISPLAY_COMMAND_UPLOAD_SHADER_UNIFORM_DATA,
					MDISPLAY_COMMAND_SET_TEXTURE_FILTERING,
					MDISPLAY_COMMAND_SET_TEXTURE_WRAPPING,
					MDISPLAY_COMMANDS_COUNT
				} DisplayCommandCodes_t;

				struct DISPCMDPARAM_COLORMASK
				{
					bool		red, green, blue, alpha;

					DISPCMDPARAM_COLORMASK()
					{
						red = green = blue = alpha = true;
					}
				};

				struct DISPCMDPARAM_SETVIEWPORT
				{
					Rect	rect;

					DISPCMDPARAM_SETVIEWPORT()
					{
						rect.Set(0, 0, 0, 0);
					}
				};

				struct DISPCMDPARAM_CLEARBUFFER
				{
					int							clear_bits;
					int							set_value_bits;
					Color						clear_color_value;
					float						clear_depth_value;
					unsigned char				clear_stencil_value;

					DISPCMDPARAM_CLEARBUFFER()
					{
						clear_bits = MDISPLAY_BUFFER_COLOR | MDISPLAY_BUFFER_DEPTH | MDISPLAY_BUFFER_STENCIL;
						set_value_bits = MDISPLAY_BUFFER_COLOR | MDISPLAY_BUFFER_DEPTH | MDISPLAY_BUFFER_STENCIL;
						clear_color_value = Color::green;
						clear_depth_value = 0.0f;
						clear_stencil_value = 0;
					}
				};

				struct DISPCMDPARAM_BINDOBJECT
				{
					DisplayObject*			object;
					int						stage;
					int						type, bindingPoint;
					DISPCMDPARAM_BINDOBJECT()
					{
						stage = 0;
						object = NULL;
						bindingPoint = 0;
						type = DISPLAY_OBJECT_UNDEFINED;
					}

					inline const int GetType() const { return object ? object->GetDisplayObjectType() : type; }
					inline const int GetBindingPoint() const { return object ? object->GetBindingPoint() : bindingPoint; }
					inline const unsigned int GetHandle() const { return object ? object->GetHandle() : 0; }
				};

				class PipelineState;
				struct DISPCMDPARAM_SETPIPELINESTATE
				{
					const PipelineState*	state;
					DISPCMDPARAM_SETPIPELINESTATE()
					{
						state = NULL;
					}
				};

				class VertexDefinition;
				struct DISPCMDPARAM_SETINPUTLAYOUT
				{
					VertexLayout* vertexLayout;

					DISPCMDPARAM_SETINPUTLAYOUT()
					{
						vertexLayout = NULL;
					}
				};

				struct DISPCMDPARAM_UPLOADSHADERUNIFORMDATA
				{
					const void*	uniform;
					const void* data;
					int			numElements;
				};

				struct DISPCMDPARAM_DRAWINDEXED
				{
					int drawMode;
					unsigned int vertexOffset, indexOffset;
					unsigned int elementsCount;
				};

				struct DISPCMDPARAM_SET_TEXTURE_FILTERING
				{
					Filtering filtering;
				};

				struct DISPCMDPARAM_SET_TEXTURE_WRAPPING
				{
					WrapMode wrapMode;
				};
			}
		}
	}
}

#endif