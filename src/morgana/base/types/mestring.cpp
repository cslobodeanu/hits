#include "mestring.h"
#include "macros.h"
#include "../compatibility/compatibility.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stringbuilder.h"

using namespace MorganaEngine::Base::Types;

char* String::allocBuf(int sz)
{
	return new char[sz];
}

void String::freeBuf()
{
	if (buf != null)
	{
		delete[] buf;
		buf = null;
	}
}

void String::copy(const char* str)
{
	freeBuf();

	const int len = str ? strlen(str) : 0;
	if(str != NULL && len)
	{
		buf = allocBuf(len + 1);
		memcpy(buf, str, len + 1);
	}

	OnStringChanged();
}

void String::wcopy(const wchar_t* str)
{
#ifdef _WIN32
	freeBuf();

    const int len = wcslen(str);
	if(str != NULL && len)
	{
        const int sz = (len + 1) << 1;
		buf = allocBuf(sz);
		memcpy(buf, str, sz);
	}
#endif

	OnStringChanged();
}


String::String()
{
	buf = NULL;
	OnInitialized();
	OnStringChanged();
}

String::String(const char* s)
{
	buf = NULL;
	OnInitialized();
	copy(s);
}

FastString tmpString1, tmpString2, tmpString3;

FastString tmpString11, tmpString12, tmpString13;

FastString tmpStringDel, tmpStringDel2;

FastString tmpStringPlus;

String::String(const char* s, const int len)
{
	buf = NULL;
	OnInitialized();

	buf = allocBuf(len + 1);
	memcpy(buf, s, len);
	buf[len] = 0;
}

String::String(const String& s)
{
	buf = NULL;
	OnInitialized();
	copy(s.buf);
}

String::String(const int& i)
{
	buf = NULL;
	OnInitialized();

    Printf("%d", i);
}

String::String(const float& f)
{
	buf = NULL;
	OnInitialized();

    Printf("%.3f", f);
}

String::String(const double& d)
{
	buf = NULL;
	OnInitialized();

    Printf("%.3f", d);
}

String::String(const bool& b)
{
	buf = NULL;
	OnInitialized();

	Printf("%s", b ? "true" : "false");
}

String::~String()
{
	freeBuf();
}

const int String::Length() const
{
	if(buf == NULL) return 0;
	return strlen(buf);
}

String::operator const char* () const
{
	return buf;
}

char& String::operator[](int i) const
{
	return buf[i];
}

String String::operator+ (const String a)
{
	const int len = Length();
	const int len_a = a.Length();

	FastStringLock f(tmpString1);
	tmpString1.InitWithSize(len + len_a + 1);

	if(len > 0)
	{
		b_strcpy((char*)tmpString1.c_str(), len + len_a + 1, buf);
		if (len_a > 0)
		{
			memcpy((char*)tmpString1.c_str() + len, a.c_str(), len_a + 1);
		}
	}
	else
	{
		if(len_a > 0)
			b_strcpy((char*)tmpString1.c_str(), len + len_a + 1, a.c_str());
		else
			tmpString1[0] = 0;
	}

	tmpString1.OnStringChanged();
	return tmpString1;
}

bool String::operator != (const String &s) const
{
	if (buf == NULL && s.buf == NULL) return false;
	if (buf == NULL && s.buf != NULL) return true;
	if (buf != NULL && s.buf == NULL) return true;

	return strcmp(buf, s.buf) != 0;
}

bool String::operator == (const String &s) const
{
    return strcmp(buf, s.buf) == 0;
}

bool String::operator != (const char*const s) const
{
    return strcmp(buf, s) != 0;
}

bool String::operator == (const char*const s) const
{
	if(Length() == 0)
	{
		return strlen(s) == 0;
	}

	return strcmp(buf, s) == 0;
}

bool String::Equals(const char* s) const
{
    if(buf == NULL && s != NULL) return false;
    if(buf != NULL && s == NULL) return false;
    if(buf == NULL && s == NULL) return true;

	return strcmp(buf,s) == 0;
}

bool String::Equals(const String& s) const
{
    return Equals(s.buf);
}

const String& String::operator = (const char* str)
{
	freeBuf();
	copy(str);
	return *this;
}

