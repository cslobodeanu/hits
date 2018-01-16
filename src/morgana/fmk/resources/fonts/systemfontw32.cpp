#include "systemfontw32.h"
#include "../images/imagefile.h"
#include "memath.h"

using namespace  MorganaEngine::Framework::Resources::Fonts;
using namespace  MorganaEngine::Framework::Resources::Images;
using namespace  MorganaEngine::Base;


#ifdef OS_WINDOWS

#include <windows.h>

void SystemFontWin32::Export(const char* systemFontName, const int height, const bool bold /*= false*/, const char* preferredName /*= NULL*/)
{
	String fn = "fonts/";
	if (preferredName == NULL)
	{
		fn += systemFontName;
		fn.ToLower();
		fn.ReplaceCharacter(' ', '_');
		fn += "_";
		fn += String(height);
	}
	else
	{
		fn += preferredName;
	}

	String picName = fn + ".png";
	String fntName = fn + ".mfnt";

	Font* fnt = new Font();
	fnt->systemFontName = systemFontName;
	fnt->imageName = Paths::GetFileName(picName);
	fnt->fontHeight = height;
	fnt->isBold = bold;

	GenerateCharBounds(fnt);
	GenerateKerningPairs(fnt);
	ResourceFile::Save(fnt, fntName);

	const int texSz = (int)Math::Mathf::Floor(16.0f * (float)height * 1.5f);
	ImageFile* img = CreateFontImage(systemFontName, height, texSz, bold);
	ResourceFile::Save(img, picName);

	SAFE_DEL(fnt);
	SAFE_DEL(img);
}

void* SystemFontWin32::CreateGDIFont(const char* systemFontName, const int height, bool bold /*= false*/)
{
	HFONT f = ::CreateFont(-height, 0, 0, 0, bold ? FW_BOLD : FW_DONTCARE,  FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_NATURAL_QUALITY, VARIABLE_PITCH | FF_SCRIPT,TEXT(systemFontName));

	return f;
}

