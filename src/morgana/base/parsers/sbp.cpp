#include "sbp.h"
#include "memath.h"

using namespace MorganaEngine::Base::Parsers;
using namespace MorganaEngine::Base::Types;

int				SimpleBlockParser::m_DefinesContext = 0;

SimpleBlockParser::SimpleBlockParser()
{

}

SimpleBlockParser::~SimpleBlockParser()
{
}

void SimpleBlockParser::Parse( const char* src, Array<Block_t>& blocks )
{
	String _src(src);
	RemoveComments(_src);
	//ResolveDefines(_src);
	RemoveCRLF(_src);
	RemoveTabs(_src);

	// remove not needed spaces

	for(int i = 0; i < _src.Length(); i++)
	{
		if(_src[i] == ';')
		{
			if(_src[i + 1] == ' ')
			{
				_src.Delete(i + 1, 1);
			}
		}
	}

// 	File* f = FILEMGR->CreateAbsolute("e:/development/parsed.txt");
// 	f->Write(_src.GetBuf(), _src.Length());
// 	f->Close();
// 	SAFE_DEL(f);

	Array<String> list;
	_src.Split("{}", list);

	ASSERT(list.Length() % 2 == 0, "Incomplete blocks. Missing a {//} ?");

	const int count = list.Length() / 2;

	Array<String> params;
	for(int i = 0; i < count; i++)
	{
		Block_t b;
		b.name = list[2 * i];

		ParseBlock(list[2 * i + 1], params);

		for(int j = 0; j < params.Length() / 2; j++)
		{
			b.params.Add(params[2 * j]);
			b.values.Add(params[2 * j + 1]);
		}
		blocks.Add(b);
	}
}

void SimpleBlockParser::ParseBlock( String& s, Array<String>& params )
{
	s.Split(";\n\r=", params);
	ASSERT(params.Length() % 2 == 0, "Incomplete properties");
	const int count = params.Length() / 2;

	// remove empty spaces
	for(int i = 0; i < params.Length(); i++)
	{
		params[i].Trim();
	}
	for(int i = 0; i < count; i++)
	{
		RemoveQuotes(params[2 * i + 1]);
	}
}

String SimpleBlockParser::GetDefineValue( const char* name )
{
	if(m_DefinesContext <= 0)
		return "";

	for(int i = 0; i < defines.Length(); i+=2)
	{
		if(defines[i].Equals(name))
			return defines[i + 1];
	}

	return "";
}