String& String::operator = (String str)
{
	freeBuf();
	copy(str.buf);
	return *this;
}

String& String::operator += (String str)
{
	const int len2 = str.Length();
	if(len2 == 0)
	{
		return *this;
	}
	const int len = Length();
	if(len == 0)
	{
		copy(str.buf);
	}
	else
	{
		FastStringLock f(tmpString1);
		tmpString1.InitWithBuffer(buf);
		freeBuf();
		const int sz = len + len2 + 1;
		buf = allocBuf(sz);
		memcpy(buf, tmpString1.c_str(), len);
		memcpy(buf + len, str, len2 + 1);
	}

	OnStringChanged();

	return *this;
}

String& String::operator += (int val)
{
	FastStringLock f(tmpString1);
	tmpString1.Printf("%d", val);
	*this += tmpString1;
	return *this;
}

String& String::operator += (float val)
{
	FastStringLock f(tmpString1);
	tmpString1.Printf("%f", val);
	*this += tmpString1;
	return *this;
}

String& String::operator += (char val)
{
	if (buf == NULL)
	{
		buf = allocBuf(2);
		buf[0] = val;
		buf[1] = 0;
	}
	else
	{
		const int sz = Length();

		FastStringLock f(tmpString1);

		tmpString1.InitWithBuffer(buf);
	
		freeBuf();

		buf = allocBuf(sz + 2);
		b_strcpy(buf, sz + 2, tmpString1.c_str());
		buf[sz] = val;
		buf[sz + 1] = 0;
	}
	return *this;
}

String String::operator+(const char* a)
{
	const int l1 = Length();
	const int l2 = strlen(a);

	FastStringLock f(tmpString1);

	tmpString1.InitWithSize(l1 + l2 + 1);
	memcpy((void*)tmpString1.c_str(), buf, l1);
	memcpy((char*)tmpString1.c_str() + l1, a, l2 + 1);

	return tmpString1;
}

String String::operator+ (const int &val)
{
	FastStringLock f(tmpStringPlus);
	tmpStringPlus.Printf("%d", val);
	*this += tmpStringPlus;
	return *this;
}

String String::Substring(int i)
{
	return buf + i;
}

String String::Substring(int start, int end)
{
    if(Length() == 0) return "";

	FastStringLock f(tmpString13);
	tmpString13.InitWithBuffer(buf + start);
	tmpString13.buf[end - start] = 0;
	return tmpString13;
}

String String::Substring(const char* src, int start, int end)
{
	FastStringLock f(tmpString1);
	tmpString1.InitWithSize(end - start + 1);
	memcpy(tmpString1.buf, src + start, end - start);
	tmpString1.buf[end - start] = 0;
	return tmpString1;
}

char String::CharAt(int i)
{
	return buf[i];
}

void String::ToUpper()
{
	const int len = Length();
	for(int i = 0; i < len; i++)
	{
		buf[i] = ToUpperCase(buf[i]);
	}

	OnStringChanged();
}

void String::ToLower()
{
	const int len = Length();
	for(int i = 0; i < len; i++)
	{
		char& c = buf[i];
		if(c < 'A' || c > 'Z') continue;
		c -= 'A' - 'a';
	}

	OnStringChanged();
}

String String::ToLower() const
{
	const int len = Length();

	FastStringLock f(tmpString1);
	tmpString1.InitWithSize(len + 1);
	tmpString1.buf[len] = 0;
	for (int i = 0; i < len; i++)
	{
		tmpString1.buf[i] = buf[i];
		char& c = tmpString1.buf[i];
		if (c < 'A' || c > 'Z') continue;
		c -= 'A' - 'a';
	}
	return tmpString1;
}

String String::ToUpper() const
{
	const int len = Length();

	FastStringLock f(tmpString1);
	tmpString1.InitWithSize(len + 1);
	tmpString1.buf[len] = 0;
	for (int i = 0; i < len; i++)
	{
		tmpString1.buf[i] = ToUpperCase(buf[i]);
	}

	return tmpString1;
}

void String::Printf(const char* s, ...)
{
	const int sz = 8192;
	static char str[sz];
	va_list args;
	va_start (args, s);
	b_vsprintf(str, sz, s, args);
	va_end (args); // end processing the argument list.
	copy(str);
}

