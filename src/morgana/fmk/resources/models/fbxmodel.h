#ifndef __MORGANA_FMK_RESOURCES_MODELS_FBXMODEL_H__
#define __MORGANA_FMK_RESOURCES_MODELS_FBXMODEL_H__

#include "../resourcefile.h"


namespace MorganaEngine
{
	namespace Framework
	{
		namespace Resources
		{
			namespace Models
			{
				class FBXModel : public ResourceFile
				{
					__declare_class(FBXModel, ResourceFile);
				public:

					FBXModel();
					virtual ~FBXModel();

				protected:
					virtual void		ImportFromStreams(const StreamList&  streams, const char* args = NULL);
					virtual const bool	SaveToStream(Stream* s);
				};
			}
		}
	}
}

#endif