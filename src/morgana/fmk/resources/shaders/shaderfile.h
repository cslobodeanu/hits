#ifndef __MORGANA_FMK_RESOURCES_SHADERS_SHADER_FILE_H__
#define __MORGANA_FMK_RESOURCES_SHADERS_SHADER_FILE_H__

#include "../resourcefile.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Resources
		{
			namespace Shaders
			{
				class ShaderFile : public ResourceFile
				{
					__declare_class(ShaderFile, ResourceFile);

				public:

					ShaderFile();

					String				m_sSource;

				protected:
					virtual void		ImportFromStreams(const StreamList&  streams, const char* args = NULL);
					virtual const bool	SaveToStream(Stream* s);
				};
			}
		}
	}
}

#endif