#include "SvgItemLink.h"
#include <math.h>
#include <QLineF>
#include <QPainter>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;
 
 SvgItemLink::SvgItemLink (SvgItemNode *sourceNode, SvgItemNode *destNode) : QGraphicsSvgItem()
 {
	 setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
     setAcceptedMouseButtons(0);
     source = sourceNode;
     dest = destNode;
 //    source->addSvgItemLink(this);
 //    dest->addSvgItemLink(this);
     //adjust();
 }

 SvgItemLink::~SvgItemLink()
 {
 }

 SvgItemNode *SvgItemLink::sourceNode()
 {
     return source;
 }

 void SvgItemLink::setSourceNode(SvgItemNode *node)
 {
     source = node;
     adjust();
 }

 SvgItemNode *SvgItemLink::destNode()
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

 QRectF SvgItemLink::boundingRect()
 {
     if (!source || !dest)
         return QRectF();

     qreal penWidth = 1;
     qreal extra = (penWidth) / 2.0;

     return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                       destPoint.y() - sourcePoint.y()))
         .normalized()
         .adjusted(-extra, -extra, extra, extra);
 }

 void SvgItemLink::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
 {
     if (!source || !dest)
         return;

     // Draw the line itself
     QLineF line(sourcePoint, destPoint);
     painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
     painter->drawLine(line);

     // Draw the arrows if there's enough room
     double angle = ::acos(line.dx() / line.length());
     if (line.dy() >= 0)
         angle = TwoPi - angle;
     
     painter->setBrush(Qt::black);
     painter->drawPolygon(QPolygonF() << line.p1());
     painter->drawPolygon(QPolygonF() << line.p2());
 }