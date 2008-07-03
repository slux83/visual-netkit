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

#include "LineItemPrivate.h"
#include <QPen>
#include <QBrush>

/**
 * Constructor
 */
LineItemPrivate::LineItemPrivate(QGraphicsItem *parent) : QGraphicsLineItem(parent)
{
	//by default the link is up
	setLineColorByState(true);
}

/**
 * [REIMPL]
 * Redefined the bounding rect
 */

/*QRectF LineItemPrivate::boundingRect() const
{
     //qreal extra = (pen().width() + 20) / 2.0;
     
     return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
		           .normalized();
		           //.adjusted(-extra, -extra, extra, extra);
	//return QRectF();
}
*/

/**
 * Set the line style gived the hardware interface state
 * up = true
 * down = false
 */
void LineItemPrivate::setLineColorByState(bool state)
{
	if(state)
		setPen(QPen(QBrush(Qt::black), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	else
		setPen(QPen(QBrush(Qt::red), 3, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));

}
