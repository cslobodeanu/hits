#include "bp.h"
#include "metypes.h"
#include "memath.h"

using namespace MorganaEngine::Base::Parsers;
using namespace MorganaEngine::Base::Types;
using namespace MorganaEngine::Base::Math;

Array<String>	_BlockParser::defines;

void _BlockParser::ResolveDefines( String& src )
{
	const char def[] = "#define";
	const int defLen = strlen(def);
	while(true)
	{
		int i = src.Find(def);
		if(i < 0)
			break;

		String s = src.c_str() + i;
		int eol = Mathf::Min(s.Find('\n'), s.Find('\r'));
		if(eol < 0)
			eol = s.Length() - 1;
		src.Delete(i, eol);

		s[eol] = 0;

		Array<String> subparams;
		s.Split(" \t", subparams);

		if(subparams.Length() > 2)
		{
			String var = subparams[1];
			String val = subparams[2];
			for(int i = 3; i < subparams.Length(); i++)
			{
				val += " ";
				val += subparams[i];
			}

			defines.Add(var);
			defines.Add(val);

			src.ReplaceString(var, val);
		}
	}
}

void _BlockParser::RemoveComments( String& src )
{
	while(true)
	{
		int indexEnd = -1;
		int index = src.Find("//");
		if(index >= 0)
		{
			String ss(src.c_str() + index);
			indexEnd = ss.Find('\n');
			if(indexEnd < 0)
				indexEnd = ss.Find('\r');
			if(indexEnd < 0)
				indexEnd = ss.Length() - 1;

			src.Delete(index, indexEnd);
			continue;
		}

		indexEnd = -1;
		index = src.Find("/*");
		if(index >= 0)
		{
			String ss(src.c_str() + index);
			indexEnd = ss.Find("*/");
			if(index < 0)
				indexEnd = ss.Length() - 1;

			if(indexEnd > 0)
			{
				src.Delete(index, indexEnd + 2);
				continue;
			}
		}

		break;
	}
}

void _BlockParser::RemoveTabs(String& src)
{
	// remove tabs
	int i = src.Find('\t');
	while(i >= 0)
	{
		src[i] = ' ';
		i = src.Find('\t');
	}
}

void _BlockParser::RemoveCRLF(String& src, const char replaceWith/* = 0*/)
{
	while(true)
	{
		int index = src.Find('\n');
		if(index >= 0)
		{
			if(replaceWith == 0)
				src.Delete(index, 1);
			else
				src[index] = replaceWith;
			continue;
		}

		index = src.Find('\r');
		if(index >= 0)
		{
			if(replaceWith == 0)
				src.Delete(index, 1);
			else
				src[index] = replaceWith;
			continue;
		}

		break;
	}
}

void _BlockParser::RemoveQuotes(String& str)
{
	/*
	if(str.Length() == 0) return;
	if(str[0] == '\"' || str[0] == '\'')
		str.Delete(0, 1);

	if(str.Last() == '\"' || str.Last() == '\'')
		str[str.Length() - 1] = 0;
	*/
	str.Trim("\"'");
}

void _BlockParser::TrimSpecialChars(String& src)
{
	int len = src.Length();
	for(int i = len - 1; i >= 0; i--)
	{
		if(src[i] == '\n') src[i] = 0;
		else
		if(src[i] == '\r') src[i] = 0;
		else
		if(src[i] == '\t') src[i] = 0;
		else
		if(src[i] == ' ') src[i] = 0;
		else
		break;
	}
}
