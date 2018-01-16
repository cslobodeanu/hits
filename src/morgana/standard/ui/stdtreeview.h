#ifndef __MORGANA_STANDARD_UI_STANDARD_TREE_VIEW_H__
#define __MORGANA_STANDARD_UI_STANDARD_TREE_VIEW_H__

#include "base.h"
#include "framework.h"

namespace MorganaEngine
{
	namespace Standard
	{
		namespace UI
		{
			class StdTreeView : public Widget
			{
				__declare_class(StdTreeView, Widget);
			public:

				StdTreeView();
				virtual ~StdTreeView();

				_property_object<FontTrueType>	font;
				_property<int>					fontSize;

				virtual void	Awake();
				virtual void	OnRenderObject();

				void			SetObject(TreeLeaf* node);

			protected:

				virtual void	Start();

				virtual void	DoAddProperties();
				virtual void	OnPropertySet(_propertyDef* atr);
				virtual void	Update();

				TreeLeaf*		root;

				virtual void	OnTreeChanged(TreeLeaf* node);

				virtual void	AttachEvents();
				virtual void	DetachEvents();

				virtual void	OnTouchClick(Component*, Vector2 p, bool& handled);

			private:

				bool		mustRefreshTree;

				void		ClearView();
				void		AddNodeToView(TreeLeaf* node);

				struct LeafInfo_t
				{
					TreeLeaf*	leaf;
					bool		closed;
					int			indent;
					const char* name;
					int			nameLen;
					int			nameVersion;
					bool		symbol;
					bool		selected;
					Vector3		min, max;

					Array<CanvasSurface::VertexFormat> cachedVertices;

					LeafInfo_t()
					{
						leaf = NULL;
						closed = false;
						indent = 0;
						symbol = false;
						selected = false;
						min = max = Vector3::Zero;
					}
				};

				Array<LeafInfo_t*>	leaves;
				void				DeleteLeaves();

				bool				mustRefreshVertices;
				void				RefreshVertices();

				Vector2				preferredSize;
			};
		}
	}
}


#endif