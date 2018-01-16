#include "renderstocanvas.h"
#include "meresources.h"
#include "canvassurface.h"
#include "image.h"

using namespace MorganaEngine::Framework::Canvas;

__implement_component_abstract(RendersToCanvas, Component);

RendersToCanvas::RendersToCanvas()
{
	material = NULL;
	materialInstance = NULL;
	canvas = NULL;
	rectTransform = NULL;

	hasWorldMatrix = false;

	spriteColorMask = Color255(0, 255, 0, 0);

	DoAddProperties();
}

RendersToCanvas::~RendersToCanvas()
{

}

void RendersToCanvas::Awake()
{
	super::Awake();

	rectTransform = gameObject->RequestComponent<RectTransform>();
	rectTransform->OnResize += Callback(OnResizeRect);

	ASSERT(rectTransform != NULL);
}

void RendersToCanvas::Start()
{
	super::Start();
}

void RendersToCanvas::DoAddProperties()
{
	super::DoAddProperties();

	__add_property1(color);
}

void RendersToCanvas::PushMask(void* imgPtr)
{
	if (canvas != NULL)
		canvas->maskStack.Add((Image*)imgPtr);
}

void RendersToCanvas::PopMask()
{
	if (canvas != NULL)
		canvas->maskStack.Pop();
}

Material* RendersToCanvas::safeMaterial()
{
	if (material == NULL)
	{
		material = canvas->defaultMaterial;
	}

	if (materialInstance == NULL)
	{
		materialInstance = new Material(*material);
	}
	return materialInstance;
}

void RendersToCanvas::SetupIndices(IndexType32* inds, const int numInds /*= 6*/)
{
	if (inds != NULL)
	{
		if (numInds != 6)
		{
			static IndexType32 stdQuad[6] = { 0, 1, 2, 0, 2, 3 };
			memcpy(inds, stdQuad, numInds * sizeof(IndexType32));
		}
		else
		{
			for (int i = 0; i < numInds / 6; i++)
			{
				inds[6 * i + 0] = 4 * i + 0;
				inds[6 * i + 1] = 4 * i + 1;
				inds[6 * i + 2] = 4 * i + 2;
				inds[6 * i + 3] = 4 * i + 0;
				inds[6 * i + 4] = 4 * i + 2;
				inds[6 * i + 5] = 4 * i + 3;
			}
		}
		canvas->batches.AdjustIndices(numInds);
	}
}

void RendersToCanvas::PushQuad(const Vector2& origin, const float& width, const float& height, const Rectf& texPos)
{
	QuadInfo_t q;
	q.origin = origin;
	q.width = width;
	q.height = height;
	q.texCoord = texPos;
	q.transposed = false;
	q.color = spriteColor;
	q.color2 = Color255(0, 255, 0, 0);

	PushQuads(&q, 1);
}

void RendersToCanvas::PushQuadTransposed(const Vector2& origin, const float& width, const float& height, const Rectf& texPos)
{
	QuadInfo_t q;
	q.origin = origin;
	q.width = width;
	q.height = height;
	q.texCoord = texPos;
	q.transposed = true;
	q.color = spriteColor;
	q.color2 = Color255(0, 255, 0, 0);

	PushQuads(&q, 1);
}

void RendersToCanvas::PushQuad(const float& width, const float& height, const Rectf& texPos)
{
	PushQuad(Vector2::Zero, width, height, texPos);
}

void RendersToCanvas::PushQuad(const Rectf& screenRect, const Rectf& texPos)
{
	PushQuad(Vector2(screenRect.x, screenRect.y), screenRect.width, screenRect.height, texPos);
}

