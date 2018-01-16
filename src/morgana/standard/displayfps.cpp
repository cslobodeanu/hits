#include "displayfps.h"
#include "objectbuilder.h"

using namespace MorganaEngine::Standard;

__implement_morgana_script1(DisplayFPS);

DisplayFPS::DisplayFPS()
{
	cumulatedFrameTime = -1.0f;
	averageFPS = 0.0f;

	for (int i = 0; i < 10; i++)
		caption[i] = NULL;
}

DisplayFPS::~DisplayFPS()
{

}

void DisplayFPS::OnResize()
{
	int cc = 6;

	float cw = Screen::width / (float)cc;
	float h = 16.0f;

	for (int i = 0; i < cc; i++)
	{
		RectTransform* rt = caption[i]->GetComponent<RectTransform>();
		rt->rect = Rectf((float)i * cw, 0.0f, cw - 5.0f, h);
	}
}

void DisplayFPS::Start()
{
	myRectTransform = gameObject->AddComponent<RectTransform>();

	float border = 0.0f;
	float h = 29.0f;

	myRectTransform->anchorMax = Vector2(1.0f, 0.0f);
	myRectTransform->offsetMin = Vector2(border, border);
	myRectTransform->offsetMax = Vector2(border, h + border);
	myRectTransform->OnResize += Callback(OnResize);

	GameObject* gradient = ObjectBuilder::CreateCanvasImage(this, Texture::DefaultGradient, "Gradient");
	gradient->GetComponent<Image>()->color = Color(1.0f, 1.0f, 1.0f, 0.4f);
	gradient->GetComponent<Image>()->isMask = true;
	gradient->GetComponent<RectTransform>()->FillParent();
	
	GameObject* bkg = ObjectBuilder::CreateCanvasImage(gradient, Texture::DefaultWhite, "Bkg");
	bkg->GetComponent<Image>()->color = Color::light_gray;
	bkg->GetComponent<RectTransform>()->FillParent();

	int cc = 7;

	float borderX = 10.0f;
	float cw = (Screen::width - 2.0f * borderX) / (float)cc;

	FontTrueType* fnt = ResourceContext::global->Load<FontTrueType>("truetype/segoeui.ttf");

	for (int i = 0; i < cc; i++)
	{
		GameObject* obj = ObjectBuilder::CreateCanvasObject(this, String::Format("DisplayFps Text#%d", i + 1));
		caption[i] = obj->AddComponent<TextTrueType>();
		caption[i]->text = "*";// String::Format("Caption #%d", i + 1);
		caption[i]->font = fnt;
		caption[i]->fontSize = 10;
		caption[i]->color = Color::black;
		caption[i]->overflow = true;
		caption[i]->verticalAlignment = TextVerticalAlignment::AlignCenter;
		caption[i]->fontGlow = true;
		caption[i]->fontGlowColor = Color(0.0f, 0.0f, 0.0f, 0.55f);

		RectTransform* rt = obj->GetComponent<RectTransform>();
		rt->rect = Rectf((float)i * cw + borderX, 0.0f, cw - 5.0f, h);
	}
}

void DisplayFPS::Update()
{
	//gameObject->BringToFront();

	int frameInterval = 20;

	if ((Time::numFrames % frameInterval) == 0)
	{
		if (cumulatedFrameTime > 0.0f)
		{
			cumulatedFrameTime /= (float)frameInterval;
			averageFPS = 1.0f / cumulatedFrameTime;
		}

		cumulatedFrameTime = 0.0f;
	}

	if (cumulatedFrameTime >= 0.0f)
		cumulatedFrameTime += Time::deltaTime;

	caption[0]->text = String::Format("FPS: %.2f", averageFPS);
	caption[1]->text = String::Format("DrawCalls: %d", FrameStats::prev.numDrawCalls);
	caption[2]->text = String::Format("Triangles: %d", FrameStats::prev.numTriangles);
	caption[3]->text = String::Format("Renderables: %d", FrameStats::prev.numRenderables);
	caption[4]->text = String::Format("UpdatedObjects: %d", FrameStats::prev.numGameObjects);
	caption[5]->text = String::Format("CanvasBatches: %d", FrameStats::prev.numCanvasBatches);
	caption[6]->text = String::Format("CPU: %.2fMB GPU: %.2fMB", MemTracker::GetCPUMem() MegaBytes, MemTracker::GetGPUMem() MegaBytes);
}