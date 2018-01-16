#ifndef __MORGANA_FRAMEWORK_RESOURCES_TEXT_DATA_H__
#define __MORGANA_FRAMEWORK_RESOURCES_TEXT_DATA_H__

#include "resourcefile.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Resources
		{
			class TextData : public ResourceFile
			{
				__declare_class(TextData, ResourceFile);

			public:

				TextData();

				String				text;

			protected:
				virtual void		ImportFromStreams(const StreamList&  streams, const char* args = NULL);
				virtual const bool	SaveToStream(Stream* s);
			};
		}
	}
}

#endif