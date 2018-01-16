#ifndef __MORGANA_BASE_REFLECTION_CLASSLIB_H__
#define __MORGANA_BASE_REFLECTION_CLASSLIB_H__


#include "../types/mestringhash.h"
#include "../types/mearray.h"
#include "../types/bits.h"
#include "classes_macros.h"
#include "singleton.h"

namespace MorganaEngine
{
	namespace Base
	{
		using namespace Types;
		namespace Reflection
		{
			typedef void* (*ConstructorFunction)();

			struct object_property_data;

			/** structure for defining a class **/
			struct _CLASSDESC
			{			
				Array<void*>						instances;

				StringHash							className;				// the name
				long								classID;				// the id (is unique)

				long								baseClassesID[16];		// max 16 base classes
				String								baseClassesNames[16];	// max 16 base classes
				char								numBaseClasses;		// base classes count

				String								description;			// description of the class

				ConstructorFunction					constructor;			// constructor function (0 params)
				long								constructorCalls;		// how many times was called

				bool								isAbstract;			// class is abstract
				bool								isHidden;				// class is hidden (cannot be seen in editor for example)

				const bool							SameAs(const _CLASSDESC& cd) const
				{
					return classID == cd.classID;
				}

				const bool							IsKindOf(const _CLASSDESC* t) const;

				Array<object_property_data*>		propertiesDesc;			// list of properties

				bool								propertiesRegistered;

				_CLASSDESC()
				{
					propertiesRegistered = false;
				}
				~_CLASSDESC();

				inline const int GetPropertiesCount() const
				{
					return propertiesDesc.Length();
				}
			};

			typedef const _CLASSDESC		*CLASSDESC;

			/** singleton for keeping all classes registered **/
			class MEClassLibrary
			{
				friend class MEObject;

				Array<_CLASSDESC*>			registeredClasses;

				static const int			maxClassesCount = 1024;

				/**
					class inheritance kept as a matrix of bits, to have an IsKindOf fast
					every line means a class, every bit from the line means if its derived from that one
				**/
				Bits<maxClassesCount>		classInheritanceTable[maxClassesCount];
				void						SetClassInheritanceCache(CLASSDESC cd, CLASSDESC base);

			public:
				MEClassLibrary();
				~MEClassLibrary();

				void				Cleanup();

				/** register a new class **/
				_CLASSDESC*			RegisterClass(char* classname, ConstructorFunction constructor, char* baseclass0, char* baseclass1 = NULL, char* baseclass2 = NULL, char* baseclass3 = NULL, char* baseclass4 = NULL, char* baseclass5 = NULL, char* baseclass6 = NULL, bool bAbstract = false, bool bIsHidden = false);				


				CLASSDESC			GetClassDesc(const char* classname) const;
				CLASSDESC			GetClassDesc(const long& classid) const;

				/** check class inheritance **/
				const bool			IsKindOf(CLASSDESC cd, CLASSDESC base) const;

				/** find all classes derived from a baseclass **/
				Array<CLASSDESC>	GetClassesOfType(const char* baseclass);
				Array<CLASSDESC>	GetClassesOfType(CLASSDESC baseclass);
				template<class T>
				Array<CLASSDESC>	GetClassesOfType();

				/** gets all instances of a class, if enabled **/
				void				GetClassInstances(CLASSDESC type, MorganaEngine::Base::Types::Array<void*>& list);
				void*				GetClassInstance(CLASSDESC type);
				template<class T>
				T*					GetClassInstance();

			protected:
				void				AddInstance(void* object);
				void				RemoveInstance(void* object);

				/** invoke constructor of a class, based on Type or name **/
				void*               Instantiate(const char* classname);
				void*               Instantiate(CLASSDESC cd);
				template<class T>
				T*					Instantiate();
			};

			template<class T>
			Array<CLASSDESC> MEClassLibrary::GetClassesOfType()
			{
				return GetClassesOfType(T::AsClassDesc);
			}

