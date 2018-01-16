#include "buffer.h"
#include "../displayobjectfactory.h"

using namespace MorganaEngine::Base::Devices::Display;
using namespace MorganaEngine::Base::Devices::Display::DisplayObjects;


__implement_class(Buffer, DisplayObject);

Buffer::Buffer() 
{

}

Buffer::Buffer(const int bindingPoint) : DisplayObject(DISPLAY_OBJECT_BUFFER, bindingPoint)
{

}

void Buffer::Create(const int size, const int access)
{
	CreateParams_t p(size, access);
	implementation<DisplayObjectFactory>()->Create(this, &p);
}
