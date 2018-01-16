#include "shadowmappingbackend.h"
#include "mescene.h"

using namespace MorganaEngine::Framework::RenderPaths;

__implement_class(ShadowMappingBackend, CameraBackend);


ShadowMappingBackend::ShadowMappingBackend()
{
	writeDepthMaterial = NULL;

	firstSplitDistance = 0.16f;
	secondSplitDistance = 0.35f;

	DoAddProperties();
}

ShadowMappingBackend::~ShadowMappingBackend()
{

}

void ShadowMappingBackend::DoAddProperties()
{
	super::DoAddProperties();

	__add_property1(firstSplitDistance);
	__add_property1(secondSplitDistance);
}

void ShadowMappingBackend::OnPropertySet(_propertyDef* atr)
{
	super::OnPropertySet(atr);

	if (atr == &firstSplitDistance || atr == &secondSplitDistance)
	{
		GenerateSplitPlanes();
	}
}

void ShadowMappingBackend::Start()
{
	super::Start();

	writeDepthMaterial = ResourceContext::global->Load<Material>("depthonly");

	camera->transform->position = Vector3::Zero;
	camera->nearPlane = 0.1f;
	camera->farPlane = 5000.0f;
	camera->clearFlags = Camera::ClearDepth;
	camera->orthographicSize = 1600.0f;
	camera->projectionType = Camera::Parallel;

	GenerateSplitPlanes();
	GenerateSplitCoords();

	const int resolution = 2048;
	RenderTarget::CreateParams_t cp;
	cp.width = (resolution * 3) / 2;
	cp.height = resolution;
	cp.attachTexturesBits = MDISPLAY_BUFFER_DEPTH;
	cp.createBuffersBits = MDISPLAY_BUFFER_DEPTH;

	camera->target = new RenderTarget();
	implementation<DisplayObjectFactory>()->Create(camera->target, &cp);
}

void ShadowMappingBackend::OnPreFlush(const CameraFrontend::RenderableFrame_t& frame)
{
	Vector3 lightDir = frame.globalDirectionalLight->transform->rotation->Forward();
	camera->transform->rotation = frame.globalDirectionalLight->transform->rotation;
	camera->transform->position = -camera->transform->rotation->Forward() * 1200.0f;// camera->farPlane * 0.5f;

	if (true)
	{
		// ONLY WHEN CHANGED
		UpdateSplitFrustums();
	}

	UpdateRenderersInSplits();
}

void ShadowMappingBackend::OnFlush(const CameraFrontend::RenderableFrame_t& frame)
{
	Render();
}

void ShadowMappingBackend::UploadGlobalShaderData(ShaderProgram* shader)
{
	shader->SetMatrices("shadowMat", splitProjectionViewMatrices, 3);

	shader->SetFloats("splitPlanes", splitPlanes, 3);

	shader->SetVectors("splitViewports", splitViewports, 3);

	if (shader->HasParameter("poissonDisk"))
	{
		static const Vector2 vec2[4] = {
			Vector2(-0.94201624f, -0.39906216f),
			Vector2(0.94558609f, -0.76890725f),
			Vector2(-0.094184101f, -0.92938870f),
			Vector2(0.34495938f, 0.29387760f)
		};
		shader->SetVectors("poissonDisk", vec2, 4);
	}
}

const bool ShadowMappingBackend::IsUsingShader(ShaderProgram* shader) const
{
	return shader->tags.Contains("shadow mapping") || shader->tags.Contains("csm");
}

void ShadowMappingBackend::UpdateSplitFrustums()
{
	// ONLY WHEN CHANGED
	const CameraFrontend::RenderableFrame_t& frame = frontend->GetFrame();

	Matrix view = camera->frustum.View();
	view.Translation() = Vector3::Zero;

	Frustum tmpFrustum;

	for (int split = 0; split < 3; split++)
	{
		// CACHE THIS !!!

		const float nearP = (split == 0) ? (float)(dependsOnCamera->nearPlane) : splitPlanes[split - 1];
		Matrix splitProj = dependsOnCamera->GetProjectionForNearFar(nearP, splitPlanes[split]);

		tmpFrustum.Update(&splitProj, dependsOnCamera->frustum.ViewPtr());

		float maxZ = Mathf::MinFloat;
		float minZ = -maxZ;
		float tmp;
		for (Vector3* v = tmpFrustum.corners; v < tmpFrustum.corners + 8; v++)
		{
			Vector3::TransformOnlyZ(v, &view, &tmp);
			maxZ = Mathf::Max(maxZ, tmp);
			minZ = Mathf::Min(minZ, tmp);
		}

		maxZ += splitPlanes[split] * 0.5f;
		minZ -= splitPlanes[split] * 0.5f;

		Matrix proj = Matrix::CreateOrthographic(-1.0f, 1.0f, -1.0f, 1.0f, minZ, maxZ);

		const Matrix cropMat = ComputeCropMatrix(tmpFrustum, proj, view);
		proj = cropMat * proj;

		splitFrustums[split].Update(&proj, &view);

		static Matrix bias(0.5f, 0.0f, 0.0f, 0.5f,
			0.0f, 0.5f, 0.0f, 0.5f,
			0.0f, 0.0f, 0.5f, 0.5f,
			0.5f, 0.5f, 0.5f, 1.0f);

		Matrix::Multiply(&bias, splitFrustums[split].ProjectionViewPtr(), &splitProjectionViewMatrices[split]);
	}
}

