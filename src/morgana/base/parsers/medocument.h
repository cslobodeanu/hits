#ifndef __MORGANA_BASE_PARSERS_MEDOCUMENT_H__
#define __MORGANA_BASE_PARSERS_MEDOCUMENT_H__

#include "xml/tinyxml2.h"
#include "reflection.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Parsers
		{
			class MEDocument : public MEObject
			{
				__declare_class(MEDocument, MEObject);
				tinyxml2::XMLDocument xml;
			public:

				typedef tinyxml2::XMLElement*	Paragraph;

				MEDocument();
				virtual ~MEDocument();

				static MEDocument*	InitWithBytes(const char* bytes, const int size);
				static MEDocument*	InitWithString(const char* str);

				Paragraph			GetFirstParagraphWithName(const char* name);
			};
		}
	}
}


#endif