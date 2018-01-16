#ifndef __MORGANA_BASE_IO_PATHS_H__
#define __MORGANA_BASE_IO_PATHS_H__

#include "../types/mestring.h"
#include "../types/mearray.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace IO
		{
			class Paths
			{
				static Types::String downloadsPath;
				static Types::String documentsPath;
				static Types::String systemFilesPath;
			public:
				static Types::String	NormalizePath(const char* path);
				static Types::String	ConcatPathWithFilename(const char* path, const char* filename);
				static Types::String	GetFileName(const char* path);
				static Types::String	GetExtension(const char* path);
				static bool				HasExtension(const char* path);
				static bool				HasFolder(const char* path);
				static Types::String	GetFolder(const char* path);
				static Types::String	RemoveExtension(const char* path);
				static Types::String	ReplaceExtension(const char* path, const char* new_ext);
				static Types::String	GetProtocol(const char* path);
				static Types::String	RemoveProtocolPrefix(const char* path);
				static Types::String	GetDownloadsPath();
				static Types::String	GetDocumentsPath();
				static Types::String	GetSystemFilesPath();
				static bool				CreatePath(const char* path);
				static Types::String	ConcatenatePaths( const char* path1, const char* path2 );
				static Types::String	GetPathInDocuments( const char* path );

				static Types::String	GetPathForSameFolder(const char* basePath, const char* fileName);

				static void				Split(const char* pathsArray, Types::Array<Types::String>& out);
				static Types::Array<Types::String> Split(const char* pathsArray);
			};
		}
	}
}

#endif