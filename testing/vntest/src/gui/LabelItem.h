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

#ifndef TEXTITEM_
#define TEXTITEM_

#include <QGraphicsTextItem>
#include <QPen>

#include "LabScene.h"

class QFocusEvent;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;

class LabelItem : public QGraphicsTextItem
{
    Q_OBJECT

public:
    enum { Type = UserType + 3 };
    LabelItem(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    LabelItem(QPointF *start, QPointF *end, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    int type() const { return Type; }
    void updatePosition(QPointF p1, QPointF p2);

    void setLinkStartPoint(QPointF *start);
    void setLinkEndPoint(QPointF *end);
    //void setText(QString *text);
    
signals:
    void lostFocus(LabelItem *item);
    void selectedChange(QGraphicsItem *item);

private:
	QString *text;
	//Link start and end points
	QPointF *start;
    QPointF *end;
    QGraphicsItem *parent;
    QGraphicsScene *scene;
    qreal currentAngle;	//angolo di inclinazione
	
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void focusOutEvent(QFocusEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};

#endif /*TEXTITEM_*/