String String::FormatF(const char*& fmtStr)
{
	static char str[8192 * 4];
	va_list args;
	va_start (args, fmtStr);
	b_vsprintf(str, 8192 * 4, fmtStr, args);
	va_end (args); // end processing the argument list.
	return str;
}

String String::Format(const char* s, ...)
{
	static const int sz = 8192;
	static char str[sz];
	va_list args;
	va_start(args, s);
	b_vsprintf(str, sz, s, args);
	va_end(args); // end processing the argument list.
	return str;
}

void String::Printf(const wchar_t* s, ...)
{
#ifdef _WIN32
	wchar_t str[1024];
	va_list args;
	va_start (args, s);
    b_vswprintf(str, 1024, s, args);
	va_end (args); // end processing the argument list.
    wcopy(str);
#endif
}

int String::Find(char c)
{
	const int len = Length();
	for(int i = 0; i < len; i++)
	{
		char& ch = buf[i];
		if (ch == c) return i;
	}
	return -1;

}

int String::RemoveCharacter(char c)
{
	int ret = 0;
	const int len = Length();
	int index = 0;

	FastStringLock f(tmpString1);
	tmpString1.Clear();

	for(int i = 0; i < len; i++)
	{
		char& ch = buf[i];
		if (ch == c) 
		{
			tmpString1 += Substring(index,i);
			index = i+1;
			ret++;
		}
	}

	if (index<len)
		tmpString1 +=Substring(index,len);
	
	*this = tmpString1;

	return ret;

}

int String::ReplaceCharacter(char c,char newc)
{
	int ret = 0;
	const int len = Length();
	for(int i = 0; i < len; i++)
	{
		char& ch = buf[i];
		if (ch == c) 
		{
			buf[i] = newc;
			ret++;
		}
	}

	OnStringChanged();

	return ret;

}

int String::ToInt() const
{
    return atoi(buf);
}

float String::ToFloat() const
{
    if(buf == NULL || buf[0] == 0) return 0.0f;
    return (float)atof(buf);
}

void String::ToFloats(float& f1, float& f2) const
{
	if (buf == NULL || buf[0] == 0)
	{
		f1 = f2 = 0.0f;
	}
	sscanf_s(buf, "%f, %f", &f1, &f2);
}

void String::ToFloats(float& f1, float& f2, float& f3) const
{
	if (buf == NULL || buf[0] == 0)
	{
		f1 = f2 = f3 = 0.0f;
	}

	sscanf_s(buf, "%f, %f, %f", &f1, &f2, &f3);
}

void String::ToFloats(float& f1, float& f2, float& f3, float& f4) const
{
	if (buf == NULL || buf[0] == 0)
	{
		f1 = f2 = f3 = f4 = 0.0f;
	}
	sscanf_s(buf, "(%f, %f, %f, %f)", &f1, &f2, &f3, &f4);
}

int String::Find(const char* str) const
{
    if(buf == NULL) return -1;
    if(str == NULL) return -1;
    const char* res = strstr(buf, str);
    if(res == NULL) return -1;
    return res - buf;
}

void String::Delete(const int& count)
{
    if(count >= Length())
    {
        freeBuf();
        return;
    }
	FastStringLock f(tmpStringDel);
    tmpStringDel.InitWithBuffer(buf + count);
    copy(tmpStringDel.buf);
}

void String::Delete( unsigned int start, unsigned int count )
{
// 	const int l = Length();
// 	memcpy(buf + start, buf + start + count, l - count + 1);

	FastStringLock f(tmpStringDel);
	tmpStringDel.InitWithBuffer(buf);

	const int sz = Length();

	buf = allocBuf(sz - count + 1);
	memcpy(buf, tmpStringDel.c_str(), start);
	memcpy(buf + start, tmpStringDel.c_str() + start + count, sz - start - count + 1);

	OnStringChanged();
}

char String::Last()
{
	if(Length() == 0) return 0;
	return buf[Length() - 1];
}

char String::First()
{
	if(Length() == 0) return 0;
	return buf[0];
}

