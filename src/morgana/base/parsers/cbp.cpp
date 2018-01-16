#include "cbp.h"
#include "metypes.h"

using namespace MorganaEngine::Base::Parsers;
using namespace MorganaEngine::Base::Types;

void CodeBlockParser::Parse(const char* src, Array<Block_t>& blocks)
{
	String _src(src);
	//RemoveComments(_src);
	//ResolveDefines(_src);
	RemoveTabs(_src);
	//RemoveCRLF(_src, ' ');
	TrimSpecialChars(_src);

	Array<String> list;

	int openedBlocks = 0;
	String tmp; list.Add(tmp);
	for(int i = 0; i < _src.Length(); i++)
	{
		if(_src[i] == '{')
		{
			openedBlocks++;
			if(openedBlocks == 1)
			{
				list.Add(tmp);
				continue;
			}
		}
		if(_src[i] == '}')
		{
			openedBlocks--;
			if(openedBlocks == 0)
			{
				list.Add(tmp);
				continue;
			}
		}

		String c;
		c.Printf("%c", _src[i]);
		list.Last() += c;
	}

	while(list.Length() && list.Last().IsEmpty()) list.Pop();

	ASSERT((list.Length() % 2) == 0, "Every block must have a header!");

	for(int i = 0; i < list.Length(); i++)
	{
		Block_t b;
		Array<String> words;
		list[i].Split("\n\r\t ", words); // dumb way to remove special chars from the name
		b.name = words.First();
		b.contents = list[++i];
		blocks.Add(b);
	}
}

void CodeBlockParser::PrintSource(const char* src)
{
	int li = 1;
	const int sz = strlen(src) + 1;
	char* srcCopy = new char[sz];
	strcpy_s(srcCopy, sz, src);
	char* ptr = srcCopy;
	char* line = ptr;
	while (*ptr)
	{
		if (*ptr == '\n')
		{
			*ptr = 0;
			DEBUG_OUT("[%d]%s", li, line);
			li++;
			line = ptr + 1;
		}

		ptr++;
	}

	delete[] srcCopy;
}