			template<class T>
			T* MEClassLibrary::GetClassInstance()
			{
				return (T*)GetClassInstance(T::);
			}

			template<class T>
			T* MEClassLibrary::Instantiate()
			{
				return (T*)Instantiate(T::AsClassDesc);
			}

		}
    }
}

/**
	macros for registering your classes to be found with classlibrary.
	for abstract classes use in your header file DECLARE_CLASS_ABSTRACT
	for a normal class, use DECLARE_CLASS, it implements also the constructor call.

	In the cpp file, you put IMPLEMENT_CLASS accordingly.

	For multiple base classes, use DECLARE_INTERFACES macro, because it also
	implements a dynamic cast for safe casting to base classes (__dynamic_cast).


	Number of base classes should be present at the end of the macro name.
**/


#define __declare_class_abstract1(classname) \
    public: \
		static const MorganaEngine::Base::Reflection::_CLASSDESC*	AsClassDesc; \
		virtual const MorganaEngine::Base::Reflection::_CLASSDESC*	GetClassDesc() const \
        { \
            return classname::AsClassDesc; \
        } \
        virtual const bool IsKindOf(char* basename) const \
        { \
			const MorganaEngine::Base::Reflection::_CLASSDESC* base = _Reflection.GetClassDesc(basename); \
            return IsKindOf(base); \
        } \
		virtual const bool IsKindOf(const MorganaEngine::Base::Reflection::_CLASSDESC* base) const\
		{ \
			return _Reflection.IsKindOf(GetClassDesc(), base); \
		} \
		private: \
		typedef classname Myself; \

#define __declare_class_abstract(classname, baseclass) \
    public: \
		static const MorganaEngine::Base::Reflection::_CLASSDESC*	AsClassDesc; \
        virtual const MorganaEngine::Base::Reflection::_CLASSDESC*	GetClassDesc() const \
        { \
            return classname::AsClassDesc; \
        } \
        virtual const bool IsKindOf(char* basename) const \
        { \
			const MorganaEngine::Base::Reflection::_CLASSDESC* base = _Reflection.GetClassDesc(basename); \
            return IsKindOf(base); \
        } \
		virtual const bool IsKindOf(const MorganaEngine::Base::Reflection::_CLASSDESC* base) const \
        { \
            return _Reflection.IsKindOf(GetClassDesc(), base); \
        } \
	private: \
		typedef baseclass super;\
		typedef classname Myself;

#define __declare_class(classname, baseclass) \
    __declare_class_abstract(classname, baseclass) \
    public: \
        static void* InstantiateClass() \
        { \
            return NEW classname(); \
        } \
    private:

#define __declare_class1(classname) \
    __declare_class_abstract1(classname) \
    public: \
        static void* InstantiateClass() \
        { \
            return NEW classname(); \
        } \
    private:

