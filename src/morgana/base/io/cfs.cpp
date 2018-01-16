#include "cfs.h"

using namespace MorganaEngine::Base::IO;

__implement_class(CloudFileSystem, GenericFileSystem);

CloudFileSystem::CloudFileSystem() : GenericFileSystem()
{

}

CloudFileSystem::~CloudFileSystem()
{

}

String CloudFileSystem::GetDescription()
{
	return "Cloud File System";
}

bool CloudFileSystem::CanHandleProtocol( const char* p )
{
	return strcmp(p, "cloud") == 0;
}

Stream* CloudFileSystem::GetStream( const char* location )
{
	return NULL;
}
