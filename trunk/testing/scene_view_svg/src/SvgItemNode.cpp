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

QVariant SvgItemNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        foreach (SvgItemLink *link, linkList)
            link->adjust();
        	 //graph->itemMoved();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}