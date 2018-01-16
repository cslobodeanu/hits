#include "tweenfunc.h"

void moveFunc(METween::TweenAction* ta, const float& progress)
{
	METween::TweenActionTyped<Vector3>* _ta = (METween::TweenActionTyped<Vector3>*)ta;
	Vector3 pos;
	Vector3::Lerp(&_ta->from, &_ta->to, progress, &pos);
	ta->owner->transform->position = pos;

	//DEBUG_OUT("progress %f", progress);
}
