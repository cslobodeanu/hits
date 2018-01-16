#ifndef __MORGANA_FMK_RESOURCES_FONTS_FONT_H__
#define __MORGANA_FMK_RESOURCES_FONTS_FONT_H__

#include "../resourcefile.h"
#include "../../../base/devices/display/objects/texture.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Resources
		{
			namespace Fonts
			{
				using namespace MorganaEngine::Base::Types;
				using namespace MorganaEngine::Base::Devices::Display;
				class Font : public ResourceFile
				{
					__declare_class(Font, ResourceFile);
				public:
					Font();
					virtual ~Font();

					Array<Rect>				charBounds;
					Array<byte>				kerningPairs; // ch1, ch2, ammount
					int						maxCharSize;
					String					imageName;
					String					systemFontName;
					int						fontHeight;
					bool					isBold;

					DisplayObjects::Texture* texture;

					const int				GetKerning(const char ch1, const char ch2) const;

					void					Copy(const Font& other);

				protected:
					virtual void			ImportFromStreams(const StreamList&  streams, const char* args = NULL);
					virtual const bool		SaveToStream(Stream* s);

					char					kerningMatrix[256][256];
					void					BuildKerningMatrix();
				};
			}
		}
	}
}

#endif