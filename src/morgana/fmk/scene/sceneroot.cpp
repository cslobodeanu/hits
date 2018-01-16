#include "sceneroot.h"

using namespace MorganaEngine::Framework::Scene;

__implement_class(SceneRoot, MEObject);

SceneRoot::SceneRoot()
{
	SetName("New Scene");
}

SceneRoot::~SceneRoot()
{

}

void SceneRoot::Update()
{
	super::Update();
}
