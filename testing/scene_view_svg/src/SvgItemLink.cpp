#include "SvgItemLink.h"
#include "SvgItemNode.h"
#include <math.h>
#include <QPainter>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

SvgItemLink::SvgItemLink (SvgItemNode *sourceNode, SvgItemNode *destNode) : QGraphicsSvgItem()
 {
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
	setAcceptedMouseButtons(0);
    source = sourceNode;
    dest = destNode;
    source->addSvgItemLink(this);
    dest->addSvgItemLink(this);
    adjust();
}

 SvgItemLink::~SvgItemLink()
 {
 }

 SvgItemNode *SvgItemLink::sourceNode() const
 {
     return source;
 }

 void SvgItemLink::setSourceNode(SvgItemNode *node)
 {
     source = node;
     adjust();
 }

 SvgItemNode *SvgItemLink::destNode() const
 {
     return dest;
 }

 void SvgItemLink::setDestNode(SvgItemNode *node)
 {
     dest = node;
     adjust();
 }


 void SvgItemLink::adjust()
 {
     if (!source || !dest)
         return;

     QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
     qreal length = line.length();
     QPointF linkOffset((line.dx() * 10) / length, (line.dy() * 10) / length);

     prepareGeometryChange();
     sourcePoint = line.p1() + linkOffset;
     destPoint = line.p2() - linkOffset;
 }

QGraphicsLineItem *SvgItemLink::draw()
{
    if (!source || !dest)
        return NULL;

    QGraphicsLineItem *link = new QGraphicsLineItem(QLineF(sourcePoint, destPoint));
    //QGraphicsLineItem *link = new QGraphicsLineItem(QLineF(QPointF(100,100), QPointF(0,0)));
    return link;
}
