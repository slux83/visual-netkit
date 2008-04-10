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

#include <QtGui>
#include "LabelItem.h"

#include "math.h"

LabelItem::LabelItem(QGraphicsItem *parent, QGraphicsScene *scene) : QGraphicsTextItem(parent, scene)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    this->parent = parent;
    this->scene = scene;
    currentAngle = 0;
}

LabelItem::LabelItem(QPointF *start, QPointF *end, QGraphicsItem *parent, QGraphicsScene *scene) : QGraphicsTextItem(parent, scene)
{
	setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    this->start = start;
    this->end = end;
    this->parent = parent;
    this->scene = scene;
    currentAngle = 0;
}

void LabelItem::setLinkStartPoint(QPointF *start)
{
	this->start = start;
}

void LabelItem::setLinkEndPoint(QPointF *end)
{
	this->end = end;
}

QVariant LabelItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedChange)
        emit selectedChange(this);
    return value;
}

void LabelItem::focusOutEvent(QFocusEvent *event)
{
    setTextInteractionFlags(Qt::NoTextInteraction);
    emit lostFocus(this);
    QGraphicsTextItem::focusOutEvent(event);
}

void LabelItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    scene->clearSelection();
    QGraphicsTextItem::mousePressEvent(event);
    setSelected(true);
}

void LabelItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (textInteractionFlags() & Qt::TextEditable) {
        QGraphicsTextItem::mouseMoveEvent(event);
    } else
        QGraphicsItem::mouseMoveEvent(event);
}

void LabelItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsSceneMouseEvent *mouseEvent =
        new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMousePress);
    mouseEvent->setAccepted(true);
    mouseEvent->setPos(event->pos());
    mouseEvent->setScenePos(event->scenePos());
    mouseEvent->setScreenPos(event->screenPos());
    mouseEvent->setButtonDownPos(Qt::LeftButton,
        event->buttonDownPos(Qt::LeftButton));
    mouseEvent->setButtonDownScreenPos(Qt::LeftButton,
        event->buttonDownScreenPos(Qt::LeftButton));
    mouseEvent->setButtonDownScenePos(Qt::LeftButton,
        event->buttonDownScenePos(Qt::LeftButton));
    mouseEvent->setWidget(event->widget());

    QGraphicsTextItem::mousePressEvent(mouseEvent);

    delete mouseEvent;
}

void LabelItem::updatePosition(QPointF p1, QPointF p2)
{	
	//centro l'etichetta nel punto medio del link
	QPointF center;
    center.setX((p2.x() + p1.x())*0.5);
    center.setY((p2.y() + p1.y())*0.5);
    setPos(center);
    
    QLineF line(p1, p2);
    qreal Pi = 3.1415;
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0) {
		angle = (Pi * 2) - angle;
		
		QPointF lp1 = line.p1() + QPointF(sin(angle + Pi / 3),
		                                 cos(angle + Pi / 3));
		QPointF lp2 = line.p1() + QPointF(sin(angle + Pi - Pi / 3),
                                         cos(angle + Pi - Pi / 3));
	}
    
    //ruoto l'etichetta per allinearla parallelamente al link
    /*
    QLineF line(p1, p2);
    qreal angle = line.angle(QLineF(0, 0, 10, 0));		//angolo tra il link e una retta parallela all'asse delle ascisse
    
    QLineF l(boundingRect().bottomLeft(), boundingRect().bottomRight());
    currentAngle = l.angle(QLineF(0,0, 10,0));
    
    rotate(currentAngle - angle);
    */
}

