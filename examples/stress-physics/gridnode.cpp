#include "gridnode.h"

DECLARE_SOBJ_INHERITANCE(GridNode, WorldObject)

GridNode::GridNode()
{
	this->initFromConstants<GridNode>();
}
