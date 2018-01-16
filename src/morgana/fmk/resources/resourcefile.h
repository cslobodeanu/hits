#ifndef __MORGANA_FMK_RESOURCES_RESOURCE_FILE_H__
#define __MORGANA_FMK_RESOURCES_RESOURCE_FILE_H__

#include "reflection.h"
#include "meio.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Resources
		{
			class ResourceFile : public MEObject, public IsNamed, public IsVersioned
			{
				__declare_class_abstract(ResourceFile, MEObject);

			public:

				static ResourceFile*	Translate(CLASSDESC classDesc, const char* paths, const char* args = NULL);
				static ResourceFile*	Translate(CLASSDESC classDesc, const StreamList& streams, const char* args = NULL);

				template <class Type>
				static Type*			Translate(const char* paths, const char* args = NULL);

				template <class Type>
				static Type*			Translate(const StreamList& streams, const char* args = NULL);

				template <class Type>
				static Type*			Translate(Stream* stream, const char* args = NULL);


				static void				Save(ResourceFile* res, const char* path);

				const String			GetFilePath() const;
				const String			GetFilePaths() const;

				virtual void			Release();

				const bool				ConversionWasOk() { return conversionWasOk; }

			protected:

				ResourceFile();
				virtual ~ResourceFile();

				Array<StringHash>	filenames;
				StringHash			createArgs;

				virtual const bool	SaveToStream(Stream* s);
				virtual void		ImportFromStreams(const StreamList&  streams, const char* args) = 0;

				void				FixPath(String& s);

				bool				conversionWasOk;

				void				SetNameFromPath(const StreamList&  streams);

			private:
				static Stream*		GetResourceStream(const char* path);
			};

			template <class Type>
			Type* MorganaEngine::Framework::Resources::ResourceFile::Translate(Stream* stream, const char* args /*= NULL*/)
			{
				StreamList streams;
				streams.Add(stream);
				return (T*)Translate(T::AsClassDesc, streams, args);
			}

			template <class Type>
			Type* ResourceFile::Translate(const StreamList& streams, const char* args /*= NULL*/)
			{
				return (T*)Translate(T::AsClassDesc, streams, args);
			}

			template <class T>
			T* ResourceFile::Translate(const char* paths, const char* args /*= NULL*/)
			{
				return (T*)Translate(T::AsClassDesc, paths, args);
			}

		}
	}
}

#endif