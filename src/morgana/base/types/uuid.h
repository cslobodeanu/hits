#ifndef __MORGANA_BASE_TYPES_UUID_H__
#define __MORGANA_BASE_TYPES_UUID_H__

#include "platform.h"

#ifdef OS_WINDOWS_WIN32
#include <Rpc.h>
#pragma  comment(lib, "Rpcrt4.lib")

#else
#include <uuid/uuid.h>
#endif

#include "mestring.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Types
		{
			class _UUID
			{
			public:

				static String New()
				{
#ifdef OS_WINDOWS_WIN32
					UUID uuid;
					UuidCreate(&uuid);

					unsigned char * str;
					UuidToStringA(&uuid, &str);

					String s((const char*)str);

					RpcStringFreeA(&str);
#else
					uuid_t uuid;
					uuid_generate_random(uuid);
					char s[37];
					uuid_unparse(uuid, s);
#endif
					return s;
				}
			};
		}
	}
}

#endif