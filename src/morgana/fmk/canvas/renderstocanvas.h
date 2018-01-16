#ifndef __MORGANA_FMK_CANVAS_RENDERS_TO_CANVAS_H__
#define __MORGANA_FMK_CANVAS_RENDERS_TO_CANVAS_H__

#include "reflection.h"
#include "gameobjects.h"
#include "recttransform.h"
#include "canvassurface.h"

namespace MorganaEngine
{
	namespace Framework
	{
		namespace Canvas
		{
			class RendersToCanvas : public Component, public IRenderable
			{
				__declare_component_abstract(RendersToCanvas, Component);
				__declare_interfaces1(IRenderable);
			public:
				RendersToCanvas();
				virtual			~RendersToCanvas();

				_property<Color>	color;

				const Material*	material;

				virtual void	Awake();

				virtual const Vector2	GetPreferredSize() const;

			protected:

				friend class TextTrueType;

				struct QuadInfo_t  
				{
					Vector3		origin;
					float		width, height;
					Rectf		texCoord;
					bool		transposed;
					Vector3		localVerts[4];
					void		ComputeLocalVerts();
					Color255	color;
					Color255	color2;
				};

				virtual void	DoAddProperties();

				virtual void	Start();
				
				virtual void	PushQuad(const Rectf& screenRect, const Rectf& texPos);
				virtual void	PushQuad(const Vector2& origin, const float& width, const float& height, const Rectf& texPos);
				virtual void	PushQuadTransposed(const Vector2& origin, const float& width, const float& height, const Rectf& texPos);
				virtual void	PushQuads(QuadInfo_t* quads, const int count, const Vector2& origin = Vector2::Zero, void** cache = NULL);
				virtual void	PushQuadsM(QuadInfo_t* quads, const int count, const Vector2& origin = Vector2::Zero, void** cache = NULL, Material* m = NULL);
				virtual void	PushCachedQuads(const void* quadVertices, const int numQuads);
				virtual void	PushQuad(const float& width, const float& height, const Rectf& texPos);
				virtual void	PushSprite(const SpriteInfo_t* si, const Vector2& origin, const float& width, const float& height);
				virtual void	PushSlicedSprite(const SpriteInfo_t* si, const Vector2& origin, const float& width, const float& height, const Vector2& borderScale = Vector2::One);

				Material*		safeMaterial();

				void			QuadsToWorldVerts(QuadInfo_t* quads, const int count, const Vector2& origin, byte* worldVerts, const int stride = 12);

				void			SetupIndices(IndexType32* inds, const int numInds = 6);

				void			PushMask(void* imgPtr);
				void			PopMask();

				CanvasSurface*	canvas;
				RectTransform*	rectTransform;

				virtual	void	OnParentAttached();
				virtual void	OnComponentAdded(CLASSDESC);

				void			__DrawDebugBackground(Color col = Color(1.0f, 1.0f, 1.0f, 0.2f));

				virtual void	OnResizeRect();

				bool			hasWorldMatrix;
				void			BeginBigDataChunk();
				void			EndBigDataChunk();

				void			SetSpriteColor(const Color& col);
				const Color&	GetSpriteColor() const;

			private:

				Color			spriteColor, spriteColorMask;

				void			GetCanvas();

				Material*		materialInstance;
			};
		}
	}
}

#endif