#include "medocument.h"
#include "debug.h"
#include "macros.h"

#include "xml/tinyxml2.h"

using namespace MorganaEngine::Base::Parsers;

__implement_class(MEDocument, MEObject);

MEDocument::MEDocument()
{

}

MEDocument::~MEDocument()
{

}

MEDocument* MEDocument::InitWithBytes(const char* bytes, const int size)
{
	MEDocument* doc = new MEDocument();
	tinyxml2::XMLError err = doc->xml.Parse(bytes, size);
	if (err != tinyxml2::XML_SUCCESS)
	{
		DEBUG_OUT("XML parse error (%d)", err);
		SAFE_DEL(doc);
	}

	return doc;
}

MEDocument* MEDocument::InitWithString(const char* str)
{
	MEDocument* doc = new MEDocument();
	tinyxml2::XMLError err = doc->xml.Parse(str);
	if (err != tinyxml2::XML_SUCCESS)
	{
		DEBUG_OUT("XML parse error (%d)", err);
		SAFE_DEL(doc);
	}

	return doc;
}

MEDocument::Paragraph MEDocument::GetFirstParagraphWithName(const char* name)
{
	return xml.FirstChildElement("material");
}