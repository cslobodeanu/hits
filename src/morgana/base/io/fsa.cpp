#include "fsa.h"

using namespace MorganaEngine::Base::IO;
using namespace MorganaEngine::Base::Types;

__implement_class_abstract(GenericFileSystem, MEObject);

GenericFileSystem::GenericFileSystem() : MEObject()
{

}

GenericFileSystem::~GenericFileSystem()
{

}

String GenericFileSystem::GetDescription()
{
	return "Generic File System";
}

void GenericFileSystem::OnStreamDeleted( Stream* s )
{

}

void GenericFileSystem::OnStreamRetrieved( Stream* s )
{

}

void GenericFileSystem::OnStreamClosed( Stream* s )
{

}
