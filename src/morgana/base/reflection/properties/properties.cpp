#include "properties.h"
#include "meobject.h"
#include "meio.h"
#include "mememory.h"
#include "objectproperties.h"

using namespace MorganaEngine::Base::Reflection;
using namespace MorganaEngine::Base::IO;

/// m_property

_propertyDef::_propertyDef()
{
	pOwner = NULL;
}

String& _propertyDef::GetName()
{
	ASSERT(pOwner != NULL, "Set owner first");
	return _object_properties::getData(pOwner, this).name;
}

String& _propertyDef::GetDisplayName()
{
	ASSERT(pOwner != NULL, "Set owner first");
	return _object_properties::getData(pOwner, this).displayName;
}

void _propertyDef::InvalidateValue()
{
	if (pOwner != NULL)
		pOwner->OnPropertySet(this);
}

bool _propertyDef::Equals(_propertyDef* other)
{
	String s1 = ToString();
	String s2 = other->ToString();
	return s1.Equals(s2);
}
