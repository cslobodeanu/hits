#include "meclasslib.h"
#include "meobject.h"
#include "properties/propertiesimpl.h"
#include "mememory.h"
#include "../io/debug.h"
#include "properties/objectproperties.h"

using namespace MorganaEngine::Base::Reflection;


_CLASSDESC::~_CLASSDESC()
{
	for(int i = 0; i < propertiesDesc.Length(); i++)
	{
		object_property_data* cpd = propertiesDesc[i];
		SAFE_DEL(cpd);
	}
}

const bool _CLASSDESC::IsKindOf(const _CLASSDESC* t) const
{
	return _Reflection.IsKindOf(this, t);
}

MEClassLibrary::MEClassLibrary()
{
    //DEBUG_OUT("ME class lib inited.\n");
}

MEClassLibrary::~MEClassLibrary()
{
    Cleanup();
}

void MEClassLibrary::Cleanup()
{
    for(int i = 0; i < registeredClasses.Length(); i++)
    {
        CLASSDESC cd = registeredClasses[i];

		if (cd->instances.Length())
		{
			ASSERT("instances left");
		}

		delete cd;
        //SAFE_DEL(cd);
    }
    registeredClasses.Clear();
}

void MEClassLibrary::SetClassInheritanceCache(CLASSDESC cd, CLASSDESC base)
{
	if(cd->classID < 0) return;
	if(!base || base->classID < 0) return;

	if(cd->classID < maxClassesCount)
	{
		classInheritanceTable[cd->classID].SetBit(cd->classID);

		if(base->classID < maxClassesCount)
			classInheritanceTable[cd->classID].SetBit(base->classID);

		for(int i = 0; i < base->numBaseClasses; i++)
		{
			const int bid = base->baseClassesID[i];
			if(bid < 0) continue;
			SetClassInheritanceCache(cd, GetClassDesc(bid));
		}
	}
}

_CLASSDESC* MEClassLibrary::RegisterClass(char* classname, ConstructorFunction constructor, char* baseclass0, char* baseclass1, char* baseclass2, char* baseclass3, char* baseclass4, char* baseclass5, char* baseclass6, bool bAbstract, bool bIsHidden)
{
    _CLASSDESC* cd = NEW _CLASSDESC();

    cd->className = classname;
    cd->description = "No description";
    cd->constructor = constructor;
    cd->constructorCalls = 0;
    cd->classID = registeredClasses.Length();
    cd->isAbstract = bAbstract;
	cd->isHidden = bIsHidden;
	cd->numBaseClasses = 0;

	Array<String> baseclasses;
	baseclasses.Add(String(baseclass0));
	baseclasses.Add(String(baseclass1));
	baseclasses.Add(String(baseclass2));
	baseclasses.Add(String(baseclass3));
	baseclasses.Add(String(baseclass4));
	baseclasses.Add(String(baseclass5));
	baseclasses.Add(String(baseclass6));

	String null_base("NULL");

	for(int i = 0; i < baseclasses.Length(); i++)
	{
		if(null_base.Equals(baseclasses[i]) == false && baseclasses[i].Length() > 0)
		{
			cd->baseClassesNames[cd->numBaseClasses] = baseclasses[i];
			const _CLASSDESC* base = GetClassDesc(baseclasses[i]);
			if(base) 
			{
				cd->baseClassesID[cd->numBaseClasses] = base->classID;
			}
			else
				cd->baseClassesID[cd->numBaseClasses] = -1;

			cd->numBaseClasses++;
		}
	}

    for(int i = 0; i < registeredClasses.Length(); i++)
    {
		for(int k = 0; k < registeredClasses[i]->numBaseClasses; k++)
		{
			if(registeredClasses[i]->baseClassesID[k] >= 0) continue;

			if(registeredClasses[i]->baseClassesNames[k].Equals(cd->className))
			{
				registeredClasses[i]->baseClassesID[k] = cd->classID;
			}
		}
    }

	registeredClasses.Add(cd);

	for(int i = 0; i < registeredClasses.Length(); i++)
    {
		for(int k = 0; k < registeredClasses[i]->numBaseClasses; k++)
		{
			SetClassInheritanceCache(registeredClasses[i], GetClassDesc(registeredClasses[i]->baseClassesID[k]));
		}
    }
    
    return cd;
}

