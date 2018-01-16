#include "treeleaf.h"

using namespace MorganaEngine::Base::Types;

__implement_class(TreeLeaf, MEObject);

TreeLeaf*	TreeNode<TreeLeaf>::root = null;
bool		TreeNode<TreeLeaf>::isDeletingRoot = false;