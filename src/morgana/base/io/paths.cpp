#include "paths.h"
#include "platform.h"
#include "debug.h"

#ifdef OS_WINDOWS_WIN32
#include <direct.h>
#endif

using namespace MorganaEngine::Base::IO;
using namespace MorganaEngine::Base::Types;

String Paths::downloadsPath;
String Paths::documentsPath;
String Paths::systemFilesPath;

String Paths::NormalizePath( const char* path )
{
	String p = ConcatenatePaths(path, "");
	return p;
}

String Paths::ConcatPathWithFilename( const char* path, const char* filename )
{
	String p = NormalizePath(path) + "/";
	return p + String(filename);
}

String Paths::GetFileName( const char* path )
{
	String p = NormalizePath(path);
	int last_slash = -1;
	int index = p.Length() - 1;
	while(true)
	{
		if(p[index] == '/')
		{
			last_slash = index;
			break;
		}
		index--;
		if(index < 0)
			break;
	}

	if(last_slash == -1)
		return p;

	p.Delete(last_slash + 1);
	return p;
}

String Paths::GetExtension( const char* path )
{
	String p = NormalizePath(path);
	int last_dot = -1;
	int index = p.Length() - 1;
	while(true)
	{
		if(p[index] == '.')
		{
			last_dot = index;
			break;
		}
		index--;
		if(index < 0)
			break;
	}

	if(last_dot == -1)
		return String("");

	p.Delete(last_dot + 1);
	return p;
}

String Paths::GetFolder( const char* path )
{
	String p = NormalizePath(path);
	int last_slash = -1;
	int index = p.Length() - 1;
	while(true)
	{
		if(p[index] == '/')
		{
			last_slash = index;
			break;
		}
		index--;
		if(index < 0)
			break;
	}

	if(last_slash == -1)
		return String("");

	p[last_slash + 1] = 0;

	String prot = GetProtocol(p);
	prot += String(":/");
	if(p.Find(prot) == 0)
		p.Delete(prot.Length());

	return p;
}

String Paths::RemoveExtension( const char* path )
{
	String p = GetFileName(path);

	int last_dot = -1;
	int index = p.Length() - 1;
	while(true)
	{
		if(p[index] == '.')
		{
			last_dot = index;
			break;
		}
		index--;
		if(index < 0)
			break;
	}

	if(last_dot == -1)
		return p;

	p[last_dot] = 0;
	return p;

}


String Paths::ReplaceExtension( const char* path, const char* new_ext )
{
	String p = RemoveExtension(path);
	if(new_ext[0] == '.')
		p += String(new_ext);
	else
		p += String(".") + String(new_ext);
	return p;
}

String Paths::GetProtocol( const char* path )
{
	String p = NormalizePath(path);
	const int index = p.Find(":/");
	if(index < 2)
		return String("file");

	p[index] = 0;
	return p;
}

String Paths::GetDownloadsPath()
{
	if(downloadsPath == "")
	{
#ifdef OS_WINDOWS
		downloadsPath = "data/downloads/";
		CreatePath(downloadsPath);
#endif
	}
	return downloadsPath;
}

String Paths::GetDocumentsPath()
{
	if(documentsPath == "")
	{
#ifdef OS_WINDOWS
		documentsPath = "data/documents/";
		CreatePath(documentsPath);
#endif
	}
	return documentsPath;
}

String Paths::GetSystemFilesPath()
{
	if(systemFilesPath == "")
	{
#ifdef OS_WINDOWS
		systemFilesPath = "data/documents/system/";
		CreatePath(systemFilesPath);
#endif
	}
	return systemFilesPath;
}

bool MorganaEngine::Base::IO::Paths::CreatePath( const char* path )
{
#ifdef OS_WINDOWS_WIN32
	char _full_path[512];
	snprintf(_full_path, 512, "%s", path);
	char* start = _full_path;
	char* n = start;
	char* end = _full_path+strlen(_full_path);
	while(n!=end){
		if(*n=='/'){
			*n = '\0';
			_mkdir(start);
			*n = '/';
		}
		++n;
	}

	return true;

#endif

	return false;
}

Array<String> Paths::Split(const char* pathsArray)
{
	Array<String> out;
	String::Split(pathsArray, ";", out);
	return out;
}

void Paths::Split(const char* pathsArray, Array<String>& out)
{
	String::Split(pathsArray, ";", out);
}

bool Paths::HasFolder(const char* path)
{
	return strstr(path, "\\") != NULL || strstr(path, "/") != NULL;
}

String Paths::GetPathInDocuments(const char* path)
{
	GetDocumentsPath();
	return ConcatenatePaths(documentsPath, path);
}

String Paths::GetPathForSameFolder(const char* basePath, const char* fileName)
{
	String folder = GetFolder(basePath);
	return ConcatPathWithFilename(folder, fileName);
}

String Paths::RemoveProtocolPrefix( const char* path )
{
	String p = String(path);
	const int index = p.Find("://");
	if(index > 0)
		p.Delete(index + 3);
	return p;
}

String Paths::ConcatenatePaths( const char* path1, const char* path2 )
{
	String p1(path1);
	String p2(path2);

	Array<String> folders1;
	p1.Split("\\/", folders1);

	Array<String> folders2;
	p2.Split("\\/", folders2);

	folders1.Append(folders2.ptr(), folders2.Length());

	const bool hasDriveName = folders1.First().Find(':') > 0;

	while(true)
	{
		bool resolvingDots = false;
		for(int i = 0; i < folders1.Length(); i++)
		{
			if(folders1[i].Length() == 0) continue;
			if(folders1[i].Equals("."))
			{
				for(int k = hasDriveName ? 1 : 0; k < i + 1; k++)
					folders1[k] = "";
				resolvingDots = true;
				break;
			}

			if(folders1[i].Equals(".."))
			{
				if(i > 0)
				{
					folders1[i - 1] = "";
					folders1[i] = "";
					resolvingDots = true;
				}
			}
		}

		if(resolvingDots == false)
			break;
	}

	String p;

	for(int i = 0; i < folders1.Length(); i++)
	{
		if(folders1[i].Length() == 0) continue;
		p += folders1[i];
		p += "/";
	}

	const int lastIndex = p.Length() - 1;
	if(p[lastIndex] == '/')
		p[lastIndex] = 0;

	return p;
}

bool Paths::HasExtension( const char* path )
{
	for(int i = strlen(path) - 1; i >= 0; i--)
	{
		if(path[i] == '/') return false;
		if(path[i] == '\\') return false;
		if(path[i] == '.')
		{
			return true;
		}
	}

	return false;
}