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


#ifndef COLLISIONDOMAINITEM_H_
#define COLLISIONDOMAINITEM_H_

#include <QGraphicsSvgItem>
#include <QSvgRenderer>
#include <QString>
#include <QPainterPath>
#include <QObject>
#include <QDebug>

#include "SvgItemLink.h"

class SvgItemLink;

/**
 * This class is a graphic element of type Collision Domain
 */
class CollisionDomainItem : public QObject, public QGraphicsItem
{
	Q_OBJECT
	
public:
	CollisionDomainItem(QString *name, SvgItemLink *l1, SvgItemLink *l2, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
	CollisionDomainItem(QString *name, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
	~CollisionDomainItem();
	
	QRectF boundingRect() const;
	QList<SvgItemLink *> getLinks();
	void removeLink(SvgItemLink *link);
	void removeLinks();
	void addLink(SvgItemLink *link);
	void setStartPoint(QPointF *point);
	void updatePosition(QPointF *p);
	
	QString* name();
	void setName(QString *s);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    
private:
	QPointF *myStartPoint;
	QString *myName;
	SvgItemLink *link1;
	SvgItemLink *link2;
	QGraphicsScene *scene;
	
	QList<SvgItemLink *> links;
	
};

#endif /*COLLISIONDOMAINITEM_H_*/
