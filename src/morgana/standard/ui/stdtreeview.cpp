#include "stdtreeview.h"
#include "framework.h"

using namespace MorganaEngine::Standard::UI;

__implement_class(StdTreeView, Widget);

StdTreeView::StdTreeView()
{
	root = NULL;
	mustRefreshTree = true;
	mustRefreshVertices = false;

	leaves.SetNullOnAlloc();

	font = NULL;
	fontSize = -1;

	DoAddProperties();
}

StdTreeView::~StdTreeView()
{
	DetachEvents();

	DeleteLeaves();
}

void StdTreeView::Awake()
{
	super::Awake();
}

void StdTreeView::Start()
{
	super::Start();

	if (font == NULL)
	{
		font = Desktop::main->theme->contentFont;
	}

	if (fontSize < 0)
	{
		fontSize = Desktop::main->theme->contentFontSize;
	}

	if (material == NULL)
		material = Material::DefaultFontCanvas;
}

void StdTreeView::OnRenderObject()
{
	if (leaves.Length() == 0) return;
	if (font == NULL) return;

	//__DrawDebugBackground(Color::red);

	Texture* tex = font->RequestGlyph('a', fontSize)->texture;

	SpriteInfo_t* white = font->RequestSprite(FontTrueType::White);
	
	safeMaterial()->SetTexture(tex);


	BeginBigDataChunk();

	if (mustRefreshVertices)
	{
		mustRefreshVertices = false;
		RefreshVertices();
	}

 	for (int i = 0; i < leaves.Length(); i++)
 	{
 		LeafInfo_t*& l = leaves[i];
 		if (l->cachedVertices.Length() == 0) continue;

		if (l->selected)
		{
			Rectf rr = Rectf(l->min, l->max - l->min); rr.Inflate((float)fontSize * 0.7f, (float)fontSize * 0.7f);

			SetSpriteColor(Desktop::main->theme->contentColor);
			PushQuad(rr.TopLeft(), rr.width, rr.height, white->rectf());
		}
 
 		PushCachedQuads(l->cachedVertices, l->cachedVertices.Length() / 4);
 
 		if (l->nameVersion != l->leaf->version)
 			mustRefreshVertices = true;
 	}

	EndBigDataChunk();
}

void StdTreeView::Update()
{
	super::Update();

	if (mustRefreshTree)
	{
		if (root != NULL)
		{
 			ClearView();
 			AddNodeToView(root);
			mustRefreshVertices = true;
		}

		mustRefreshTree = false;
	}
}

void StdTreeView::DoAddProperties()
{
	super::DoAddProperties();

	__add_property1(font);
	__add_property1(fontSize);
}

void StdTreeView::OnTreeChanged(TreeLeaf* node)
{
	mustRefreshTree = true;
}

void StdTreeView::AttachEvents()
{
	if (root != NULL)
		root->OnTreeChanged += Callback(OnTreeChanged, TreeLeaf*);
}

void StdTreeView::DetachEvents()
{
	if (root != NULL)
		root->OnTreeChanged -= Callback(OnTreeChanged, TreeLeaf*);
}

void StdTreeView::SetObject(TreeLeaf* node)
{
	DetachEvents();
	root = node;
	OnTreeChanged(root);
	AttachEvents();
}

void StdTreeView::ClearView()
{
	leaves.clear();
}

int currentIndent = 0;

void StdTreeView::AddNodeToView(TreeLeaf* node)
{
	if (node == root)
		currentIndent = 1;

	leaves.IncreaseSize(1);
	if (leaves.Last() == NULL)
	{
		leaves.Last() = new LeafInfo_t();
	}

	LeafInfo_t*& l = leaves.Last();
	l->leaf = node;
	l->closed = false;
	l->indent = currentIndent;
	l->cachedVertices.SetLength(0);
	l->name = node->GetNameRef().c_str();
	l->nameLen = strlen(l->name);
	l->nameVersion = node->version;

	currentIndent++;

	for (int i = 0; i < node->GetChildrenCount(); i++)
		AddNodeToView(node->GetChild(i));

	currentIndent--;
}

