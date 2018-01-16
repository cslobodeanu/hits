#ifndef __MORGANA_FMK_CANVAS_RECT_TRANSFORM_H__
#define __MORGANA_FMK_CANVAS_RECT_TRANSFORM_H__

#include "gameobjects.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Canvas
		{
			class RectTransform : public Transform
			{
				__declare_class(RectTransform, Transform);

			public:

				RectTransform();
				virtual ~RectTransform();

				_property<Rectf>	rect;
				_property<Vector2>	pivot;
				_property<Vector2>	anchorMin;
				_property<Vector2>	anchorMax;

				_property<Vector2>	offsetMin;
				_property<Vector2>	offsetMax;

				inline void SetPosition(const Pointf& p)
				{
					rect = Rectf(p.x, p.y, rect->width, rect->height);
				}

				inline void SetSize(const SizeF& sz)
				{
					rect = Rectf(rect->x, rect->y, sz.x, sz.y);
				}

				inline void FillParent(const float borderX = 0.0f, const float borderY = 0.0f)
				{
					GameObject* p = gameObject->GetParent();
					if (p != null)
					{
						anchorMin = Vector2::Zero;
						anchorMax = Vector2::One;

						offsetMin = Vector2(borderX, borderY);
						offsetMax = -Vector2(borderX, borderY);
					}
				}

				void GetWorldCorners(Vector3* corners);

				Action<>		OnResize;

			protected:

				virtual void	DoAddProperties();
				virtual void	OnPropertySet(_propertyDef* p);

				void			RecalculateRect();		

				virtual void	Update();

			private:
				bool			isDynamic;
				void			ComputeDynamicRect();
			};
		}
	}
}

#endif