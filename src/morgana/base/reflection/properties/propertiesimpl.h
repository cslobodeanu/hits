#ifndef __MORGANA_BASE_CLASSES_PROPERTIES_PROPERTIES_IMPL_H__
#define __MORGANA_BASE_CLASSES_PROPERTIES_PROPERTIES_IMPL_H__

#include "properties.h"
#include "meobject.h"
#include "metypes.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Reflection
		{

			inline Vector3 operator * (const _property<Quaternion> &q, const Vector3 &v)	{ return Quaternion::RotateVector(q, v); }
		}
	}
}

#endif