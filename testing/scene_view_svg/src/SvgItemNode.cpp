#include "SvgItemNode.h"

SvgItemNode::SvgItemNode() : QGraphicsSvgItem("host.svg")
{
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
}

void SvgItemNode::addSvgItemLink(SvgItemLink *l)
{
	links.append(l);
}

QList<SvgItemLink *> SvgItemNode::getAllLinks()
{
	return links;
}
