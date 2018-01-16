#include "light.h"
#include "../../../base/devices/display/medisplay.h"
#include "meutils.h"

using namespace MorganaEngine::Framework::Scene::Lights;
using namespace MorganaEngine::Framework::Scene;
using namespace MorganaEngine::Base::Devices::Display;

__implement_component(Light, Component);

Light::Light()
{
	color = Color::white;
	priority = 0;

	DoAddProperties();
}

Light::~Light()
{
}

void Light::OnPropertySet(_propertyDef* atr)
{
	super::OnPropertySet(atr);

	if (atr == &attenuation && type == PointLight)
	{
		UpdateRadius();
	}
}

void Light::UpdateRadius()
{
	radius = 0.0f;

	//m_vAttenuation.value.x - 1.0f / 0.0001f + m_vAttenuation.value.y * m_fBoundingRadius  + m_vAttenuation.value.z * m_fBoundingRadius * m_fBoundingRadius = 0.0f;

	const float minAtt = 0.0001f;

	Vector3 att = attenuation;
	const float a = att.z;
	const float b = att.y;
	const float c = att.x - 1.0f / minAtt;

	// b * r + c = 0
	if (a < 0.000001f)
	{
		ASSERT(b != 0.0f);
		radius = -c / b;
		return;
	}

	// a * r ^ 2 + b * r + c = 0
	const float D = b * b - 4.0f * a * c;
	const float sqD = D > 0.0f ? Mathf::Sqrt(D) : 0.0f;

	const float x1 = (-b + sqD) / (2.0f * a);
	const float x2 = (-b - sqD) / (2.0f * a);

	radius = Mathf::Max(x1, x2);
}

void Light::DoAddProperties()
{
	super::DoAddProperties();
	__add_property1(color);
	__add_property1(priority);
	__add_property1(radius);
	__add_property1(type);
	__add_property1(drawVolume);
	__add_property1(attenuation);
}