int String::FindLast( char c )
{
	const int len = Length();
	for(int i = 1; i < len; i++)
	{
		char& ch = buf[len-i];
		if (ch == c) return len-i;
	}
	return -1;
}

const char* String::c_str() const
{
    return buf;
}

void String::Split(const char* separators, Array<String>& out) const
{
	out.SetLength(0);

	FastStringLock f(tmpString1);
	tmpString1.InitWithBuffer(buf);

	while(tmpString1.Length())
	{
		int i = -1;
		const int sc = strlen(separators);
		for(int k = 0; k < sc; k++)
		{
			const int j = tmpString1.Find(separators[k]);
			if(j >= 0)
			{
				if(j < i || i == -1)
					i = j;
			}
		}
		if(i < 0) i = tmpString1.Length();
		if(i >= 0)
		{
			if(i > 0)
			{
				out.Add(tmpString1.Substring(0, i));
			}
			const int l1 = tmpString1.Length();
			tmpString1.Delete(i + 1);
			const int l2 = tmpString1.Length();
		}
	}
}

void String::Split(const char* src, const char* separators, Array<String>& out)
{
	FastStringLock f(tmpString11);
	tmpString11.InitWithBuffer(src);
	tmpString11.Split(separators, out);
}

Array<String> String::Split(const char* src, const char* separators)
{
	Array<String> out;

	FastStringLock f(tmpString11);
	tmpString11.InitWithBuffer(src);
	tmpString11.Split(separators, out);
	return out;
}

const int String::ComputeHashCode()
{
	return ComputeHashCode(buf);
}

const int String::ComputeHashCode(const char* str)
{
	if(str == NULL) return 0;
	int h = 0;
	for(char* s = (char*)str; *s; h += *s, s++);
	return h;
}

const bool String::TestWildcard( const char* pattern ) const
{
	return TestWildcard(buf, pattern);
}

bool String::TestWildcard( const char* name, const char* pattern )
{
	//empty strings will fail the match
	if (!name[0])
		return false;

	if (!pattern[0])
		return false;


	//count the number of stars from the beginning
	int nw = 0;	//star count
	int ln = 0;	//char count
	for (int i=0; pattern[i] && (ln==nw); i++)
	{
		if (pattern[i] == '*')
			nw ++;

		ln ++;
	}

	if (nw == ln)
	{
		//we have only stars (including just one) so match anything
		return true;
	}

	int sp=0;
	int pp=0;

	if (pattern[0] != '*')
	{
		//if we have a pattern start, it must match the name start
		while (pattern[pp] && (pattern[pp] != '*'))
		{
			if (pattern[pp] == name[sp])
			{
				pp++;
				sp++;
			}
			else
			{
				//one of the chars doesn't match
				return false;
			}
		}
	}

	if ((pattern[pp] == 0) && (name[sp] == 0))
	{
		//we have a match (no wildcards)
		return true;
	}

	char str[256];
	int sLen = 0;

	char ptr[256];
	int pLen = 0;

	while ((str[sLen]=name[sp+sLen]) != 0)
	{
		sLen++;
	}

	while ((ptr[pLen]=pattern[pp+pLen]) != 0)
	{
		pLen++;
	}

	if (sLen > 0 && pLen > 0 && ptr[pLen-1] != '*')
	{
		pp = pLen-1;
		sp = sLen-1;

		//if we have a pattern end, it must match the name end
		while (pp && (ptr[pp] != '*'))
		{
			if (sp >= 0 && ptr[pp] == str[sp])
			{
				pp--;
				sp--;
			}
			else
			{
				//one of the chars doesn't match
				return false;
			}
		}

		str[sp+1] = 0;
		ptr[pp+1] = 0;
	}

	//now the pattern has stars at the start and end
	//also both have been stripped so we can match anywhere

	pp = 0;
	char min[256];
	int mLen;

	char* strptr = str;

	while (ptr[pp])
	{
		//jump over wildcards
		while (ptr[pp] == '*')
		{
			pp++;
		}

		//we're at the end of the pattern and we had only wildcards so match
		if (!ptr[pp])
		{
			return true;
		}

		//now search for a mini-pattern
		mLen = 0;
		while (ptr[pp] && (ptr[pp] != '*'))
		{
			min[mLen++] = ptr[pp++];
		}

		min[mLen] = 0;

		//search for the first occurrence of the mini-pattern in the name
		strptr = strstr(strptr, min);

		//if nothing found, no match
		if (!strptr)
			return false;
	}

	//should never get here (early exit in the while), but to keep the compiler happy
	return true;
}

