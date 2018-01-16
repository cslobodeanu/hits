#include "text.h"
#include "meresources.h"
#include "memath.h"
#include "recttransform.h"


using namespace MorganaEngine::Framework::Canvas;
using namespace MorganaEngine::Framework;
using namespace MorganaEngine::Base;

__implement_component(Text, Component);

#define REF_CHAR		'A'

Text::Text()
{
	myRectTransform = NULL;

	DoAddProperties();
}

Text::~Text()
{
	if (tempBuffer != null)
		delete [] tempBuffer;
}

void Text::Awake()
{
	super::Awake();

	myRectTransform = GetComponent<RectTransform>();

	font = ResourceContext::global->Load<Font>("default");

	style = StyleSimple;
	scaling = Vector2::One;

	tempBuffer = NULL;
	tempBufferSize = 0;
}

void Text::Draw(Font* f, const char* str, const float x, const float y)
{
	if (str == NULL) return;

	if(style == StyleSimple)
	{
		DrawBase(f, str, x, y);
		return;
	}

	if(style & StyleShadowed)
	{
		Color c = color * 0.2f; c.a = color->a * 0.85f;
		SetSpriteColor(c);
		const float ofs = Math::Mathf::Min(1.5f, GetLineHeight(f) * 0.06f);
		DrawBase(f, str, x + ofs, y + ofs);

		SetSpriteColor(color);
		DrawBase(f, str, x, y);
	}
}

Vector2 Text::Measure(Font* f, const char* str, int startChar /*= 0*/, int endChar /*= Mathf::MaxInt*/)
{
	if (str == NULL || str[0] == 0)
	{
		return Vector2::Zero;
	}

	Vector2 offset = Vector2::Zero;
	float ofsy = 0.0f;

	for(int i = startChar; str[i]; i++)
	{
		if (i >= endChar) break;

		if (str[i] == '\n')
		{
			ofsy += f->charBounds[REF_CHAR].height * scaling->y;
			continue;
		}

		Vector2 chSz = GetCharSize(f, str[i]);

		offset.y = Math::Mathf::Max(offset.y, ofsy + f->charBounds[(unsigned char)str[i]].y + chSz.y);

		offset.x += chSz.x;
		const float kern = str[i + 1] ? (float)f->GetKerning(str[i], str[i + 1]) * scaling->x : 0.0f;
		offset.x += kern;
	}

	return offset;
}

float Text::GetLineHeight(Font* f)
{
	return Measure(f, "A").y;
}

void Text::EventuallyCreateTextureForFont(Font* f)
{
	if(f->texture != NULL) return;

	f->texture = ResourceContext::global->Load<Texture>(Paths::GetPathForSameFolder(f->GetFilePath().c_str(), f->imageName));
}

Vector2 Text::GetCharSize(Font* f, const char ch)
{
	Rect& b = f->charBounds[(unsigned char)ch];
	Vector2 v((float)b.width, (float)b.height);

	v.x *= scaling->x;
	v.y *= scaling->y;

	if(ch == ' ')
	{
		v.x = v.y * 0.35f;
	}

	return v;
}

void Text::DrawBase(Font* f, const char* str, const float x, const float y)
{
	EventuallyCreateTextureForFont(f);
	if (f->texture == NULL) return;

	safeMaterial()->SetTexture(f->texture);

	const float texSz = (float)f->texture->GetBounds().width;
	const float chSz = Math::Mathf::Floor(texSz / 16.0f);

	Vector2 ofs(x, y);

	static Array<QuadInfo_t> quads;
	quads.SetLength(strlen(str));
	int quadsCount = 0;

	for (int i = 0; str[i]; i++)
	{
		const unsigned char ch = str[i];
		if (ch == '\n')
		{
			ofs.y += f->charBounds[REF_CHAR].height * scaling->y;
			ofs.x = x;
			continue;
		}
		const unsigned char chx = (ch % 16);
		const unsigned char chy = (ch / 16);

		const Vector2 measuredChar = GetCharSize(f, str[i]);

		QuadInfo_t& q = quads[quadsCount];
		q.origin = ofs;
		q.width = measuredChar.x;
		q.height = measuredChar.y;
		q.texCoord.x = (chx * chSz + f->charBounds[ch].x) / texSz;
		q.texCoord.y = (chy * chSz + f->charBounds[ch].y) / texSz;
		q.texCoord.width = (float)f->charBounds[ch].width / texSz;
		q.texCoord.height = (float)f->charBounds[ch].height / texSz;
		q.transposed = false;

		ofs.x += measuredChar.x;
		const float kern = str[i + 1] ? (float)f->GetKerning(str[i], str[i + 1]) * scaling->x : 0.0f;
		ofs.x += kern;

		quadsCount++;
	}

	PushQuads(quads.ptr(), quadsCount, false);
}

void Text::OnRenderObject()
{
	Draw(font, textToDraw, 0.0f, 0.0f);
}

void Text::OnPropertySet(_propertyDef* atr)
{
	super::OnPropertySet(atr);

	if (atr == &text || atr == &wrap || atr == &scaling)
	{
		ConvertText();
	}
}

void Text::ConvertText()
{
	if (wrap == false)
	{
		textToDraw = text;
		return;
	}

	Rectf rect = myRectTransform->rect;
	Vector2 sz = Measure(font, text->c_str());
	if (sz.x < rect.width)
	{
		textToDraw = text;
		return;
	}

	const int newTextSz = text->Length() * 2;

	if (newTextSz == 0)
	{
		textToDraw = "";
		return;
	}

	char* newText = AllocTempBuffer(newTextSz);
	int newTextIndex = 0;

	int spacePos = -1;
	const char* buf = (char*)text->c_str();
	char* ptr = (char*)buf;
	
	Vector2 size = Vector2::Zero;

	while (*ptr)
	{
		const int index = ptr - buf;
		
		if (*ptr == ' ')
			spacePos = index;

		newText[newTextIndex++] = *ptr;
		newText[newTextIndex] = 0;

		Vector2 sz = Measure(font, buf, index, index + 1);
		size.x += sz.x;

		if (size.x > rect.width && spacePos >= 0)
		{
			newText[spacePos] = '\n';
			newText[spacePos + 1] = 0;
			newTextIndex = spacePos + 1;

			size.x = 0.0f;
			ptr = (char*)buf + spacePos;
			spacePos = -1;
		}
				
		ptr++;
	}

	textToDraw = newText;
}

void Text::DoAddProperties()
{
	super::DoAddProperties();

	__add_property1(style);
	__add_property1(scaling);

	__add_property1(text);
	__add_property1(wrap);
}

char* Text::AllocTempBuffer(const int sz)
{
	if (sz > tempBufferSize)
	{
		if (tempBuffer != null)
			delete[] tempBuffer;

		tempBuffer = new char[sz];
		tempBufferSize = sz;
	}

	memset(tempBuffer, 0, tempBufferSize);

	return tempBuffer;
}

void Text::OnPreRenderObject()
{

}

void Text::OnPostRenderObject()
{

}
