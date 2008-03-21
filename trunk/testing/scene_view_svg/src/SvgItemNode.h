#ifndef SVGITEMNODE_H
#define SVGITEMNODE_H

#include <QGraphicsSvgItem>
#include <QLinkedList>
#include <QVariant>
#include <QRectF>
#include "SvgItemLink.h"


class QColor;
class QPolygonF;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneMouseEvent;
class QMenu;
class QGraphicsSceneContextMenuEvent;
class QGraphicsScene;

class SvgItemNode : public QGraphicsSvgItem
{
	Q_OBJECT

public:
	SvgItemNode();
	QList<SvgItemLink *> getLinks();

    void removeLink(SvgItemLink *link);
    void removeLinks();
    void addLink(SvgItemLink *link);
    
    QPolygonF polygon() const
    { return myPolygon; }
    
private:
    QList<SvgItemLink *> links;
    QMenu *myContextMenu;
	QColor linkColor;
    QPolygonF myPolygon;
	
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

};
#endif
