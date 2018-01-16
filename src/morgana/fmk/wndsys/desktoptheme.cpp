#include "desktoptheme.h"
#include "meresources.h"

using namespace MorganaEngine::Framework::WindowSystem;

__implement_class(DesktopTheme, MEObject);

DesktopTheme::DesktopTheme()
{
	DoAddProperties();

	SetDefaults();
}

void DesktopTheme::SetDefaults()
{
	titleBkgColor = Color255(49, 49, 49);
	titleTextColor = Color255(245, 245, 245);
	windowBkgColor = Color255(255, 250, 244);

	titleFont = ResourceContext::global->Load<FontTrueType>("truetype/segoeui.ttf");
	symbolsFont = ResourceContext::global->Load<FontTrueType>("truetype/webdings.ttf");
	titleFont->Append(symbolsFont);

	titleFontSize = 10;

	contentFont = ResourceContext::global->Load<FontTrueType>("truetype/segoeui.ttf");
	contentFontSize = 12;
	contentColor = Color255(30, 30, 30);
}

void DesktopTheme::DoAddProperties()
{
	super::DoAddProperties();
}
