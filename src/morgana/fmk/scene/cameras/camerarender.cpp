#include "camerarender.h"
#include "framework.h"

using namespace MorganaEngine::Framework::Scene::Cameras;

Array<Camera*>				CameraRender::activeCameras;
Array<CameraFrontend*>		CameraRender::activeCamerasFrontends;
Array<CameraBackend*>		CameraRender::activeCamerasBackends;
Array<bool>					CameraRender::activeCamerasReady;
Array<CanvasList>			CameraRender::canvases;
CanvasList					CameraRender::overlayCanvases;


void CameraRender::ProcessNode(GameObject* node)
{
	if (node->IsEnabled() == false) return;

	static Array<bool> activeCamerasPrevState;
	activeCamerasPrevState.Copy(activeCamerasReady);

	for (int i = 0; i < activeCameras.Length(); i++)
	{
		Camera* c = activeCameras[i];
		if (c->nodeToRender == NULL)
			c->nodeToRender = GameObject::root;

		if (c->nodeToRender == node)
			activeCamerasReady[i] = true;

		if (activeCamerasReady[i] && c->renderMask.Contains(node->layer) == false)
			activeCamerasReady[i] = false;
	}

	bool thereAreCamerasRendering = false;
	for (int i = 0; i < activeCameras.Length(); i++)
	{
		Camera* c = activeCameras[i];
		if (activeCamerasReady[i] == false) continue;

		CameraFrontend* frontend = activeCamerasFrontends[i];

		thereAreCamerasRendering = true;

		if (frontend != NULL)
		{
			Renderer* r = node->GetComponent<Renderer>();
			if (r != null)
			{
				frontend->PushRenderer(r);
				continue;
			}

			Light* l = node->GetComponent<Light>();
			if (l != null)
			{
				frontend->PushLight(l);
				continue;
			}
		}

		CanvasSurface* cs = node->GetComponent<CanvasSurface>();
		if (cs != null)
		{
			if (cs->mode == CanvasSurface::Overlay)
				overlayCanvases.Add(cs);
			else
			{
				int index = 0;
				if (cs->renderCamera != NULL)
					index = activeCameras.Find(cs->renderCamera);
				canvases[index].Add(cs);
			}
		}
	}

	if (thereAreCamerasRendering)
	{
		for (int i = 0; i < node->GetChildrenCount(); i++)
		{
			ProcessNode(node->GetChild(i));
		}
	}

	activeCamerasReady.Copy(activeCamerasPrevState);
}

void CameraRender::CollectRenderData()
{
	ProcessNode(GameObject::root);
}

void CameraRender::SendRenderData()
{
	for (int i = 0; i < activeCameras.Length(); i++)
	{
		if (activeCamerasReady[i])
		{
			activeCameras[i]->SetupBackground();

			if (activeCamerasBackends[i] != NULL)
			{
				CameraBackend* backend = activeCamerasBackends[i];
				backend->Flush();
			}

			for (int c = 0; c < canvases[i].Length(); c++)
			{
				CanvasSurface* cs = canvases[i][c];
				RenderCanvas(cs);
			}
		}
	}

	for (int i = 0; i < overlayCanvases.Length(); i++)
	{
		RenderCanvas(overlayCanvases[i]);
	}
}

void CameraRender::RenderCanvas(CanvasSurface* cs)
{
	Array<Renderer::Drawable_t>& drawables = cs->batches.BuildDrawablesList();
	Array<const Material*>& materials = cs->batches.GetMaterialList();
	Array<const Matrix*>& worldMatrices = cs->batches.GetWorldMatricesList();

	Matrix pvwMat, tmp;
	Matrix::Multiply(&cs->projectionMatrix, &cs->worldMatrix, &pvwMat);

	for (int i = 0; i < drawables.Length(); i++)
	{
		Renderer::Drawable_t& d = drawables[i];
		const Material* m = materials[i];

		ShaderProgram* shader = RenderPath::SetProgram(m->shader);

		if (worldMatrices[i] != NULL)
		{
			Matrix::Multiply(&pvwMat, worldMatrices[i], &tmp);
			shader->SetMatrix("pvwMat", tmp);
		}
		else
		{
			shader->SetMatrix("pvwMat", pvwMat);
		}

		RenderPath::SetMaterial(m);

		RenderPath::DrawIndexed(d);
	}

	FrameStats::current.numCanvasBatches += drawables.Length();
}

int SortByDepthFlag(Camera*& c1, Camera*& c2)
{
	if (c1->depth > c2->depth) return 1;
	if (c1->depth < c2->depth) return -1;

	return 0;
}

void CameraRender::RenderAllCameras()
{
	for (Camera **c = activeCameras.ptr(), **end = activeCameras.end(); c < end; c++)
	{
		(*c)->CreateRenderPath();
	}

	activeCameras.Sort(SortByDepthFlag);

	const int camCount = activeCameras.Length();
	activeCamerasFrontends.SetLength(camCount);
	activeCamerasBackends.SetLength(camCount);
	for (int i = 0; i < camCount; i++)
	{
		activeCamerasFrontends[i] = activeCameras[i]->GetComponent<CameraFrontend>();
		activeCamerasBackends[i] = activeCameras[i]->GetComponent<CameraBackend>();
	}

	ResetRenderData();
	CollectRenderData();
	SendRenderData();

	activeCameras.Clear();
	activeCamerasReady.Clear();
}

void CameraRender::ResetRenderData()
{
	activeCamerasReady.SetLength(activeCameras.Length());

	canvases.SetLength(activeCameras.Length());
	for (int i = 0; i < canvases.Length(); i++)
		canvases[i].Clear();

	for (bool *b = activeCamerasReady.ptr(), *end = activeCamerasReady.end(); b < end; *b = false, b++);

	for (int i = 0; i < canvases.Length(); i++)
		canvases[i].Clear();
	canvases.Clear();
	overlayCanvases.Clear();
}