#ifndef __MORGANA_FMK_TWEEN_METWEEN_H__
#define __MORGANA_FMK_TWEEN_METWEEN_H__

#include "base.h"
#include "gameobjects.h"
#include "../application/meapplication.h"

namespace MorganaEngine
{
	namespace Framework
	{
		using namespace Application;
		namespace Tween
		{
			class METween
			{
			public:
				enum EaseTypes_t
				{
					Linear = 0
				};

				class TweenAction;

			protected:
				friend class MEApplication;

				static void Initialize();
				static void Destroy();
				static void UpdateAll();

				typedef float(*EaseFuncPtr)(const float& progress);
				typedef void(*UpdateFuncPtr)(TweenAction* ta, const float& progress);

				static Array<TweenAction*>	allTweens;

				static void OnObjectDeleted(GameObject* obj);

			public:
				static TweenAction*	move(GameObject* obj, const Vector3& to, const float duration);

				class TweenAction
				{
				public:
					float						length;
					float						position;
					float						delay;
					UpdateFuncPtr				updateFunc;
					EaseFuncPtr					easeFunc;
					GameObject*					owner;

					TweenAction(GameObject* obj);
					TweenAction* setEase(EaseTypes_t t);
					TweenAction* setDelay(const float& d);
				};

				template<class T>
				class TweenActionTyped : public TweenAction
				{
				public:
					T from, to;
					TweenActionTyped(GameObject* obj) : TweenAction(obj)
					{}
				};
			};
		}
	}
}

#endif