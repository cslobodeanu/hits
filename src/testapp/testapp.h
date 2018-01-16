#ifndef __TEST_APPLICATION_H__
#define __TEST_APPLICATION_H__

#include "framework.h"

class TestApplication : public MEApplication
{
	__declare_class(TestApplication, MEApplication);
public:

	TestApplication();
	virtual ~TestApplication();

	virtual void Initialize();

protected:

	virtual void RunFrame();
	// for test
	void		 OnTimer(METimer* t, const float);

	void		 OnImageClick(Component* sender, Vector2 p, bool& ret);
};

#endif