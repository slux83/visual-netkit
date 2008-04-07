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

#ifndef SVGITEMLINK_H
#define SVGITEMLINK_H

#include <QGraphicsLineItem>
#include <QGraphicsItem>
#include <QRectF>
#include <QPainterPath>
#include <QObject>
#include "LabelItem.h"
#include "VirtualMachineItem.h"
#include "CollisionDomainItem.h"

class VirtualMachineItem;
class CollisionDomainItem;
class LabelItem;
class QColor;

class SvgItemLink : public QObject, public QGraphicsLineItem
{
	Q_OBJECT	
	
public:
	 SvgItemLink(QGraphicsItem *sourceNode, QGraphicsItem *destNode,
			 QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
	 ~SvgItemLink();
	 
	 QRectF boundingRect() const;
	 QPainterPath shape() const;
	 
	 void setSourceNode(QGraphicsItem *node);
	 void setDestNode(QGraphicsItem *node);

     void setColor(const QColor &color) { myColor = color; }
     QGraphicsItem *startNode() const { return myStartNode; }
     QGraphicsItem *endNode() const { return myEndNode; }
     
     void addLabel(LabelItem *label);
     LabelItem *label() const { return ipLabel; };
     
	 void adjust();
	 QGraphicsLineItem* draw();

public slots:
     void updatePosition();

protected:
     void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);
private:
	 QGraphicsItem *myStartNode;
	 QGraphicsItem *myEndNode;
	 LabelItem *ipLabel;
     QColor myColor;

     QPointF sourcePoint;
     QPointF destPoint;

     QGraphicsScene *scene;
};
#endif
