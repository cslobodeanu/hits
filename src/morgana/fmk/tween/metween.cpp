#include "metween.h"
#include "../times/time.h"
#include "gameobjects.h"
#include "meapplication.h"
#include "tweenease.h"
#include "tweenfunc.h"

using namespace MorganaEngine::Framework::Tween;
using namespace MorganaEngine::Framework::Times;
using namespace MorganaEngine::Framework::Application;

Array<METween::TweenAction*>	METween::allTweens;

void METween::Initialize()
{
	MEApplication::main->OnDeleteGameObject += CallbackStatic(OnObjectDeleted, GameObject*);
}

void METween::Destroy()
{
	for (int i = 0; i < allTweens.Length(); i++)
	{
		TweenAction* ta = allTweens[i];
		SAFE_DEL(ta);
	}
}

void METween::UpdateAll()
{
	for (int i = 0; i < allTweens.Length(); i++)
	{
		TweenAction* ta = allTweens[i];
		if (ta->position > ta->length || ta->length == 0.0f)
		{
			ta->updateFunc(ta, 1.0f);
			SAFE_DEL(ta);
			allTweens.RemoveNoFast(i);
			i--;
		}
	}

	for (int i = 0; i < allTweens.Length(); i++)
	{
		TweenAction* ta = allTweens[i];
		if (ta->delay > 0.0f)
			ta->delay -= Time::deltaTime;
		if (ta->delay > 0.0f) continue;

		const float progress = ta->easeFunc(Mathf::Saturate(ta->position / ta->length));
		ta->updateFunc(ta, progress);

		ta->position += Time::deltaTime;
	}
}

void METween::OnObjectDeleted(GameObject* obj)
{
	for (int i = 0; i < allTweens.Length(); i++)
	{
		TweenAction* ta = allTweens[i];
		if (ta->owner == obj)
		{
			allTweens.RemoveNoFast(i);
			SAFE_DEL(ta);
			i--;
		}
	}
}

METween::TweenAction* METween::move(GameObject* obj, const Vector3& to, const float duration)
{
	TweenActionTyped<Vector3>* ta = new TweenActionTyped<Vector3>(obj);
	ta->from = obj->transform->position;
	ta->to = to;
	ta->length = duration;
	ta->setEase(EaseTypes_t::Linear);
	ta->updateFunc = moveFunc;
	return ta;
}

METween::TweenAction::TweenAction(GameObject* obj)
{
	position = 0.0f;
	length = 0.0f;
	delay = 0.0f;
	owner = obj;
	allTweens.Add(this);
}

METween::TweenAction* METween::TweenAction::setEase(EaseTypes_t t)
{
	switch (t)
	{
	case EaseTypes_t::Linear: easeFunc = easeLinear; break;
	default: easeFunc = easeLinear; break;
	}

	return this;
}

METween::TweenAction* METween::TweenAction::setDelay(const float& d)
{
	delay = d;
	return this;
}
