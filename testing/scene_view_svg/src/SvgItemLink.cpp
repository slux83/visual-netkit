#include "SvgItemLink.h"
#include "SvgItemNode.h"
#include <math.h>
#include <QPainter>
#include <QPolygonF>

static const double Pi = 3.14159;

SvgItemLink::SvgItemLink (SvgItemNode *startNode, SvgItemNode *endNode,
		QGraphicsItem *parent, QGraphicsScene *scene) : QGraphicsLineItem(parent, scene)
{
	setFlags(QGraphicsItem::ItemIsSelectable);
	myStartNode = startNode;
	myEndNode = endNode;
	myColor = Qt::black;
	setPen(QPen(myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	//setAcceptedMouseButtons(0);
}

 SvgItemLink::~SvgItemLink()
 {
 }

 void SvgItemLink::setSourceNode(SvgItemNode *node)
 {
	 myStartNode = node;
    adjust();
 }

 void SvgItemLink::setDestNode(SvgItemNode *node)
 {
	 myEndNode = node;
     adjust();
 }

 
 QPainterPath SvgItemLink::shape() const
 {
     QPainterPath path = QGraphicsLineItem::shape();
     //path.addPolygon(arrowHead);
     return path;
 }
 
 QRectF SvgItemLink::boundingRect() const
 {
      qreal extra = (pen().width() + 20) / 2.0;
      
      return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                        line().p2().y() - line().p1().y()))
		           .normalized()
		           .adjusted(-extra, -extra, extra, extra);
 }
 
 void SvgItemLink::adjust()
 {
     if (!myStartNode || !myEndNode) {
    	 QLineF line(QPointF(0,0), QPointF(300,30));
    	 return;
     }

     //QLineF line(mapFromItem(myStartNode, 0, 0), mapFromItem(myEndNode, 0, 0));
     QLineF line(QPointF(100,0), QPointF(300,30));
     qreal length = line.length();
     QPointF linkOffset((line.dx() * 10) / length, (line.dy() * 10) / length);

     prepareGeometryChange();
     sourcePoint = line.p1() + linkOffset;
     destPoint = line.p2() - linkOffset;
 }


 void SvgItemLink::updatePosition()
 {
     QLineF line(mapFromItem(myStartNode, 0, 0), mapFromItem(myEndNode, 0, 0));
     setLine(line);
 }

 void SvgItemLink::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
 {
     if (myStartNode->collidesWithItem(myEndNode))
         return;

     QPen myPen = pen();
     myPen.setColor(myColor);
     painter->setPen(myPen);
     painter->setBrush(myColor);

     QLineF centerLine(myStartNode->pos(), myEndNode->pos());
     QPolygonF endPolygon = myEndNode->polygon();
     QPointF p1 = endPolygon.first() + myEndNode->pos();
     QPointF p2;
     QPointF intersectPoint;
     QLineF polyLine;
     for (int i = 1; i < endPolygon.count(); ++i) 
     {
	     p2 = endPolygon.at(i) + myEndNode->pos();
	     polyLine = QLineF(p1, p2);
	     QLineF::IntersectType intersectType = polyLine.intersect(centerLine, &intersectPoint);
	     if (intersectType == QLineF::BoundedIntersection)
	         break;
	         p1 = p2;
     }

     setLine(QLineF(intersectPoint, myStartNode->pos()));

     double angle = ::acos(line().dx() / line().length());
     if (line().dy() >= 0)
         painter->drawLine(line());
     if (isSelected())
     {
    	 painter->setPen(QPen(myColor, 1, Qt::DashLine));
         QLineF myLine = line();
         myLine.translate(0, 4.0);
         painter->drawLine(myLine);
         myLine.translate(0,-8.0);
         painter->drawLine(myLine);
     }
 }

 