ImageFile* SystemFontWin32::CreateFontImage(const char* font_name, int height, int texSz, bool bold /*= false*/)
{
	const int w = texSz;
	const int h = texSz;

	int offset_left = 0;
	int offset_top = 0;

	int cellsize = w / 16;

	RECT r; r.left = r.top = 0; r.right = w; r.bottom = h;

	HDC screen = ::GetDC(NULL);
	HDC hdc = ::CreateCompatibleDC(screen);
	HBITMAP bmp = ::CreateCompatibleBitmap(screen, w, h);
	::SelectObject(hdc, bmp);

	HBRUSH brush1 = ::CreateSolidBrush(RGB(0, 0, 0));

	::FillRect(hdc, &r, brush1);

	HFONT font = (HFONT)CreateGDIFont(font_name, height, bold);
	::SelectObject(hdc, font);

	::SetMapMode(hdc, MM_TEXT);

	::SetTextColor(hdc, RGB(255, 255, 255));
	::SetBkMode(hdc, TRANSPARENT);

	for(int y = 0; y < 16; y++)
	{
		for(int x = 0; x < 16; x++)
		{
			const int ch = x + y * 16;
			String chr; chr.Printf("%c", (char)ch);
			RECT cell;

			ABC abc;
			::GetCharABCWidths(hdc, ch, ch, &abc);

			cell.left = x * cellsize + offset_left - abc.abcA;
			cell.top = y * cellsize + offset_top;
			cell.right = cell.left + cellsize - offset_left;
			cell.bottom = cell.top + cellsize - offset_top;
			::DrawText(hdc, (char*)chr.c_str(), chr.Length(), &cell, DT_NOCLIP);//DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}

	ImageFile* img = GetPixels(hdc, bmp, w, h);
	img->FlipY();

	::DeleteObject(brush1);
	::DeleteObject(font);
	::DeleteObject(bmp);
	::DeleteDC(hdc);
	::ReleaseDC(NULL, screen);

	return img;
}

ImageFile* SystemFontWin32::GetPixels(HDC hdc, HBITMAP bmp, int w, int h)
{
	ImageFile* img = new ImageFile();
	img->Alloc(w, h, 32);
	unsigned char* imgPtr = (unsigned char*)img->GetPixels();
	
	BITMAPINFO bi;
	memset(&bi, 0, sizeof(BITMAPINFO));
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	::GetDIBits(hdc, bmp, 0, 0, NULL, &bi, DIB_RGB_COLORS);

	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biBitCount = 32;
	bi.bmiHeader.biCompression = BI_RGB;

	unsigned char* bits = new unsigned char[bi.bmiHeader.biSizeImage + 3 * sizeof(DWORD)];
	memset(bits, 0, bi.bmiHeader.biSizeImage);
	::GetDIBits(hdc, bmp, 0, bi.bmiHeader.biHeight, bits, &bi, DIB_RGB_COLORS);

	const int aw = bi.bmiHeader.biSizeImage / (4 * bi.bmiHeader.biHeight);
	for(int y = 0; y < h; y++)
	{
		for(int x = 0; x < w; x++)
		{
			COLORREF* rgb = (COLORREF*)(bits + (x + y * aw) * 4);
			unsigned char* pixel = imgPtr + (x + y * w) * 4;

			//float alpha = (float)GetRValue(*rgb) *0.33f + (float)GetGValue(*rgb) * 0.59f + (float)GetBValue(*rgb) * 0.11f;
			float alpha = (float)(GetRValue(*rgb) + (float)GetGValue(*rgb) + (float)GetBValue(*rgb)) / 3.0f;
			if(alpha > 255.0f) alpha = 255.0f;

			pixel[0] = 255;
			pixel[1] = 255;
			pixel[2] = 255;
			pixel[3] = (unsigned char)alpha;
		}
	}

	delete [] bits;

	return img;
}

void SystemFontWin32::GenerateCharBounds(Font* exportFont)
{
	const bool glyph_box = false;

	HDC screen = ::GetDC(NULL);
	HDC hdc = ::CreateCompatibleDC(screen);
	HBITMAP bmp = ::CreateCompatibleBitmap(screen, 512, 512);
	::SelectObject(hdc, bmp);

	HFONT font = (HFONT)CreateGDIFont(exportFont->systemFontName, exportFont->fontHeight, exportFont->isBold);
	::SelectObject(hdc, font);

	::SetMapMode(hdc, MM_TEXT);

	exportFont->maxCharSize = -100000;
	exportFont->charBounds.SetLength(256);

	for(int ch = 0; ch < 256; ch++)
	{
		String chr; chr.Printf("%c", (char)ch);

		SIZE size;
		int ofsx = 0;
		if(chr > 0)
		{
			::GetTextExtentPoint32(hdc, chr, 1, &size);

			ABC abc;
			::GetCharABCWidths(hdc, ch, ch, &abc);

			if(glyph_box)
			{	
				ofsx = 0;
				size.cx = /*abc.abcA + */abc.abcB;// + abc.abcC;
			}
			else
			{
				ofsx = -abc.abcA;
			}
		}
		else
		{
			size.cx = size.cy = 0;
		}

		exportFont->maxCharSize = Math::Mathf::Max(exportFont->maxCharSize, size.cx);
		exportFont->maxCharSize = Math::Mathf::Max(exportFont->maxCharSize, size.cy);

		exportFont->charBounds[ch].Set(ofsx, 0, size.cx, size.cy);
	}

	::DeleteObject(font);
	::DeleteObject(bmp);
	::DeleteDC(hdc);
	::ReleaseDC(NULL, screen);
}

void SystemFontWin32::GenerateKerningPairs(Font* exportFont)
{
	const int w = exportFont->fontHeight * 10;
	const int h = exportFont->fontHeight * 10;

	HDC screen = ::GetDC(NULL);
	HDC hdc = ::CreateCompatibleDC(screen);
	HBITMAP bmp = ::CreateCompatibleBitmap(screen, w, h);
	::SelectObject(hdc, bmp);

	HFONT font = (HFONT)CreateGDIFont(exportFont->systemFontName, exportFont->fontHeight, exportFont->isBold);
	::SelectObject(hdc, font);

	::SetMapMode(hdc, MM_TEXT);

	int count = ::GetKerningPairs(hdc, 0, NULL);
	KERNINGPAIR* pairs = new KERNINGPAIR[count];

	::GetKerningPairs(hdc, count, pairs);

	exportFont->kerningPairs.SetLength(0);
	for(int i = 0; i < count; i++)
	{
		exportFont->kerningPairs.Add((char)pairs[i].wFirst);
		exportFont->kerningPairs.Add((char)pairs[i].wSecond);
		exportFont->kerningPairs.Add((char)(pairs[i].iKernAmount + 128));
	}

	delete [] pairs;

	::DeleteObject(font);
	::DeleteObject(bmp);
	::DeleteDC(hdc);
	::ReleaseDC(NULL, screen);

}


#endif
