#ifndef __MORGANA_FMK_CANVAS_CANVAS_SURFACE_H__
#define __MORGANA_FMK_CANVAS_CANVAS_SURFACE_H__

#include "reflection.h"
#include "gameobjects.h"
#include "../renderers/renderbatches.h"
#include "../scene/cameras/camera.h"

namespace MorganaEngine
{
	namespace Framework
	{
		using namespace Renderers;
		class Scene::Cameras::Camera;

		namespace Canvas
		{
			class Image;
			class CanvasSurface : public Component, public IRenderable
			{
				__declare_component(CanvasSurface, Component);
				__declare_interfaces1(IRenderable);

			public:
				CanvasSurface();
				virtual ~CanvasSurface();

				enum CanvasMode
				{
					Overlay = 0,
					Camera,
					World
				};

				_property<int>								mode;
				_property_object<Scene::Cameras::Camera>	renderCamera;

				virtual void Awake();

				typedef VertexPosColorColor2TexTex2 VertexFormat;

			protected:
				friend class	RendersToCanvas;
				friend class	CameraRender;

				static const int maxQuadsInBatch = 2048;
				typedef RenderBatches<VertexFormat, IndexType32, maxQuadsInBatch * 4, maxQuadsInBatch * 6, true> CanvasQuadBatches;
				CanvasQuadBatches	batches;

				const Material* defaultMaterial;

				virtual void	OnPreRenderObject();
				virtual void	OnRenderObject();
				virtual void	OnPostRenderObject();

				virtual void	Update();

				Array<Image*>	maskStack;
				Matrix			projectionMatrix, worldMatrix;
				Rectf			viewport;
			};

			typedef Array<CanvasSurface*> CanvasList;
		}
	}
}

#endif