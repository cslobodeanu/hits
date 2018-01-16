#include "resourcefile.h"
#include "../../base/io/paths.h"
#include "resourcecontext.h"

using namespace MorganaEngine::Framework::Resources;
using namespace MorganaEngine::Base::IO;

__implement_class_abstract(ResourceFile, MEObject);

ResourceFile::ResourceFile()
{
}

ResourceFile::~ResourceFile()
{
	Release();
}

ResourceFile* ResourceFile::Translate(CLASSDESC classDesc, const char* multiplePaths, const char* args /*= NULL*/)
{
	Array<String> paths = Paths::Split(multiplePaths);

	if(paths.Length() == 0) return NULL;

	ResourceFile* rff	= (ResourceFile*)(MEObject::Instantiate(classDesc));

	StreamList streams;

	for(int i = 0; i < paths.Length(); i++)
	{
		String path = paths[i];
		rff->FixPath(path);
		rff->filenames.Add(StringHash(path));
		streams.Add(GetResourceStream(path));
	}

	rff->conversionWasOk = true;
	rff->ImportFromStreams(streams, args);

	for(int i = 0; i < streams.Length(); i++)
	{
		streams[i]->Close();
	}

	if(true)//rff->name.IsEmpty())
	{
		rff->name = Paths::GetFileName(paths.First());
		Paths::RemoveExtension(rff->name);
		rff->name.ToLower();
	}

	return rff;
}

ResourceFile* ResourceFile::Translate(CLASSDESC classDesc, const StreamList& streams, const char* args /*= NULL*/)
{
	ResourceFile* rff = (ResourceFile*)(MEObject::Instantiate(classDesc));
	rff->conversionWasOk = true;
	rff->ImportFromStreams(streams, args);

	for (int i = 0; i < streams.Length(); i++)
	{
		streams[i]->Close();
	}

	return rff;
}

void ResourceFile::Save(ResourceFile* res, const char* path)
{
	res->SaveToStream(GetResourceStream(path));
}

const String ResourceFile::GetFilePath() const
{
	return filenames.First();
}

const String ResourceFile::GetFilePaths() const
{
	String p = "";
	for (int i = 0; i < filenames.Length(); i++)
	{
		p += filenames[i];
		p += ";";
	}
	return p;
}

void ResourceFile::Release()
{

}

const bool ResourceFile::SaveToStream(Stream* s)
{
	return false;
}

Stream* ResourceFile::GetResourceStream(const char* path)
{
	String fullPath = Paths::GetPathInDocuments(path);
	Paths::CreatePath(fullPath);
	return singleton<MFiles>()->GetStream(fullPath);
}

void ResourceFile::FixPath(String& s)
{
	String p = ResourceContext::global->GetPreferredPath(this);
	if (p.IsEmpty() == false)
	{
		if (s.Find(p) != 0)
			s = Paths::ConcatPathWithFilename(p, s);
	}

	String ext = ResourceContext::global->GetPreferredExtension(this);
	if (ext.IsEmpty() == false && Paths::HasExtension(s) == false)
	{
		if (ext.First() != '.')
			s += ".";

		s += ext;
	}
}

void ResourceFile::SetNameFromPath(const StreamList&  streams)
{
	if (streams.Length())
	{
		name = Paths::GetFileName(streams.First()->location);
		name = Paths::RemoveExtension(name);
		name.ToLower();
	}
}