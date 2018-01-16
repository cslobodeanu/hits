#ifndef __MORGANA_FMK_WNDSYS_DESKTOP_THEME_H__
#define __MORGANA_FMK_WNDSYS_DESKTOP_THEME_H__

#include "base.h"
#include "../resources/fonts/fonttruetype.h"

namespace MorganaEngine
{
	namespace Framework
	{
		using namespace Resources::Fonts;
		namespace WindowSystem
		{
			class DesktopTheme : public MEObject
			{
				__declare_class(DesktopTheme, MEObject);
			public:
				DesktopTheme();

				_property<Color>				titleTextColor;
				_property<Color>				titleBkgColor;

				_property<Color>				windowBkgColor;

				_property_object<FontTrueType>	titleFont;
				_property_object<FontTrueType>	symbolsFont;
				_property<int>					titleFontSize;

				_property_object<FontTrueType>	contentFont;
				_property<int>					contentFontSize;
				_property<Color>				contentColor;

			protected:

				void			SetDefaults();

				virtual void	DoAddProperties();
			};
		}
	}
}

#endif
