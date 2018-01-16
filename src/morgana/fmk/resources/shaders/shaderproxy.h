#ifndef __MORGANA_FMK_RESOURCES_SHADERS_SHADER_PROXY_H__
#define __MORGANA_FMK_RESOURCES_SHADERS_SHADER_PROXY_H__

#include "medisplay.h"
#include "metypes.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Resources
		{
			namespace Shaders
			{
				class ShaderProxy
				{
				public:
					ShaderProxy();
					virtual ~ShaderProxy();

					ShaderProgram* shader;

					const int	UseDefinition(const char* defName);
					const void	UseDefinition(const int handle);

				private:

					struct  ProxyStruct_t
					{
						StringHash			define;
						ShaderProgram*		shader;
					};

					Array<ProxyStruct_t>	variants;
					const int				GetVariant(const char* defName);
					String					CreateArgs(const char* defName);
				};
			}
		}
	}
}

#endif