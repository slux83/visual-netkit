/**
 * VisualNetkit is an advanced graphical tool for NetKit <http://www.netkit.org>
 * Copyright (C) 2008  Alessio Di Fazio, Paolo Minasi
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <math.h>
#include <QPainter>
#include <QPointF>

#include "SvgItemLink.h"

static const double Pi = 3.14159;

SvgItemLink::SvgItemLink (QGraphicsItem *startNode, QGraphicsItem *endNode,
		QGraphicsItem *parent, QGraphicsScene *scene) : QGraphicsLineItem(parent, scene)
{
	setFlags(QGraphicsItem::ItemIsSelectable);
	this->scene = scene;
	
	myStartNode = startNode;
	myEndNode = endNode;
	
	ipLabel = new LabelItem();
	myColor = Qt::black;
	setPen(QPen(myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	//setAcceptedMouseButtons(0);
}

 SvgItemLink::~SvgItemLink()
 {
 }

 void SvgItemLink::setSourceNode(QGraphicsItem *node)
 {
	 myStartNode = node;
     adjust();
 }

 void SvgItemLink::setDestNode(QGraphicsItem *node)
 {
	 myEndNode = node;
     adjust();
 }

 
 QPainterPath SvgItemLink::shape() const
 {
     QPainterPath path = QGraphicsLineItem::shape();
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

     QLineF line(mapFromItem(myStartNode, 0, 0), mapFromItem(myEndNode, 0, 0));
     qreal length = line.length();
     QPointF linkOffset((line.dx() * 10) / length, (line.dy() * 10) / length);

     prepareGeometryChange();
     sourcePoint = line.p1() + linkOffset;
     destPoint = line.p2() - linkOffset;
 }


 void SvgItemLink::updatePosition()
 {
	 //update line position
     QLineF line(mapFromItem(myStartNode, 0, 0), mapFromItem(myEndNode, 0, 0));
     setLine(line);
     
     //update label position
     ipLabel->updatePosition(line.p1(), line.p2());
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
     
     int itemWidth = 48;	//per centrare il punto di attacco dei link
     int itemHeight = 48;
     
     QPoint start;
     start.setX(myStartNode->pos().x() + myStartNode->boundingRect().width()/2);
     start.setY(myStartNode->pos().y() + myStartNode->boundingRect().height()/2);
     
     QPoint end;
     end.setX(myEndNode->pos().x() + myEndNode->boundingRect().width()/2);
     end.setY(myEndNode->pos().y() + myEndNode->boundingRect().height()/2);
     
     setLine(QLineF(start, end));
     painter->drawLine(line());
     
     /*
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
     */
 }
 
 //Associa l'etichetta al link
 void SvgItemLink::addLabel(LabelItem *label)
 {
	 ipLabel = label;
 }
 

