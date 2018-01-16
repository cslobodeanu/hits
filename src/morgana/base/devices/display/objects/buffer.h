#ifndef __MORGANA_BASE_DEVICES_DISPLAY_OBJECTS_BUFFER_H__
#define __MORGANA_BASE_DEVICES_DISPLAY_OBJECTS_BUFFER_H__

#include "../displayobject.h"
#include "../rendepipelinedefs.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Devices
		{
			namespace Display
			{
				namespace DisplayObjects
				{
					class Buffer : public DisplayObject
					{
						__declare_class(Buffer, DisplayObject);
						Buffer();
					public:
						Buffer(const int bindingPoint);

						struct CreateParams_t
						{
							const int		size;
							const int		accessMode;
							CreateParams_t(const int _size, const int access) : size(_size), accessMode(access){}
						};

						void Create(const int size, const int access);
					};
				}
			}
		}
	}
}

#endif