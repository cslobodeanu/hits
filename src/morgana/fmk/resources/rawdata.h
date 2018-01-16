#ifndef __MORGANA_FRAMEWORK_RESOURCES_RAW_DATA_H__
#define __MORGANA_FRAMEWORK_RESOURCES_RAW_DATA_H__

#include "resourcefile.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Resources
		{
			class RawData : public ResourceFile
			{
				__declare_class(RawData, ResourceFile);

			public:

				RawData();

				Array<byte>			bytes;

			protected:
				virtual void		ImportFromStreams(const StreamList&  streams, const char* args = NULL);
				virtual const bool	SaveToStream(Stream* s);
			};
		}
	}
}

#endif