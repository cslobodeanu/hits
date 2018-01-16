#ifndef __MORGANA_FMK_RESOURCES_SHADERS_SHADER_BUILDER_H__
#define __MORGANA_FMK_RESOURCES_SHADERS_SHADER_BUILDER_H__

#include "../resourcefile.h"
#include "medisplay.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Resources
		{
			namespace Shaders
			{
				class ShaderBuilder
				{
				public:

					ShaderBuilder();
					virtual ~ShaderBuilder();

					virtual void	BeginProgram(const char* name);
					virtual void	BeginSourceBlock(const char* blockName);
					virtual void	AppendSource(const char* txt);
					virtual void	AppendSourceFromFile(const char* path);
					virtual void	AppendSourceFromStream(Stream* s);
					virtual void	EndBlock(const char* blockName);
					virtual String	EndProgram();
				protected:
					struct ProgramBlock_t
					{
						String name;
						String source;
					};

					int				GetBlockIndexWithName(const char* name);

					String	programName;
					int		currentOpenedBlock;
					String	completedSource;
								
					Array<ProgramBlock_t> programBlocks;
				};
			}
		}
	}
}

#endif