void RendersToCanvas::QuadsToWorldVerts(QuadInfo_t* quads, const int count, const Vector2& origin, byte* worldVerts, const int stride /*= 12*/)
{
	Vector3 right;
	Vector3 up;
	
	bool noRotnoScale = false;
	if (rectTransform->rotationIsZero)
	{
		if (rectTransform->scaleIsOne)
		{
			right = Vector3::Right;
			up = Vector3::Up;
			noRotnoScale = true;
		}
		else
		{
			right = Vector3(rectTransform->scale->x, 0.0f, 0.0f);
			up = Vector3(0.0f, rectTransform->scale->y, 0.0f);
		}
	}
	else
	{
		right = rectTransform->rotation->RightDir() * rectTransform->scale;
		up = rectTransform->rotation->UpDir() * rectTransform->scale;
	}

	const Vector3 offset = rectTransform->position + origin;

	byte* worldBuf = worldVerts;
	Vector3 *v1, *v2, *v3, *v4;

	for (QuadInfo_t *q = quads, *end = quads + count; q < end; q++, worldBuf += stride * 4)
	{
		Vector3 farRight;
		Vector3 farUp;

		if (noRotnoScale)
		{
			farRight = q->transposed ? Vector3(0.0f, q->width, 0.0f) : Vector3(q->width, 0.0f, 0.0f);
			farUp = q->transposed ? Vector3(q->height, 0.0f, 0.0f) : Vector3(0.0f, q->height, 0.0f);
		}
		else
		{
			farRight = (q->transposed ? up : right) * q->width;
			farUp = (q->transposed ? right : up) * q->height;
		}

		v1 = (Vector3*)worldBuf;
		v2 = (Vector3*)(worldBuf + stride);
		v3 = (Vector3*)(worldBuf + stride * 2);
		v4 = (Vector3*)(worldBuf + stride * 3);

		Vector3::Add(&offset, &q->origin, v1);
		Vector3::Add(v1, &farRight, v2);
		Vector3::Add(v2, &farUp, v3);
		Vector3::Add(v1, &farUp, v4);
	}
}

void RendersToCanvas::OnParentAttached()
{
	GetCanvas();
}

void RendersToCanvas::OnComponentAdded(CLASSDESC cd)
{
	GetCanvas();

	rectTransform = gameObject->RequestComponent<RectTransform>();
	rectTransform->OnResize += Callback(OnResizeRect);
}

void RendersToCanvas::GetCanvas()
{
	canvas = NULL;

	RendersToCanvas* rtc = gameObject->GetComponentInParent<RendersToCanvas>();
	if (rtc != NULL)
	{
		canvas = rtc->canvas;
	}

	CanvasSurface* surf = gameObject->GetComponentInParent<CanvasSurface>();
	if (surf != NULL)
	{
		canvas = surf;
	}
}

void RendersToCanvas::PushQuads(QuadInfo_t* quads, const int count, const Vector2& origin /*= Vector2::Zero*/, void** cache /*= NULL*/)
{
	PushQuadsM(quads, count, origin, cache, safeMaterial());
}

void RendersToCanvas::PushQuadsM(QuadInfo_t* quads, const int count, const Vector2& origin /*= Vector2::Zero*/, void** cache /*= NULL*/, Material* mat /*= NULL*/)
{
	ASSERT(canvas != NULL, "No Canvas !");

	const bool isUsingMask = canvas->maskStack.Length() > 0;
	RectTransform* maskTransform = NULL;
	if (isUsingMask)
	{
		Image* mask = canvas->maskStack.Last();

		Material* mat = safeMaterial();
		const Material* maskMat = mask->material;
		if (maskMat != NULL)
		{
			mat->ChangeShader(maskMat->shader);
		}
		mat->SetTexture(mask->texture, 1);
		mat->SetColor("maskColor", mask->color);

		maskTransform = mask->GetComponent<RectTransform>();
	}

	canvas->batches.SetContext(this);

	static Array<Vector3> worldCorners;
	worldCorners.SetLength(count * 4);

	CanvasSurface::VertexFormat* vertices;
	IndexType32* indices;

	if (cache != NULL)
		vertices = (CanvasSurface::VertexFormat*)(*cache);
	else
		canvas->batches.Alloc(count * 4, count * 6, &vertices, &indices, mat);

	if (hasWorldMatrix == false)
		QuadsToWorldVerts(quads, count, origin, (byte*)worldCorners.ptr(), sizeof(Vector3));

	Vector3* wc = worldCorners.ptr();

	const Vector3 origin3 = origin;

	for (QuadInfo_t *q = quads, *end = quads + count; q < end; q++, vertices += 4, wc += 4)
	{
		if (hasWorldMatrix || cache != NULL)
		{
			q->ComputeLocalVerts();
		}
		for (int i = 0; i < 4; i++)
		{
			memcpy(&vertices[i].color.r, &q->color.r, 4);
			vertices[i].pos = hasWorldMatrix ? (q->localVerts[i] + origin3): wc[i];
			vertices[i].uv = q->texCoord.TopLeft();
			vertices[i].color2 = q->color2;
		}

		vertices[1].uv.x += q->texCoord.width;
		vertices[2].uv.x += q->texCoord.width;
		vertices[2].uv.y += q->texCoord.height;
		vertices[3].uv.y += q->texCoord.height;

		if (isUsingMask)
		{
			const Vector3 maskCorner = maskTransform->rect->TopLeft();
			Vector3 tmp;
			Vector2 localCorners[4];
			localCorners[0] = q->origin;
			localCorners[1] = Vector2(q->origin.x + q->width, q->origin.y);
			localCorners[2] = Vector2(q->origin.x + q->width, q->origin.y + q->height);
			localCorners[3] = Vector2(q->origin.x, q->origin.y + q->height);

			for (int i = 0; i < 4; i++)
			{
				Vector3::Transform(&localCorners[i], maskTransform->GetInvertedWorldMatrixPtr(), &tmp);
				Vector3 d = tmp - maskCorner;
				vertices[i].uv2 = Vector2(d.x / maskTransform->rect->width, d.y / maskTransform->rect->height);
			}
		}
	}

	if (cache == NULL)
		SetupIndices(indices, 6 * count);
}

