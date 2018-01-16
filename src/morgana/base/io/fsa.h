#ifndef __MORGANA_BASE_IO_FSA_H__
#define __MORGANA_BASE_IO_FSA_H__

#include "reflection.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace IO
		{
			class GenericFileSystem : public MEObject
			{
				__declare_class_abstract(GenericFileSystem, MEObject);

				Array<Stream*>	m_pFileList;

			public:

				GenericFileSystem();
				virtual ~GenericFileSystem();

				virtual String	GetDescription();

				virtual bool	CanHandleProtocol(const char* p) = 0;
				virtual Stream*	GetStream(const char* location) = 0;

				friend class MFiles;

			protected:
				virtual void	OnStreamRetrieved(Stream* s);
				virtual void	OnStreamDeleted(Stream* s);
				virtual void	OnStreamClosed(Stream* s);
			};
		}
	}
}

#endif