#define __implement_class(classname, baseclass) \
	const MorganaEngine::Base::Reflection::_CLASSDESC* classname::AsClassDesc = _Reflection.RegisterClass(#classname, classname::InstantiateClass, #baseclass);

#define __implement_class2(classname, baseclass0, baseclass1) \
	const MorganaEngine::Base::Reflection::_CLASSDESC* classname::AsClassDesc = _Reflection.RegisterClass(#classname, classname::InstantiateClass, #baseclass0, #baseclass1);

#define __implement_class2_hidden(classname, baseclass0, baseclass1) \
	const MorganaEngine::Base::Reflection::_CLASSDESC* classname::AsClassDesc = _Reflection.RegisterClass(#classname, classname::InstantiateClass, #baseclass0, #baseclass1, NULL, NULL, NULL, NULL, NULL, false, true);

#define __implement_class3(classname, baseclass0, baseclass1, baseclass2) \
	const MorganaEngine::Base::Reflection::_CLASSDESC* classname::AsClassDesc = _Reflection.RegisterClass(#classname, classname::InstantiateClass, #baseclass0, #baseclass1, #baseclass2);

#define __implement_class4(classname, baseclass0, baseclass1, baseclass2, baseclass3) \
	const MorganaEngine::Base::Reflection::_CLASSDESC* classname::AsClassDesc = _Reflection.RegisterClass(#classname, classname::InstantiateClass, #baseclass0, #baseclass1, #baseclass2, #baseclass3);

#define __implement_class_hidden(classname, baseclass) \
	const MorganaEngine::Base::Reflection::_CLASSDESC* classname::AsClassDesc = _Reflection.RegisterClass(#classname, classname::InstantiateClass, #baseclass, NULL, NULL, NULL, NULL, NULL, NULL, false, true);

#define __implement_class_abstract(classname, baseclass) \
    const MorganaEngine::Base::Reflection::_CLASSDESC* classname::AsClassDesc = _Reflection.RegisterClass(#classname, NULL, #baseclass, NULL, NULL, NULL, NULL, NULL, NULL, true);

#define __implement_class_abstract1(classname) \
    const MorganaEngine::Base::Reflection::_CLASSDESC* classname::AsClassDesc = _Reflection.RegisterClass(#classname, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, true);

#define __declare_interfaces1(class1) \
	public: \
	virtual void* DynamicCast(const MorganaEngine::Base::Reflection::_CLASSDESC* cd) const \
	{ \
		if(cd == class1::AsClassDesc) \
			return (void*)((class1*)this); \
		return IsKindOf(cd) ? (void*)((class1*)this) : NULL; \
	} \
	private:

#define __declare_interfaces2(class1, class2) \
	public: \
	virtual void* DynamicCast(const MorganaEngine::Base::Reflection::_CLASSDESC* cd) const \
	{ \
		if(cd == class1::AsClassDesc) \
			return (class1*)this; \
		if(cd == class2::AsClassDesc) \
			return (class2*)this; \
		return IsKindOf(cd) ? this : NULL; \
	} \
	private:

#define __declare_interfaces3(class1, class2, class3) \
	public: \
	virtual void* DynamicCast(const MorganaEngine::Base::Reflection::_CLASSDESC* cd) const \
	{ \
		if(cd == class1::AsClassDesc) \
			return (class1*)this; \
		if(cd == class2::AsClassDesc) \
			return (class2*)this; \
		if(cd == class3::AsClassDesc) \
			return (class3*)this; \
		return IsKindOf(cd) ? this : NULL; \
	} \
	private:

#define __declare_interfaces4(class1, class2, class3, class4) \
	public: \
	virtual void* DynamicCast(const MorganaEngine::Base::Reflection::_CLASSDESC* cd) const \
	{ \
		if(cd == class1::AsClassDesc) \
			return (class1*)this; \
		if(cd == class2::AsClassDesc) \
			return (class2*)this; \
		if(cd == class3::AsClassDesc) \
			return (class3*)this; \
		if(cd == class4::AsClassDesc) \
			return (class4*)this; \
		return IsKindOf(cd) ? this : NULL; \
	} \
	private:

#define __declare_interfaces5(class1, class2, class3, class4, class5) \
	public: \
	virtual void* DynamicCast(const MorganaEngine::Base::Reflection::_CLASSDESC* cd) const \
	{ \
		if(cd == class1::AsClassDesc) \
			return (class1*)this; \
		if(cd == class2::AsClassDesc) \
			return (class2*)this; \
		if(cd == class3::AsClassDesc) \
			return (class3*)this; \
		if(cd == class4::AsClassDesc) \
			return (class4*)this; \
		if(cd == class5::AsClassDesc) \
			return (class5*)this; \
		return IsKindOf(cd) ? this : NULL; \
	} \
	private:


#define _Reflection (*singleton<MEClassLibrary>())

#endif




