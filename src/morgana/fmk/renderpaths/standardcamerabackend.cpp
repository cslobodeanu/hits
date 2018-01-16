#include "standardcamerabackend.h"
#include "camera.h"
#include "renderpath.h"
#include "mescene.h"

using namespace MorganaEngine::Framework::RenderPaths;

__implement_class(StandardCameraBackend, CameraBackend);

StandardCameraBackend::StandardCameraBackend()
{

}

void StandardCameraBackend::OnFlush(const CameraFrontend::RenderableFrame_t& frame)
{
	Camera* shadowCamera = Camera::GetSystemCamera(Camera::SystemCameraTypes::LightsDepthBuffer);
	if (shadowCamera != NULL)
	{
		Texture* depth = (Texture*)shadowCamera->target->GetDepthTexture();
		if (depth == NULL)
			return;
		DisplayDevice::main->BindObject(depth, 5);
	}

	for (int order = Renderer::FirstInQueue; order < Renderer::LastInQueue; order++)
	{
		RendererList& list = frame.renderQueue[order];
		if (list.IsEmpty()) continue;

		for (int i = 0; i < list.Length(); i++)
		{
			Renderer* renderer = list[i];
			if (renderer->GetDrawablesCount() == 0) continue;

			const Material* mat = renderer->readMaterial();

			if (mat == NULL)
				continue;

			for (int k = 0; k < renderer->GetDrawablesCount(); k++)
			{
				const Renderer::Drawable_t& r = renderer->GetDrawable(k);
				const Matrix* customPVMat = renderer->GetCustomProjectionView();
				const Matrix* pvMat = customPVMat ? customPVMat : camera->frustum.ProjectionViewPtr();

				RenderPath::SetMaterial(mat);

				ShaderProgram* prg = RenderPath::SetProgram(mat->shader);

				prg->SetMatrix("pvwMat", *pvMat);
				prg->SetMatrix("worldMat", renderer->transform->GetWorldMatrix());

				RenderPath::DrawIndexed(r);
			}
		}
	}
}

void StandardCameraBackend::UploadGlobalShaderData(ShaderProgram* shader)
{
	const CameraFrontend::RenderableFrame_t& frame = frontend->GetFrame();

	shader->SetColor("ambientLightColor", frame.globalAmbientLight ? frame.globalAmbientLight->color : Color::black);
	shader->SetColor("dirLightColor", frame.globalDirectionalLight ? frame.globalDirectionalLight->color : Color::black);
	shader->SetVector("dirLightDirWS", frame.globalDirectionalLight ? frame.globalDirectionalLight->transform->rotation->Forward() : Vector3::UnitY);

	shader->SetVector("globalColor", Color::white);

	shader->SetMatrix("viewMat", camera->frustum.View());

	shader->SetVector("camDir", camera->transform->rotation->Forward());
}

const bool StandardCameraBackend::IsUsingShader(ShaderProgram* shader) const
{
	return shader->tags.Contains("directional lights");
}
