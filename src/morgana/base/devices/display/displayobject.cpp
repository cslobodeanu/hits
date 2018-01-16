#include "displayobject.h"
#include "displayobjectfactory.h"
#include "displaydevice.h"

using namespace MorganaEngine::Base::Devices::Display;

__implement_class(DisplayObject, MEObject);

#define INVALID_HANDLE		0xffffffff


DisplayObject::DisplayObject(const int type, const int bindingPoint)
{
	this->type = type;
	this->bindingPoint = bindingPoint;
	handle = INVALID_HANDLE;
	userData = NULL;

	createParams = NULL;

	display = NULL;

	lockedPtr = NULL;

	valid = true;
}

DisplayObject::DisplayObject()
{
	valid = true;
}

DisplayObject::~DisplayObject()
{
	if(display && display->GetBoundObject(type, bindingPoint) == this)
	{
		display->BindObjectNull(type, bindingPoint);
	}
	implementation<DisplayObjectFactory>()->Delete(this);

	DeleteCreateParams();
}

const unsigned long DisplayObject::GetHandle() const
{
	return handle;
}

const bool DisplayObject::IsValid() const
{
	return handle > 0 && handle != INVALID_HANDLE;
}

void DisplayObject::SetHandle(const unsigned long h)
{
	handle = h;
}

const int DisplayObject::GetDisplayObjectType() const
{
	return type;
}

void DisplayObject::SetUserData(void* ptr)
{
	userData = ptr;
}

const void* DisplayObject::GetUserData() const
{
	return userData;
}

const int DisplayObject::GetBindingPoint() const
{
	return bindingPoint;
}

void DisplayObject::SetCreateParams(void* params, const int size)
{
	DeleteCreateParams();
	createParams = new char[size];
	memcpy(createParams, params, size);
}

void DisplayObject::DeleteCreateParams()
{
	if(createParams)
	{
		char* ptr = (char*)createParams;
		delete [] ptr;
		createParams = NULL;
	}
}

void DisplayObject::GenerateHandleEventually()
{
	if(handle == INVALID_HANDLE)
		implementation<DisplayObjectFactory>()->GenerateHandle(this);
}

const bool DisplayObject::IsLocked()
{
	return lockedPtr != NULL;
}

void DisplayObject::SetLockedPtr(const void* ptr)
{
	lockedPtr = ptr;
}

const void* DisplayObject::GetLockedPtr()
{
	return lockedPtr;
}

const void* DisplayObject::Lock()
{
	return implementation<DisplayObjectFactory>()->Lock(this);
}

void DisplayObject::Unlock()
{
	implementation<DisplayObjectFactory>()->Unlock(this);
}

void DisplayObject::OnObjectBound(int bindingPoint, int stage)
{

}