void* MEClassLibrary::Instantiate(const char* classname)
{
    const _CLASSDESC* cd = GetClassDesc(classname);
    if(cd == NULL) return NULL;
    ((_CLASSDESC*)cd)->constructorCalls++;
    return cd->constructor();
}

void* MEClassLibrary::Instantiate(CLASSDESC cd)
{
    if(cd == NULL) return NULL;

    String str;
	str.Printf("Class \"%s\" has no default constructor.", cd->className);
	ASSERT((cd->constructor != NULL), "%s", str);
    ((_CLASSDESC*)cd)->constructorCalls++;
    return cd->constructor();
}

const _CLASSDESC* MEClassLibrary::GetClassDesc(const char* classname) const 
{
	const int hash = String::ComputeHashCode(classname);
    for(int i = 0; i < registeredClasses.Length(); i++)
	{
		const StringHash& cs = registeredClasses[i]->className;
        if(cs.Equals(classname, hash)) return registeredClasses[i];
	}
    return NULL;
}

const _CLASSDESC* MEClassLibrary::GetClassDesc(const long& classid) const 
{
    if(classid < 0) return NULL;
    if(classid >= registeredClasses.Length()) return NULL;
    return registeredClasses[classid];
}

const bool MEClassLibrary::IsKindOf(const _CLASSDESC* cd, const _CLASSDESC* base) const 
{
	if(base == NULL) return false;

	if(cd->classID < maxClassesCount)
	{
		return classInheritanceTable[cd->classID].IsBitSet(base->classID);
	}
	else
	{
		if(cd->classID == base->classID) return true;

		bool ret = false;

		const int len = cd->numBaseClasses;
		if(len == 1)
		{
			const long cdi = cd->baseClassesID[0];
			if(cdi >= 0)
			{
				_CLASSDESC* c = registeredClasses[cdi];
				if(c != NULL && IsKindOf(c, base))
				{
					ret = true;
				}
			}
		}
		else
		{
			for (long *ptr = (long*)cd->baseClassesID, *end = ptr + len; ptr < end; ptr++)
			{
				if((*ptr) < 0) continue;
				_CLASSDESC* c = registeredClasses[*ptr];
				if(c == NULL) continue;
				if(IsKindOf(c, base)) { ret = true; break; }
			}
		}

		return ret;
	}
}

Array<CLASSDESC> MEClassLibrary::GetClassesOfType(const char* baseclass)
{
    return GetClassesOfType(GetClassDesc(baseclass));
}

Array<CLASSDESC> MEClassLibrary::GetClassesOfType(const _CLASSDESC* baseclass)
{
	Array<CLASSDESC> result;
	for (int i = 0; i < registeredClasses.Length(); i++)
    {
        if(registeredClasses[i]->isAbstract == true) continue;
        if(registeredClasses[i]->SameAs(*baseclass)) continue; // skip the base class
		if (IsKindOf(registeredClasses[i], baseclass)) result.Add(registeredClasses[i]);
    }

	return result;
}

void MEClassLibrary::GetClassInstances(CLASSDESC type, Array<void*>& list)
{
	list.Copy(type->instances);
}

void MEClassLibrary::AddInstance(void* object)
{
	MEObject* o = (MEObject*)object;
	_CLASSDESC* t = (_CLASSDESC*)o->GetClassDesc();
	if (t != NULL)
		t->instances.Add(object);
}

void MEClassLibrary::RemoveInstance(void* object)
{
	MEObject* o = (MEObject*)object;
	_CLASSDESC* t = (_CLASSDESC*)o->GetClassDesc();
	t->instances.Remove(object);
}

void* MEClassLibrary::GetClassInstance(CLASSDESC type)
{
	if (type->instances.Length() > 0)
		return type->instances.First();

	return NULL;
}
