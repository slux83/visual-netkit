#include "SvgItemNode.h"

SvgItemNode::SvgItemNode() : QGraphicsSvgItem("host.svg")
{
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
}

QList<SvgItemLink *> SvgItemNode::getLinks()
{
	return links;
}


void SvgItemNode::removeLinks()
{
    foreach (SvgItemLink *link, links) {
        //link->startNode()->removeLink(link);
        //link->endNode()->removeLink(link);
        //scene()->removeItem(link);
        delete link;
    }
}

void SvgItemNode::addLink(SvgItemLink *link)
{
    links.append(link);
}

void SvgItemNode::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    //scene()->clearSelection();
    setSelected(true);
    //myContextMenu->exec(event->screenPos());
}

QVariant SvgItemNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        foreach (SvgItemLink *link, links) {
            link->updatePosition();
        }
    }

    return value;
}

