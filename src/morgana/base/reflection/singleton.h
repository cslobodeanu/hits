#ifndef __MORGANA_BASE_REFLECTION_SINGLETON_H__
#define __MORGANA_BASE_REFLECTION_SINGLETON_H__

template<class T>
class singleton
{
	static T*	pInstance;
	static T*	SafeGet()
	{
		if (pInstance == NULL)
			pInstance = new T();
		return pInstance;
	}
public:

	singleton()
	{
		SafeGet();
	}

	inline T* operator -> () const
	{
		return SafeGet();
	}

	inline operator T* () const
	{
		return SafeGet();
	}
};

template<class T> T* singleton<T>::pInstance = NULL;

template<class T>
class implementation
{
	static T*	pImplementation;
	static T*	SafeGet()
	{
		if (pImplementation == NULL)
		{
			Array<CLASSDESC> classes = singleton<MEClassLibrary>()->GetClassesOfType<T>();
			if (classes.Length() == 0) return NULL;
				pImplementation = (T*)MEObject::Instantiate(classes[0]);
			return pImplementation;
		}
		return pImplementation;
	}
public:

	inline T* operator -> () const
	{
		return SafeGet();
	}

	inline operator T* () const
	{
		return SafeGet();
	}
};

template<class T> T* implementation<T>::pImplementation = NULL;

#endif