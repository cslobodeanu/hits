#include "shaderfile.h"
#include "shaderbuilder.h"

using namespace MorganaEngine::Framework::Resources::Shaders;

__implement_class(ShaderFile, ResourceFile);

ShaderFile::ShaderFile()
{
	m_sSource = "";
}

void ShaderFile::ImportFromStreams(const StreamList&  streams, const char* args /*= NULL*/)
{
	if(streams.Length())
	{
		name = Paths::GetFileName(streams.First()->location);
		name = Paths::RemoveExtension(name);
		name.ToLower();
	}

	ShaderBuilder sb;
	sb.BeginProgram("import");

	for (int i = 0; i < streams.Length(); i++)
	{
		Stream* s = streams[i];
		s->Open(Stream::READ);
		LocalMemoryBlock lmb(s->Size() + 1);
		lmb[s->Size()] = 0;
		s->ReadBuffer((char*)lmb, s->Size());
		s->Close();

		String blockName = "";
		if(strstr(lmb, "gl_Position") != NULL)
			blockName = "VertexShader";
		else
		if(strstr(lmb, "gl_FragColor") != NULL)
			blockName = "PixelShader";
		
		if(blockName.IsEmpty())
		{
			DEBUG_OUT("Unknown file!");
			continue;
		}

		sb.BeginSourceBlock(blockName);
		if(args != NULL)
		{
			String _args;
			_args.Printf("\n\n%s\n\n", args);
			sb.AppendSource(_args);
		}
		sb.AppendSource(lmb.c_str());
		sb.EndBlock(blockName);
	}

	m_sSource = sb.EndProgram();
}

const bool ShaderFile::SaveToStream(Stream* s)
{
	if(s->Open(Stream::CREATE) == false) return false;

	s->WriteStringBuffer("// exported by morgana engine\n\n");
	s->WriteStringBuffer(m_sSource);
	
	s->Close();
	return true;
}