const bool String::IsEmpty() const
{
	return buf == null || buf[0] == 0;
}

bool String::IsNumber( const char c )
{
	return c >= '0' && c <= '9';
}

const bool String::IsNumber() const
{
	for(int i = 0; buf[i] != 0; i++)
	{
		if(buf[i] >= '0' && buf[i] <= '9')
			continue;

		return false;
	}

	return true;
}

String String::GetPrefix( const char* str, const char* separator /*= ":"*/ )
{
	FastStringLock f(tmpString1);
	tmpString1.InitWithBuffer(str);
	const int index = tmpString1.Find(separator);
	if(index > 0)
	{
		tmpString1[index] = 0;
	}
	else
	{
		tmpString1 = "";
	}

	return tmpString1;
}

bool String::HasPrefix( const char* str, const char* separator /*= ":" */)
{
	if(str == NULL) return false;
	return strstr(str, separator) != NULL;
}

bool String::StartsWith(const char* str)
{
	return Find(str) == 0;
}


void String::RemovePrefix( const char* separator /*= ":"*/ )
{
	const int index = Find(separator);
	if(index > 0)
		Delete(0, index + strlen(separator));
}

bool String::EqualsNC(const char* s)  const
{
	FastStringLock f1(tmpString1);
	FastStringLock f2(tmpString2);
	tmpString1.InitWithBuffer(buf);tmpString1.ToLower();
	tmpString2.InitWithBuffer(s); tmpString2.ToLower();

	return tmpString1.Equals(tmpString2);
}

void String::ReplaceString( const char* str_to_replace, const char* new_str )
{
	while(true)
	{
		const int i = Find(str_to_replace);
		if(i < 0) break;
		FastStringLock f(tmpString1);
		tmpString1.InitWithBuffer(*this);
		tmpString1[i] = 0;
		tmpString1 += new_str;

		FastStringLock f2(tmpString2);
		tmpString2.InitWithBuffer(c_str() + i + strlen(str_to_replace));
		tmpString1 += tmpString2;
		*this = tmpString1;
	}

	OnStringChanged();
}

bool String::ToBool() const
{
	return EqualsNC("yes") || EqualsNC("true") || !EqualsNC("0");
}

void String::Clear()
{
	freeBuf();
	OnStringChanged();
}

void String::Append(const char* str)
{
	if(str == NULL || str[0] == 0) return;

	FastStringLock f(tmpString1);

	tmpString1.InitWithBuffer(buf);
	freeBuf();

	const int strLen1 = tmpString1.Length();
	const int strLen2 = strlen(str);
	buf = allocBuf(strLen1 + strLen2 + 1);
	memcpy(buf, tmpString1.c_str(), strLen1);
	memcpy(buf + strLen1, str + 1, strLen2 + 1); // include zero
}

char String::ToUpperCase(const char ch)
{
	if (ch < 'a' || ch > 'z') return ch;
	return ch + 'A' - 'a';
}

bool String::IsUpperCase(const char ch)
{
	return ch >= 'A' && ch < 'Z';
}

void String::Trim(const char* chars/* = " \t"*/)
{
	if (chars == NULL || strlen(chars) == 0) return;

	// beginning
	char* c = buf;
	int index = 0;
	while (strchr(chars, *c) != NULL)
	{
		c++;
		if (*c == 0) break;
		index++;
	}

	Delete(index);

	// ending
	int len = strlen(buf) - 1;
	index = len;
	c = buf + index;
	while (strchr(chars, *c) != NULL)
	{
		c--;
		index--;
		if (index < 0) break;
	}

	if (index < len)
		Delete(index + 1, len - index);
}

bool String::Contains(char c)
{
	return Find(c) > 0;
}

bool String::Contains(const char* str) const
{
	return Find(str) > 0;
}

void String::InitWithBuffer(const char* str)
{
	copy(str);
}