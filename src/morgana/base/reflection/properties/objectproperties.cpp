#include "objectproperties.h"

using namespace MorganaEngine::Base::Reflection;

int _object_properties::getIndex(MEObject* owner, _propertyDef* p)
{
	for (int i = 0; i < owner->GetClassDesc()->GetPropertiesCount(); i++)
	{
		if (p == propertyAt(owner, i)) return i;
	}

	return -1;
}

object_property_data& _object_properties::getData(MEObject* owner, _propertyDef* p)
{
	const int index = getIndex(owner, p);
	ASSERT(index >= 0, "PropertyDesc not found");
	return *(owner->GetClassDesc()->propertiesDesc[index]);
}

_propertyDef* _object_properties::propertyAt(MEObject* owner, const int& index)
{
	const unsigned long addr = (unsigned long)owner + owner->GetClassDesc()->propertiesDesc[index]->locationOffset;
	void* ptr = (void*)addr;
	return (_propertyDef*)ptr;
}

long _object_properties::getLocationOffset(MEObject* owner, _propertyDef* p)
{
	return (unsigned long)p - (unsigned long)owner;
}

bool _object_properties::locationExists(MEObject* owner, _propertyDef* p)
{
	const long addr = getLocationOffset(owner, p);
	const CLASSDESC cd = owner->GetClassDesc();
	for (int i = 0; i < cd->propertiesDesc.Length(); i++)
	{
		const unsigned long addr2 = cd->propertiesDesc[i]->locationOffset;
		if (addr2 == addr)
			return true;
	}
	return false;
}

object_property_data& _object_properties::add(MEObject* owner, const char* dispname, const char* varname, _propertyDef* var)
{
	var->SetOwner(owner);

	_CLASSDESC* cd = (_CLASSDESC*)owner->GetClassDesc();
	if (cd->propertiesRegistered)
	{
		return getData(owner, var);
	}

	if (locationExists(owner, var))
	{
		cd->propertiesRegistered = true;
		return getData(owner, var);
	}

	object_property_data* cpd = new object_property_data();
	cpd->name = varname;
	cpd->displayName = dispname;
	cpd->locationOffset = getLocationOffset(owner, var);

	cd->propertiesDesc.Add(cpd);

	return *cpd;
}

object_property_data& _object_properties::add1(MEObject* owner, const char* varname, _propertyDef* var)
{
	return add(owner, UniqueNameGenerator::ConvertVarNameToDisplayName(varname), varname, var);
}

void _object_properties::copy(MEObject* owner, MEObject* mrs)
{
	long clsid1, clsid2;
	clsid1 = owner->GetClassDesc()->classID;
	clsid2 = mrs->GetClassDesc()->classID;

	ASSERT(owner->GetClassDesc()->classID == mrs->GetClassDesc()->classID, "Not the same class.");

	for (int i = 0; i < owner->GetClassDesc()->GetPropertiesCount(); i++)
	{
		_propertyDef* dest_atr = propertyAt(owner, i);
		_propertyDef* source_atr = propertyAt(mrs, i);

		if (!source_atr->GetName().Equals(dest_atr->GetName()))
		{
			source_atr = NULL;
			for (int j = 0; j < mrs->GetClassDesc()->GetPropertiesCount(); j++)
			{
				if (source_atr->GetName().Equals(dest_atr->GetName()))
				{
					source_atr = propertyAt(mrs, j);
					break;
				}
			}
		}

		if (source_atr == NULL) continue;

		static String tmp = source_atr->ToString();
		dest_atr->FromString(tmp);
	}
}

_propertyDef* _object_properties::find(MEObject* owner, const char* name)
{
	for (int i = 0; i < owner->GetClassDesc()->GetPropertiesCount(); i++)
	{
		_propertyDef* p = propertyAt(owner, i);
		if (p->GetName().Equals(name))
			return p;
	}
	return NULL;
}
