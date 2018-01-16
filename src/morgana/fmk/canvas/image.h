#ifndef __MORGANA_FMK_CANVAS_IMAGE_H__
#define __MORGANA_FMK_CANVAS_IMAGE_H__

#include "renderstocanvas.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Canvas
		{
			class Image : public RendersToCanvas
			{
				__declare_component(Image, RendersToCanvas);
			public:
				Image();
				virtual ~Image();

				_property_object<Texture>		texture;
				_property<bool>			keepAspectRatio;
				_property<int>			drawMode;
				_property<float>		slicedTextureTopBottomBorder;
				_property<float>		slicedTextureLeftRightBorder; // percents
				_property<float>		slicedScreenTopBottomBorder;
				_property<float>		slicedScreenLeftRightBorder;
				_property<bool>			slicedDrawCenter;

				_property<bool>			isMask;

				void			SetSlicedTextureBordersFromCenterSize(float centerWidth, float centerHeight);
				void			SetSlicedTextureBordersFromCenterSize(float centerSize);

				void			SetSlicedScreenBordersFromCenterSize(float centerWidth, float centerHeight);
				void			SetSlicedScreenBordersFromCenterSize(float centerSize);

				enum DrawMode
				{
					Default,
					Sliced
				};

			protected:
			
				virtual void	Start();

				virtual void	DoAddProperties();

				virtual void	OnPreRenderObject();
				virtual void	OnRenderObject();
				virtual void	OnPostRenderObject();
			};
		}
	}
}

#endif