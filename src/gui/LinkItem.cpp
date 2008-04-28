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

#include "LinkItem.h"
#include <QPointF>
#include <QPen>

/**
 * Constructor
 */
LinkItem::LinkItem(VirtualMachineItem* vmItem, CollisionDomainItem* cdItem, QString label)
	: QObject(), QGraphicsItemGroup()
{
	vm = vmItem;
	cd = cdItem;
	
	lineItem = new LineItemPrivate();
	lineItem->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	
	myLabel = new LabelItemPrivate(label);
	myLabel->setFont(GRAPHICS_FONT);
	
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
	setZValue(800);
	
	//set the line position
	setLinePosition();
	
	addToGroup(lineItem);
}

/**
 * Deconstructor
 */
LinkItem::~LinkItem()
{
}

/**
 * [PRIVATE]
 * Set the line start and end
 */
void LinkItem::setLinePosition()
{
	QPointF vmCenter, cdCenter;
	
	//start point
	vmCenter.setX(vm->getSvgPrivate()->scenePos().x());
	vmCenter.setY(vm->getSvgPrivate()->scenePos().y());
	
	//end point
	cdCenter.setX(cd->getSvgPrivate()->scenePos().x());
	cdCenter.setY(cd->getSvgPrivate()->scenePos().y());
	
	lineItem->setLine(QLineF(vmCenter, cdCenter));
	
}

