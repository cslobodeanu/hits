#include "base.h"
#include "framework.h"
#include <crtdbg.h>

int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	if (MEObject::Instantiate(argv[1]) == NULL)
	{
		DEBUG_OUT("app class [%s] does not exist.", argv[1]);
		return -2;
	}

	MEApplication::main->mainView = new MEViewWin32();
	String cap = "morganaEngine: ";
	cap += argv[1];
	MEApplication::main->mainView->caption = cap;

	MEApplication::main->Initialize();

	DEBUG_OUT("Startup memory: %.2fMB allocated (%.2fMB peak)", __mt_mega(MemTracker::GetAllAllocatedMem()), __mt_mega(MemTracker::GetAllAllocatedMemPeak()));

	MEApplication::main->Run();
	MEApplication::main->Shutdown();


	return 0;
}