void ShadowMappingBackend::UpdateRenderersInSplits()
{
	const CameraFrontend::RenderableFrame_t& frame = frontend->GetFrame();

	for (int i = 0; i < 3; i++)
		renderersInSplits[i].Clear();

	for (int split = 0; split < 3; split++)
	{
		for (int i = 0; i < frame.renderQueue[Renderer::Opaque].Length(); i++)
		{
			if (splitFrustums[split].Contains(&frame.renderQueue[Renderer::Opaque][i]->bounds))
			{
				renderersInSplits[split].Add(frame.renderQueue[Renderer::Opaque][i]);
			}
		}
	}
}

void ShadowMappingBackend::GenerateSplitPlanes()
{
	splitPlanes[0] = dependsOnCamera->farPlane * firstSplitDistance;
	splitPlanes[1] = dependsOnCamera->farPlane * secondSplitDistance;
	splitPlanes[2] = dependsOnCamera->farPlane;
}

void ShadowMappingBackend::GenerateSplitCoords()
{
	Vector2 ofs = Vector2::Zero;
	float scale = 2.0f / 3.0f;

	splitViewports[0] = Vector3(ofs, scale);

	ofs.x += 1.0f * scale;
	scale *= 0.5f;
	splitViewports[1] = Vector3(ofs, scale);


	ofs.y += scale;
	scale *= 0.5f;

	splitViewports[2] = Vector3(ofs, scale);
}

Matrix ShadowMappingBackend::ComputeCropMatrix(const Frustum& splitFrustum, const Matrix& lightProj, const Matrix& lightView)
{
	// Find boundaries in light's clip space  

	const Matrix lightProjView = lightProj * lightView;

	Vector3 cropMin = Vector3::One * Mathf::MaxFloat;
	Vector3 cropMax = -cropMin;
	Vector3 tmp;
	for (int i = 0; i < 8; i++)
	{
		Vector3::TransformPerspectiveDivide(&splitFrustum.corners[i], &lightProjView, &tmp);
		cropMin = Vector3::Min(cropMin, tmp);
		cropMax = Vector3::Max(cropMax, tmp);
	}

#if 0
	//Fix Shadow Jittering
	if (true)
	{
		// I borrowed this code from some forum that I don't remember anymore =/
		// We snap the camera to 1 pixel increments so that moving the camera does not cause the shadows to jitter.
		// This is a matter of integer dividing by the world space size of a texel
		// Without this, the shadow map isn't big enough in the world.
		const float diagonalLength = Vector3::Distance(&f.point[0], &f.point[6]) + 2.f;
		const float worldsUnitsPerTexel = diagonalLength / float(Param.CascadeResolution);
		const Vector3 vBorderOffset = (vMin - vMax + diagonalLength) * 0.5f;

		vMax += vBorderOffset;
		vMin -= vBorderOffset;

		vMin /= worldsUnitsPerTexel;
		vMin.x = Mathf::Floor(vMin.x);
		vMin.y = Mathf::Floor(vMin.y);
		vMin.z = Mathf::Floor(vMin.z);
		vMin *= worldsUnitsPerTexel;

		vMax /= worldsUnitsPerTexel;
		vMax.x = Mathf::Floor(vMax.x);
		vMax.y = Mathf::Floor(vMax.y);
		vMax.z = Mathf::Floor(vMax.z);
		vMax *= worldsUnitsPerTexel;
	}
#endif

	// Use default near-plane value  
	cropMin.z = 0.0f;
	// Create the crop matrix  
	float scaleX, scaleY;
	float offsetX, offsetY;
	scaleX = 2.0f / (cropMax.x - cropMin.x);
	scaleY = 2.0f / (cropMax.y - cropMin.y);
	offsetX = -0.5f * (cropMax.x + cropMin.x) * scaleX;
	offsetY = -0.5f * (cropMax.y + cropMin.y) * scaleY;
	return Matrix(scaleX, 0.0f, 0.0f, offsetX,
		0.0f, scaleY, 0.0f, offsetY,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

void ShadowMappingBackend::Update()
{
	super::Update();
}

void ShadowMappingBackend::Render()
{
	const int rbw = camera->target->GetBounds().width;
	const int rbh = camera->target->GetBounds().height;

	RenderPath::SetMaterial(writeDepthMaterial);

	const int worldMatIndex = writeDepthMaterial->shader->GetParameterIndex("worldMat");

	for (int split = 0; split < 3; split++)
	{
		if (renderersInSplits[split].Length() == 0) continue;

		Rect vp;
		vp.x = int((float)splitViewports[split].x * rbw);
		vp.y = int((float)splitViewports[split].y * rbh);
		vp.width = int((float)splitViewports[split].z * rbw);
		vp.height = vp.width;

		DisplayDevice::main->SetViewport(vp);

		writeDepthMaterial->shader->SetMatrix("pvwMat", splitFrustums[split].ProjectionView());

		for (Renderer** ptr = renderersInSplits[split].ptr(); ptr < renderersInSplits[split].end(); ptr++)
		{
			Renderer* renderer = *ptr;
			if (renderer->GetDrawablesCount() == 0) continue;

			for (int k = 0; k < renderer->GetDrawablesCount(); k++)
			{
				const Renderer::Drawable_t& r = renderer->GetDrawable(k);

				writeDepthMaterial->shader->SetMatrix(worldMatIndex, renderer->transform->GetWorldMatrix());

				RenderPath::DrawIndexed(r);
			}
		}
	}
}
