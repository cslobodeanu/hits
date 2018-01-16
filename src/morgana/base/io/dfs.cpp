#include "dfs.h"

using namespace MorganaEngine::Base::IO;

__implement_class(DiskFileSystem, GenericFileSystem);



DiskFileSystem::DiskFileSystem() : GenericFileSystem()
{

}

DiskFileSystem::~DiskFileSystem()
{

}

String DiskFileSystem::GetDescription()
{
	return "Disk File System";
}

bool DiskFileSystem::CanHandleProtocol( const char* p )
{
	return strcmp(p, "file") == 0;
}

Stream* DiskFileSystem::GetStream( const char* location )
{
	return new FileStream();
}
