#ifndef __MORGANA_BASE_TYPES_TREE_LEAF_H__
#define __MORGANA_BASE_TYPES_TREE_LEAF_H__

#include "named.h"
#include "../reflection/meobject.h"
#include "metreenode.h"

namespace MorganaEngine
{
	namespace Base
	{
		namespace Types
		{
			class TreeLeaf : public TreeNode<TreeLeaf>, public MEObject, public IsNamed
			{
				__declare_class(TreeLeaf, MEObject);
			};
		}
	}
}

#endif