void RendersToCanvas::PushCachedQuads(const void* quadVertices, const int numQuads)
{
	CanvasSurface::VertexFormat* vertices;
	IndexType32* indices;

	canvas->batches.Alloc(numQuads * 4, numQuads * 6, &vertices, &indices, safeMaterial());
	memcpy(vertices, quadVertices, numQuads * 4 * sizeof(CanvasSurface::VertexFormat));
	SetupIndices(indices, 6 * numQuads);
}

const Vector2 RendersToCanvas::GetPreferredSize() const
{
	return Vector2::Zero;
}

void RendersToCanvas::__DrawDebugBackground(Color col /*= Color(1.0f, 1.0f, 1.0f, 0.2f)*/)
{
#ifdef _DEBUG
	SetSpriteColor(col);
	safeMaterial()->SetTexture(Texture::DefaultCheckered16);
	PushQuad(rectTransform->rect->width, rectTransform->rect->height, Rectf(0.0f, 0.0f, 3.0f, 3.0f / rectTransform->rect->Aspect()));
#endif
}

void RendersToCanvas::OnResizeRect()
{

}

void RendersToCanvas::BeginBigDataChunk()
{
	hasWorldMatrix = true;
	canvas->batches.PushWorldMatrix(rectTransform->GetWorldMatrixPtr());
}

void RendersToCanvas::EndBigDataChunk()
{
	hasWorldMatrix = false;
	canvas->batches.PopWorldMatrix();
}

void RendersToCanvas::QuadInfo_t::ComputeLocalVerts()
{
	if (transposed)
	{
		const Vector3 right = Vector3(0.0f,width, 0.0f);
		const Vector3 up = Vector3(height, 0.0f, 0.0f);

		localVerts[0] = origin;
		localVerts[1] = origin + right;
		localVerts[2] = origin + right + up;
		localVerts[3] = origin + up;
	}
	else
	{
		const Vector3 right = Vector3(width, 0.0f, 0.0f);
		const Vector3 up = Vector3(0.0f, height, 0.0f);

		localVerts[0] = origin;
		localVerts[1] = origin + right;
		localVerts[2] = origin + right + up;
		localVerts[3] = origin + up;
	}
}

void RendersToCanvas::SetSpriteColor(const Color& col)
{
	spriteColor = col;
}

const Color& RendersToCanvas::GetSpriteColor() const
{
	return spriteColor;
}

void RendersToCanvas::PushSprite(const SpriteInfo_t* si, const Vector2& origin, const float& width, const float& height)
{
	PushQuad(origin, width, height, si->rectf());
}

