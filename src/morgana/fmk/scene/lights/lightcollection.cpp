#include "lightcollection.h"
#include "light.h"

using namespace Morgana::Framework::Scene::Lights;

const float THRESHOLD = 1.0f;
const float THRESHOLD2 = THRESHOLD * THRESHOLD;

LightCollection::LightCollection()
{
	Clear();
}

LightCollection::~LightCollection()
{
	Clear();
}

void LightCollection::Clear()
{
	directionalLight = NULL;
	ambientLight = NULL;

	for (int i = 0; i < roots.Length(); i++)
	{
		SAFE_DEL(roots[i]);
	}

	roots.SetLength(0);
}

void LightCollection::PushLight(const Light* l)
{
	if (l->type == Light::AmbientLight)
	{
		ambientLight = l;
		return;
	}

	if (l->type == Light::DirectionalLight)
	{
		directionalLight = l;
		return;
	}

	LightNode_t* ln = new LightNode_t(l);
	LightNode_t* closest = (LightNode_t*)GetClosest(ln, NULL);
	if (closest == NULL)
	{
		roots.Add(ln);
		return;
	}

	closest->AddChild(ln);
}



Array<const Light*> LightCollection::GetLightsForBox(const Box& b, const int maxCount /*= 8*/)
{
	Array<const Light*> results;

	for (int i = 0; i < roots.Length(); i++)
	{
		GetLightsForBox(b, roots[i], results, maxCount);
	}

	if (results.Length() > maxCount)
		results.SetLength(maxCount);

	return results;
}

void LightCollection::GetLightsForBox(const Box& b, LightNode_t* root, Array<const Light*>& results, const int maxResults /*= 8*/)
{
	const bool intersects = Box::Intersects(b, root->bounds);
	if (intersects/* && root->children.IsEmpty()*/)
	{
		if (root->bounds.Contains(&root->light->transform->position.value, root->light->radius))
		{
			results.Add(root->light);
			if (results.Length() == maxResults) return;
		}
	}

	if (!intersects) return;

	for (LightNode_t** lns = root->children.ptr(); lns < root->children.end(); lns++)
	{
		GetLightsForBox(b, *lns, results, maxResults);
	}
}

const Light* LightCollection::GetGlobalDirectionalLight()
{
	return directionalLight;
}

const Light* LightCollection::GetGlobalAmbientLight()
{
	return ambientLight;
}

const LightCollection::LightNode_t* LightCollection::GetClosest(const LightNode_t* ln, const LightNode_t* root)
{
	if (root == NULL)
	{
		for (int i = 0; i < roots.Length(); i++)
		{
			const LightNode_t* c = GetClosest(ln, roots[i]);
			if (c != NULL)
				return c;
		}
	}
	else
	{
		if (Box::Distance2(ln->bounds, root->bounds) > THRESHOLD * THRESHOLD) return NULL;
		for (LightNode_t** lns = root->children.ptr(); lns < root->children.ptr(); lns++)
		{
			const LightNode_t* c = GetClosest(ln, *lns);
			if (c != NULL)
				return c;
		}

		return root;
	}
	return NULL;
}
