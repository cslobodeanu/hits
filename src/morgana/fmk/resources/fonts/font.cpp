#include "font.h"
#include "meparsers.h"
#include "mememory.h"

using namespace MorganaEngine::Framework::Resources;
using namespace MorganaEngine::Framework::Resources::Fonts;

__implement_class(Font, ResourceFile);

Font::Font()
{
	texture = NULL;
	maxCharSize = 0;
	fontHeight = 0;
	isBold = false;
}

Font::~Font()
{

}

void Font::ImportFromStreams(const StreamList&  streams, const char* args /*= NULL*/)
{
	Stream* s = streams.First();
	if(s->Open(Stream::READ) == false) return;

	int fsz = s->Size();

	LocalMemoryBlock lmb(fsz + 1);
	s->ReadBuffer((void*)lmb.ptr(), fsz);
	lmb[fsz] = 0;

	tinyxml2::XMLDocument doc;
	doc.Parse(lmb.c_str(), fsz);

	tinyxml2::XMLElement* root = doc.FirstChildElement("fontdata");
	if (root == NULL)
	{
		ASSERT(false, "Not a font");
		return;
	}

	tinyxml2::XMLElement* node = root->FirstChildElement("description");
	if (node != null)
		systemFontName = node->Attribute("value");

	node = root->FirstChildElement("systemfont");
	if (node != null)
		systemFontName = node->Attribute("value");

	if (systemFontName.IsEmpty() == false)
		name = systemFontName;

	node = root->FirstChildElement("size");
	if (node != null)
		fontHeight = String(node->Attribute("value")).ToInt();

	node = root->FirstChildElement("maxcharsize");
	if (node != null)
		maxCharSize = String(node->Attribute("value")).ToInt();

	node = root->FirstChildElement("texture");
	if (node != null)
		imageName = node->Attribute("path");

	node = root->FirstChildElement("charbounds");
	if (node != null)
	{
		Array<byte>& tmp = Base64::Decode(String(node->Attribute("value")));
		charBounds.SetLength(tmp.Length() / sizeof(Rect));
		memcpy(charBounds.ptr(), tmp.ptr(), tmp.bufsize());
	}

	node = root->FirstChildElement("kerningpairs");
	if (node != null)
	{
		Array<byte>& tmp = Base64::Decode(String(node->Attribute("value")));
		kerningPairs.SetLength(tmp.Length());
		memcpy(kerningPairs.ptr(), tmp.ptr(), tmp.bufsize());

		BuildKerningMatrix();
	}
}

const bool Font::SaveToStream(Stream* s)
{
	if(s->Open(Stream::CREATE) == false) return false;


	tinyxml2::XMLDocument doc;

	tinyxml2::XMLElement* elem = doc.NewElement("fontdata");
	tinyxml2::XMLNode* mainblock = doc.InsertFirstChild(elem);

	elem = doc.NewElement("description");
	elem->SetAttribute("value", name);
	tinyxml2::XMLNode* node = mainblock->InsertFirstChild(elem);

	elem = doc.NewElement("systemfont");
	elem->SetAttribute("value", systemFontName);
	node = mainblock->InsertAfterChild(node, elem);

	elem = doc.NewElement("size");
	elem->SetAttribute("value", fontHeight);
	node = mainblock->InsertAfterChild(node, elem);

	elem = doc.NewElement("maxcharsize");
	elem->SetAttribute("value", maxCharSize);
	node = mainblock->InsertAfterChild(node, elem);

	elem = doc.NewElement("texture");
	elem->SetAttribute("path", imageName);
	node = mainblock->InsertAfterChild(node, elem);

	String buf = Base64::Encode((const unsigned char*)charBounds.ptr(), charBounds.bufsize());
	elem = doc.NewElement("charbounds");
	elem->SetAttribute("value", buf);
	node = mainblock->InsertAfterChild(node, elem);

	buf = Base64::Encode(kerningPairs.ptr(), kerningPairs.bufsize());
	elem = doc.NewElement("kerningpairs");
	elem->SetAttribute("value", buf);
	node = mainblock->InsertAfterChild(node, elem);


	// write it
	tinyxml2::XMLPrinter xmlPrinter(0, false, 0);
	doc.Print(&xmlPrinter);
	s->WriteBuffer(xmlPrinter.CStr(), xmlPrinter.CStrSize());

	s->Close();

	return true;
}

void Font::BuildKerningMatrix()
{
	memset(kerningMatrix, 0, sizeof(kerningMatrix));
	const int pairsCount = kerningPairs.Length() / 3;

	for(int i = 0; i < pairsCount; i++)
	{
		const char ch1 = kerningPairs[3 * i];
		const char ch2 = kerningPairs[3 * i + 1];
		const char ammount = kerningPairs[3 * i + 2] - 128;
		kerningMatrix[(unsigned char)ch1][(unsigned char)ch2] = ammount;
	}
}

const int Font::GetKerning(const char ch1, const char ch2) const
{
	return kerningMatrix[(unsigned char)ch1][(unsigned char)ch2];
}

void Font::Copy(const Font& other)
{
	charBounds.Copy(other.charBounds);
	kerningPairs.Copy(other.kerningPairs);
	maxCharSize = other.maxCharSize;
	imageName = other.imageName;
	systemFontName = other.systemFontName;
	fontHeight = other.fontHeight;
	isBold = other.isBold;

	texture = other.texture;

	BuildKerningMatrix();
}