#include "meobject.h"
#include "properties/properties.h"
#include "properties/objectproperties.h"
#include "meio.h"
#include "mememory.h"
#include "metypes.h"

using namespace MorganaEngine::Base::Reflection;
using namespace MorganaEngine::Base::IO;

__implement_class_abstract(MEObject, NULL);

Action<MEObject*>	MEObject::OnObjectDestroy;

MEObject::MEObject()
{
	_Reflection.AddInstance(this);
	uid = _UUID::New();
}

MEObject::~MEObject()
{
	OnObjectDestroy(this);
	_Reflection.RemoveInstance(this);
	//DeleteCategories();
}

MEObject* MEObject::Clone()
{
    MEObject* clone = Instantiate(GetClassDesc());
	//clone->copy(this);
    return clone;
}


void MEObject::DoAddProperties()
{
}

bool MEObject::Equals(MEObject* other)
{
	if(GetClassDesc()->classID != other->GetClassDesc()->classID) return false;

	for (int i = 0; i < GetClassDesc()->GetPropertiesCount(); i++)
	{
		_propertyDef* p = _object_properties::propertyAt(this, i);
		if (p->Equals(_object_properties::propertyAt(other, i)) == false) return false;
	}

	return true;
}

void* MEObject::DynamicCast(CLASSDESC cd) const
{
	if(this->IsKindOf(cd) && GetClassDesc()->numBaseClasses == 1) return (void*)this;
	return NULL;
}

String MEObject::GetDescription()
{
	return GetClassDesc()->className;
}

MEObject* MEObject::Instantiate(const char* className)
{
	CLASSDESC classInfo = _Reflection.GetClassDesc(className);
	return Instantiate(classInfo);
}

MEObject* MEObject::Instantiate(CLASSDESC type)
{
	if (type == NULL) return NULL;

	return (MEObject*)_Reflection.Instantiate(type);
}
