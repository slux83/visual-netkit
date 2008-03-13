#include "SvgItemNode.h"

SvgItemNode::SvgItemNode() : QGraphicsSvgItem("host.svg")
{
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);

}