void RendersToCanvas::PushSlicedSprite(const SpriteInfo_t* si, const Vector2& origin, const float& width, const float& height, const Vector2& borderScale /*= Vector2::One*/)
{
	static QuadInfo_t quads[9];
	memset(quads, 0, sizeof(quads));

	const float texWidth = (float)si->texture->GetBounds().width;
	const float texHeight = (float)si->texture->GetBounds().height;
	const Vector2 texSize = Vector2(texWidth, texHeight);

	const Vector2 topLeftCorner = Vector2((float)si->borderLeft, (float)si->borderTop) * borderScale;
	const Vector2 topRightCorner = Vector2((float)si->borderRight, (float)si->borderTop) * borderScale;
	const Vector2 bottomLeftCorner = Vector2((float)si->borderLeft, (float)si->borderBottom) * borderScale;
	const Vector2 bottomRightCorner = Vector2((float)si->borderRight, (float)si->borderBottom) * borderScale;

	QuadInfo_t* q = quads;

	//top left
	q->width = topLeftCorner.x;
	q->height = topLeftCorner.y;
	q->texCoord.pos(Vector2((float)si->rect.x, (float)si->rect.y));
	q->texCoord.width = (float)si->borderLeft;
	q->texCoord.height = (float)si->borderTop;

	q++;
	//top border
	q->origin = Vector2(topLeftCorner.x, 0.0f);
	q->width = width - topLeftCorner.x - topRightCorner.x;
	q->height = topLeftCorner.y;
	q->texCoord.pos(Vector2((float)(si->rect.x + si->borderLeft), (float)si->rect.y));
	q->texCoord.width = (float)(si->rect.width - si->borderLeft - si->borderRight);
	q->texCoord.height = (float)si->borderTop;

	q++;
	//top right
	q->origin = Vector2(width - topRightCorner.x, 0.0f);
	q->width = topRightCorner.x;
	q->height = topRightCorner.y;
	q->texCoord.pos(Vector2((float)(si->rect.x + si->rect.width - si->borderRight), (float)si->rect.y));
	q->texCoord.width = (float)si->borderRight;
	q->texCoord.height = (float)si->borderTop;

	q++;
	// right border
	q->origin = Vector2(width - topRightCorner.x, topRightCorner.y);
	q->width = topRightCorner.x;
	q->height = height - topRightCorner.y - bottomRightCorner.y;
	q->texCoord.pos(Vector2((float)(si->rect.x + si->rect.width - si->borderRight), (float)(si->rect.y + si->borderTop)));
	q->texCoord.width = (float)si->borderRight;
	q->texCoord.height = (float)(si->rect.height - si->borderTop - si->borderBottom);

	q++;
	// bottom right
	q->origin = Vector2(width - bottomRightCorner.x, height - bottomRightCorner.y);
	q->width = bottomRightCorner.x;
	q->height = bottomRightCorner.y;
	q->texCoord.pos(Vector2((float)(si->rect.right() - si->borderRight), (float)(si->rect.bottom() - si->borderBottom)));
	q->texCoord.width = (float)si->borderRight;
	q->texCoord.height = (float)si->borderBottom;

	q++;
	// bottom border
	q->origin = Vector2(bottomLeftCorner.x, height - bottomLeftCorner.y);
	q->width = width - bottomRightCorner.x - bottomLeftCorner.x;
	q->height = bottomRightCorner.y;
	q->texCoord.pos(Vector2((float)(si->rect.x + si->borderLeft), (float)(si->rect.bottom() - si->borderBottom)));
	q->texCoord.width = (float)(si->rect.width - si->borderLeft - si->borderRight);
	q->texCoord.height = (float)si->borderBottom;

	q++;
	// bottom left
	q->origin = Vector2(0.0f, height - bottomLeftCorner.y);
	q->width = bottomLeftCorner.x;
	q->height = bottomLeftCorner.y;
	q->texCoord.pos(Vector2((float)si->rect.x, (float)(si->rect.bottom() - si->borderBottom)));
	q->texCoord.width = (float)si->borderLeft;
	q->texCoord.height = (float)si->borderBottom;

	q++;
	// left border
	q->origin = Vector2(0.0f, topLeftCorner.y);
	q->width = topLeftCorner.x;
	q->height = height - topLeftCorner.y - bottomLeftCorner.y;
	q->texCoord.pos(Vector2((float)si->rect.x, (float)(si->rect.y + si->borderTop)));
	q->texCoord.width = (float)si->borderLeft;
	q->texCoord.height = (float)(si->rect.height - si->borderTop - si->borderBottom);


	q++;
	// middle
	q->origin = topLeftCorner;
	q->width = width - topLeftCorner.x - topRightCorner.x;
	q->height = height - topLeftCorner.y - bottomLeftCorner.y;
	q->texCoord.pos(Vector2((float)(si->rect.x + si->borderLeft), (float)(si->rect.y + si->borderTop)));
	q->texCoord.width = (float)(si->rect.width - si->borderLeft - si->borderRight);
	q->texCoord.height = (float)(si->rect.height - si->borderTop - si->borderBottom);

	for (QuadInfo_t* qq = quads; qq < (quads + 9); qq++)
	{
		qq->texCoord.x /= texSize.x;
		qq->texCoord.y /= texSize.y;
		qq->texCoord.width /= texSize.x;
		qq->texCoord.height /= texSize.y;

		qq->color = spriteColor;
		qq->color2 = spriteColorMask;
	}

	PushQuads(quads, 9, origin);
}

