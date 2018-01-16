#include "shaderbuilder.h"
#include "meio.h"
#include "displayobjectfactory.h"

using namespace MorganaEngine::Framework::Resources::Shaders;
using namespace MorganaEngine::Base::Devices::Display;

ShaderBuilder::ShaderBuilder()
{
	currentOpenedBlock = -1;
}

ShaderBuilder::~ShaderBuilder()
{

}

int ShaderBuilder::GetBlockIndexWithName(const char* name)
{
	for(int i = 0; i < programBlocks.Length(); i++)
	{
		if(programBlocks[i].name == name)
			return i;
	}
	return -1;
}

void ShaderBuilder::BeginProgram(const char* name)
{
	programBlocks.Clear();
	programName = name;
	currentOpenedBlock = -1;
	completedSource = "";
}

void ShaderBuilder::BeginSourceBlock(const char* blockName)
{
	currentOpenedBlock = GetBlockIndexWithName(blockName);
	if(currentOpenedBlock < 0)
	{
		ProgramBlock_t pb;
		pb.name = blockName;
		programBlocks.Add(pb);
		currentOpenedBlock = programBlocks.Length() - 1;
	}
}

void ShaderBuilder::AppendSource(const char* txt)
{
	ASSERT(currentOpenedBlock >= 0, "No source block opened !");

	ProgramBlock_t& pb = programBlocks[currentOpenedBlock];
	pb.source += "\n";
	pb.source += txt;
}

void ShaderBuilder::AppendSourceFromFile(const char* path)
{
	String fullPath = Paths::GetPathInDocuments("shaders");
	fullPath = Paths::ConcatPathWithFilename(fullPath, path);

	singleton<MFiles> files;
	Stream* s = files->GetStream(fullPath);
	AppendSourceFromStream(s);
}

void ShaderBuilder::AppendSourceFromStream(Stream* s)
{
	if(s == NULL) return;
	if(s->Open(Stream::READ))
	{
		const long size = s->Size();
		Base::Memory::LocalMemoryBlock buf(size + 1);
		buf[size] = 0;
		s->ReadBuffer(buf, size);
		AppendSource(buf);
		s->Close();
	}
}

void ShaderBuilder::EndBlock(const char* blockName)
{
	currentOpenedBlock = -1;
}

String ShaderBuilder::EndProgram()
{
	completedSource = "";
	for(int i = 0; i < programBlocks.Length(); i++)
	{
		completedSource += programBlocks[i].name;
		completedSource += "\n";
		completedSource += "{\n";
		completedSource += programBlocks[i].source;
		completedSource += "\n}\n\n";
	}

	return completedSource;

	/*
	DisplayObjects::ShaderProgram* prg =  new DisplayObjects::ShaderProgram();

	implementation<DisplayObjectFactory>()->GenerateHandle(prg);
	implementation<DisplayObjectFactory>()->Create(prg, &DisplayObjects::ShaderProgram::CreateParams_t(m_sCompletedSource, ""));

	LinkSymbols(prg);

	return prg;*/
}
