#ifndef __MORGANA_BASE_REFLECTION_SERIALIZER_SERIALIZER_BASE_H__
#define __MORGANA_BASE_REFLECTION_SERIALIZER_SERIALIZER_BASE_H__

#include "../../io/stream.h"

namespace MorganaEngine
{
	namespace Base
	{
		class IO::Stream;
		namespace Reflection
		{
			class MEObject;
			namespace Serializer
			{
				class SerializerBase
				{
					SerializerBase();
				public:

					virtual ~SerializerBase();

					virtual bool		Serialize(const MEObject* obj, IO::Stream* data_out);
					virtual MEObject*	Deserialize(IO::Stream* data_in, MEObject* obj = NULL);
				};
			}
		}
	}
}

#endif