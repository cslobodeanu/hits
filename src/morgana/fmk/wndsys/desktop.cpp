#include "desktop.h"
#include "desktoptheme.h"

using namespace MorganaEngine::Framework::WindowSystem;

__implement_class(Desktop, Widget);

Desktop* Desktop::main = NULL;

Desktop::Desktop()
{
	if (main == NULL)
		main = this;

	theme = NULL;
}

Desktop::~Desktop()
{
	SAFE_DEL(theme);

	if (main == this)
	{
		main = NULL;
	}
}

void Desktop::Start()
{
	super::Start();
}


void Desktop::Update()
{
	super::Update();

	if (canvas != NULL)
	{
		RectTransform* canvasRT = canvas->GetComponent<RectTransform>();
		rectTransform->rect = canvasRT->rect;
	}
}

void Desktop::Awake()
{
	super::Awake();
	rectTransform = GetComponent<RectTransform>();

	theme = new DesktopTheme();

	gameObject->SetName("DesktopMain");
}

void Desktop::OnParentAttached()
{
	if (gameObject->GetParent() != NULL)
		canvas = gameObject->GetParent()->GetComponent<CanvasSurface>();
	else
		canvas = NULL;
}
