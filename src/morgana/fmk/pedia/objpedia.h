#ifndef __MORGANA_FMK_PEDIA_OBJPEDIA_H__
#define __MORGANA_FMK_PEDIA_OBJPEDIA_H__

#include "metypes.h"
#include "reflection.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Pedia
		{
			class ObjectPedia
			{
				Dictionary<String, MEObject*> dictionary;
			};
		}
	}
}

#endif