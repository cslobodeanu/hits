#include "nfs.h"

using namespace MorganaEngine::Base::IO;

__implement_class(NetworkFileSystem, GenericFileSystem);

NetworkFileSystem::NetworkFileSystem() : GenericFileSystem()
{

}

NetworkFileSystem::~NetworkFileSystem()
{

}

String NetworkFileSystem::GetDescription()
{
	return "Network File System";
}

bool NetworkFileSystem::CanHandleProtocol( const char* p )
{
	return strcmp(p, "net") == 0 || strcmp(p, "network") == 0;
}

Stream* NetworkFileSystem::GetStream( const char* location )
{
	return NULL;
}
