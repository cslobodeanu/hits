#include "recttransform.h"

using namespace MorganaEngine::Framework::Canvas;

__implement_class(RectTransform, Transform);

RectTransform::RectTransform()
{
	pivot = Vector2::Zero;
	anchorMin = Vector2::Zero;
	anchorMax = Vector2::Zero;

	offsetMin = Vector2::Zero;
	offsetMax = Vector2::Zero;

	RecalculateRect();

	DoAddProperties();
}

RectTransform::~RectTransform()
{

}

void RectTransform::DoAddProperties()
{
	super::DoAddProperties();
	__add_property1(rect);
	__add_property1(pivot);
	__add_property1(anchorMin);
	__add_property1(anchorMax);
	__add_property1(offsetMin);
	__add_property1(offsetMax);
}

void RectTransform::OnPropertySet(_propertyDef* p)
{
	super::OnPropertySet(p);

	if (p == &anchorMin || p == &anchorMax || p == &pivot || p == &offsetMin || p == &offsetMax)
	{
		RecalculateRect();
	}

	if (p == &rect)
	{
		RecalculateRect();
	}
}

void RectTransform::RecalculateRect()
{
	localOffset = rect->TopLeft();

	isDynamic = false;

	float eps = 0.00001f;

	if (anchorMin->LengthSquared() < eps && anchorMax->LengthSquared() < eps && offsetMin->LengthSquared() < eps && offsetMax->LengthSquared() < eps)
	{
		return;
	}

	isDynamic = true;

	ComputeDynamicRect();
}

void RectTransform::ComputeDynamicRect()
{
	if (GetParent() == null) return;

	RectTransform* prt = gameObject->GetParent()->GetComponent<RectTransform>();
	if (prt == NULL) return;

	Vector2 min = Vector2::Multiply(anchorMin, prt->rect->size()) + offsetMin;
	Vector2 max = Vector2::Multiply(anchorMax, prt->rect->size()) + offsetMax;

	float eps = 0.00001f;

	Rectf r = rect;

	if (Vector2::Distance2(min, max) < eps)
		r.pos(min);
	else
		r.Set(min, max - min);

	if (r == rect)
		return;

	rect = r;

	OnResize();

	localOffset = rect->TopLeft();
}

void RectTransform::Update()
{
	super::Update();

	if (isDynamic)
		ComputeDynamicRect();
}

void RectTransform::GetWorldCorners(Vector3* corners)
{
	corners[0] = GetWorldMatrix() * rect->TopLeft();
	corners[1] = GetWorldMatrix() * rect->TopRight();
	corners[2] = GetWorldMatrix() * rect->BottomRight();
	corners[3] = GetWorldMatrix() * rect->BottomLeft();
}
