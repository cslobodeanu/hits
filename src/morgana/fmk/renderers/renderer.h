#ifndef __MORGANA_FMK_RENDERERS_RENDERER_H__
#define __MORGANA_FMK_RENDERERS_RENDERER_H__

#include "../gameobjects/component.h"
#include "../gameobjects/irenderable.h"
#include "../resources/materials/material.h"

namespace MorganaEngine
{
	namespace Framework
	{
		using namespace Resources::Materials;
		using namespace GameObjects;
		namespace Renderers
		{
			class Renderer : public Component, public IRenderable
			{
				__declare_component_abstract(Renderer, Component);
				__declare_interfaces1(IRenderable);
			public:

				enum RenderQueue
				{
					FirstInQueue = 0,
					Opaque,
					Transparent,
					Overlay,
					Reserved1,
					Reserved2,
					Reserved3,
					Reserved4,
					LastInQueue
				};

				struct Drawable_t
				{
					Buffer					*vbo, *ibo;
					unsigned int			vertexOffset, indexOffset;
					int						vertexCount, indexCount;
					VertexLayout*			vertexLayout;
					int						drawMode;

					Drawable_t()
					{
						drawMode = DRAWMODE_TRIANGLES;
					}
				};

				Renderer();
				virtual ~Renderer();

				Box							bounds;

				enum RendererFlags
				{
					Default = 0,
					AlwaysVisible = 1
				};

				_property<int>				flags;
				_property<int>				renderQueue;

				_property<bool>				castShadows;
				_property<bool>				useInstancing;
				_property<bool>				useBatching;

				const Drawable_t&			GetDrawable(const int index = 0) const;
				const int					GetDrawablesCount() const;
				virtual const Matrix*		GetCustomProjectionView() const;

				Material*					writeMaterial();
				const Material*				readMaterial();

			protected:

				Material*					materialToUse;
				const Material*				sharedMaterial;
				virtual const Material*		defaultMaterial();

				virtual void	Start();
				virtual void	Update();
				virtual void	UpdateRenderables() = 0;

				virtual void	OnComponentAdded(CLASSDESC);

				Array<Drawable_t>	renderables;

				virtual void	OnPreRenderObject(){}
				virtual void	OnRenderObject(){}
				virtual void	OnPostRenderObject(){}

			private:
				void						ResetRenderable();
				void						RefreshMaterial();
				void						CleanupMaterials();
			};

			typedef Array<Renderer*>		RendererList;
		}
	}
}

#endif