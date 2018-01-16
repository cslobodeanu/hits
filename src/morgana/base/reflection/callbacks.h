#ifndef __MORGANA_BASE_REFLECTION_CALLBACKS_H__
#define __MORGANA_BASE_REFLECTION_CALLBACKS_H__

#include "../types/mearray.h"

namespace MorganaEngine
{
	namespace Base
	{
		using namespace Types;

		namespace Reflection
		{
			class EmptyType
			{
			public:
				EmptyType() {}
				virtual ~EmptyType() {};
			};

			class MEObject;

			template <class T> class CallbackFunc
			{
			public:

				typedef EmptyType*					CallerT;

				T									func;
				CallerT								caller;

				CallbackFunc<T>()
				{
					caller = NULL;
					func = NULL;
				}
				
				CallbackFunc<T>(CallerT caller, T func)
				{
					this->caller = caller;
					this->func = func;
				}

				CallbackFunc<T>(const CallbackFunc<T>& other)
				{
					func = other.func;
					caller = other.caller;
				}

				inline bool operator == (const CallbackFunc& c) const
				{
					return c.func == func && c.caller == caller;
				}
			};

			template <typename ...Args>
			class Action
			{
			public:

				typedef void(EmptyType::*FuncPtr)(Args...);
				typedef void(*FuncStaticPtr)(Args...);
				
				inline Action()
				{
				}

				inline Action(const Action& other)
				{
					callbacks.Copy(other.callbacks);
					callbacksStatic.Copy(other.callbacksStatic);
				}

				inline ~Action()
				{
				}

				inline void operator += (CallbackFunc<FuncPtr> cb)
				{
					callbacks.Add(cb);
				}

				inline void operator -= (CallbackFunc<FuncPtr> cb)
				{
					callbacks.Remove(cb);
				}

				inline void operator += (CallbackFunc<FuncStaticPtr> cb)
				{
					callbacksStatic.Add(cb);
				}

				inline void operator -= (CallbackFunc<FuncStaticPtr> cb)
				{
					callbacksStatic.Remove(cb);
				}

				bool operator  != (const void* p)
				{
					return callbacks.ptr() != p;
				}

				bool operator == (const void* p)
				{
					return callbacks.ptr() == p;
				}

				void operator ()(Args... args)
				{
					for (callback_t* ptr = callbacks.ptr(); ptr < callbacks.end(); ptr++)
					{
						if (ptr->caller)
							(ptr->caller->*ptr->func)(args...);
					}

					for (callbackStatic_t* ptr = callbacksStatic.ptr(); ptr < callbacksStatic.end(); ptr++)
					{
						(*ptr->func)(args...);
					}
				}
			protected:
				typedef CallbackFunc<FuncPtr>		callback_t;
				typedef CallbackFunc<FuncStaticPtr> callbackStatic_t;
				Array<callback_t>					callbacks;
				Array<callbackStatic_t>				callbacksStatic;
			};
		}
	}
}

#define CallbackT(T, callbackFunc, ...)		MorganaEngine::Base::Reflection::CallbackFunc<MorganaEngine::Base::Reflection::Action<__VA_ARGS__>::FuncPtr>(this, (MorganaEngine::Base::Reflection::Action<__VA_ARGS__>::FuncPtr)&T::callbackFunc)
#define Callback(callbackFunc, ...)			MorganaEngine::Base::Reflection::CallbackFunc<MorganaEngine::Base::Reflection::Action<__VA_ARGS__>::FuncPtr>(this, (MorganaEngine::Base::Reflection::Action<__VA_ARGS__>::FuncPtr)&Myself::callbackFunc)
#define CallbackStatic(callbackFunc, ...)	MorganaEngine::Base::Reflection::CallbackFunc<MorganaEngine::Base::Reflection::Action<__VA_ARGS__>::FuncStaticPtr>(NULL, (MorganaEngine::Base::Reflection::Action<__VA_ARGS__>::FuncStaticPtr)&callbackFunc)

#endif