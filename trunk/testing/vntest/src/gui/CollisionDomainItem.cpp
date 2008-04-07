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


#include <QPainter>
#include <QPointF>
#include <QSizeF>

#include "CollisionDomainItem.h"


/**
 * Contructor
 */
CollisionDomainItem::CollisionDomainItem (QString *name, SvgItemLink *l1, SvgItemLink *l2, 
						QGraphicsItem *parent, QGraphicsScene *s)
{
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
	scene = s;
	myName = name;
	link1 = l1;
	link2 = l2;
	myStartPoint = new QPointF( (l1->pos().x() + l2->pos().x())/2, 
								(l1->pos().y() + l2->pos().y())/2 );
}

/**
 * Contructor
 */
CollisionDomainItem::CollisionDomainItem (QString *name, QGraphicsItem *parent, QGraphicsScene *s)
{
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
	scene = s;
	myName = name;
	myStartPoint = new QPointF(0,0);
}

/**
 * Deconstructor
 */
CollisionDomainItem::~CollisionDomainItem()
{
}

QRectF CollisionDomainItem::boundingRect() const
{
	qreal penWidth = 3;
	return QRectF( 0.0 - penWidth/2,  0.0 - penWidth/2,
				   20.0 + penWidth/2,  20.0 + penWidth/2);
}

QString* CollisionDomainItem::name()
{
	return myName;
}

void CollisionDomainItem::setName(QString *name)
{
	myName = name;
}

void CollisionDomainItem::addLink(SvgItemLink *link)
{
    links.append(link);
}

void CollisionDomainItem::setStartPoint(QPointF *point)
{
	myStartPoint = point;
}

QList<SvgItemLink *> CollisionDomainItem::getLinks()
{
	return links;
}

void CollisionDomainItem::removeLink(SvgItemLink *link) {
	links.removeAt(links.indexOf(link));
}

void CollisionDomainItem::removeLinks()
{
    foreach (SvgItemLink *link, links) {
        
    	//faccio il downcasting dei QGraphicsItem  a seconda del loro tipo
		if (link->startNode()->type() == VirtualMachineItem::Type) {
			qgraphicsitem_cast<VirtualMachineItem *>(link->startNode())->removeLink(link);
		} else {
			qgraphicsitem_cast<CollisionDomainItem *>(link->startNode())->removeLink(link);
		}
		
		if (link->endNode()->type() == CollisionDomainItem::Type) {
			qgraphicsitem_cast<CollisionDomainItem *>(link->endNode())->removeLink(link);
		} else {
			qgraphicsitem_cast<VirtualMachineItem *>(link->endNode())->removeLink(link);
		}
    	
		//scene()->removeItem(link);
        delete link;
    }
}

void CollisionDomainItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	painter->setPen(QColor::fromRgb(255,0,0,100));
	QRectF rectangle(*myStartPoint, QSizeF(boundingRect().width(), boundingRect().height()));
	painter->drawEllipse(rectangle);
	
	painter->setPen(QColor::fromRgb(0,0,0,100));
	QFont font("Helvetica", 14, QFont::Bold, false);
	painter->drawText(myStartPoint->x() + (boundingRect().width()/2),
					  myStartPoint->y() + (boundingRect().height()/2), 
					  *myName);
}


QVariant CollisionDomainItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        foreach (SvgItemLink *link, links) {
            link->updatePosition();
        }
    }

    return value;
}

void CollisionDomainItem::updatePosition(QPointF *p)
{
	setPos(*p);
}