void StdTreeView::DeleteLeaves()
{
	for (int i = 0; i < leaves.GetAllocatedLength(); i++)
	{
		delete leaves[i];
	}

	leaves.Clear();
}

void StdTreeView::RefreshVertices()
{
	float y = 0.0f;

	const float lh = font->GetLineSpacing(fontSize);
	const float texth = font->GetLineHeight(fontSize);
	const float indentSz = lh * 0.8f;

	preferredSize = Vector2::Zero;

	FontTrueType* symbolFont = Desktop::main->theme->symbolsFont;
	const int symbolFontSize = fontSize;

	int closedIndex = -1;

	for (int i = 0; i < leaves.Length(); i++)
	{
		LeafInfo_t*& l = leaves[i];


		if (closedIndex >= 0)
		{
			if(l->indent > closedIndex)
			{
				l->cachedVertices.SetLength(0);
				continue;
			}

			if (l->indent == closedIndex)
				closedIndex = -1;
		}

		const float x = indentSz * (float)l->indent;

		l->cachedVertices.SetLength(l->nameLen * 4);

		SetSpriteColor(Desktop::main->theme->contentColor);

		if (l->selected)
		{
			SetSpriteColor(Desktop::main->theme->windowBkgColor);
		}

		CanvasSurface::VertexFormat* buf = l->cachedVertices.ptr();
		const int numQuads = TextTrueType::Draw<char>(this, font, fontSize, l->name, Vector2(x, y), NULL, (void**)&buf);
		l->cachedVertices.SetLength(numQuads * 4);

		l->min = l->cachedVertices.ptr()->pos;
		l->max = (l->cachedVertices.end() - 2)->pos;

		preferredSize.x = Mathf::Max(preferredSize.x, l->max.x);
		preferredSize.y = Mathf::Max(preferredSize.y, l->max.y);

		l->symbol = false;

		if (l->leaf->GetChildrenCount() > 0) // put symbol there
		{
			l->symbol = true;
			l->cachedVertices.IncreaseSize(4);

			buf = l->cachedVertices.end() - 4;

			char symbol[2]; symbol[1] = 0;
			symbol[0] = '6';

			if (l->closed)
				symbol[0] = '4';

			SetSpriteColor(Desktop::main->theme->contentColor);

			Vector2 sz = TextTrueType::Measure<char>(this, symbolFont, symbolFontSize, symbol); // down arrow
			TextTrueType::Draw<char>(this, symbolFont, symbolFontSize, symbol, Vector2(x - indentSz, y - texth * 0.5f), NULL, (void**)&buf); // down arrow
		}

		if (l->closed)
		{
			closedIndex = l->indent;
		}

		y += lh;
	}

	rectTransform->rect = Rectf(rectTransform->rect->TopLeft(), preferredSize);
}

void StdTreeView::OnPropertySet(_propertyDef* atr)
{
	super::OnPropertySet(atr);
}

void StdTreeView::OnTouchClick(Component*, Vector2 p, bool& handled)
{
	for (int i = 0; i < leaves.Length(); i++)
	{
		LeafInfo_t*& l = leaves[i];
		if (l->cachedVertices.Length() == 0) continue;

		if (l->symbol)
		{
			Vector3 min = (l->cachedVertices.end() - 4)->pos;
			Vector3 max = (l->cachedVertices.end() - 2)->pos;

			Rectf r(min, max - min); r.Inflate(4.0f, 4.0f);
			if (r.Contains(p))
			{
				// symbol clicked
				l->closed = !l->closed;
				mustRefreshVertices = true;
				return;
			}
		}
		
		if (true)
		{
			Rectf r(l->min, l->max - l->min); r.Inflate(fontSize * 0.5f, fontSize);
			if (r.Contains(p))
			{
				l->selected = !l->selected;
				mustRefreshVertices = true;
				return;
			}
		}
	}
}
