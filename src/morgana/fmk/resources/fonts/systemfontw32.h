#ifndef __MORGANA_FMK_RESOURCES_FONTS_SYSTEMFONTW32_H__
#define __MORGANA_FMK_RESOURCES_FONTS_SYSTEMFONTW32_H__

#include "platform.h"
#include "../images/imagefile.h"
#include "../fonts/font.h"

#ifdef OS_WINDOWS

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Resources
		{
			namespace Fonts
			{
				class Images::ImageFile;
				class SystemFontWin32
				{
				public:
					static void					Export(const char* systemFontName, const int height, const bool bold = false, const char* preferredName = NULL);
				private:
					static void*				CreateGDIFont(const char* systemFontName, const int height, bool bold = false);
					static Images::ImageFile*	CreateFontImage(const char* font_name, int height, int texSz, bool bold = false);
					static Images::ImageFile*	GetPixels(HDC hdc, HBITMAP bmp, int w, int h);
					static void					GenerateCharBounds(Fonts::Font* exportFont);
					static void					GenerateKerningPairs(Fonts::Font* exportFont);
				};
			}
		}
	}
}

#endif

#endif