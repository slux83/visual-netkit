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
	
	myLabel = new LabelItemPrivate(label);
	myLabel->setFont(GRAPHICS_FONT);
	
	//setFlags(QGraphicsItem::ItemIsSelectable);
	setZValue(800);
	
	//set the line position
	updateLinkPos();
	
	addToGroup(lineItem);
	
	/* connects */
	connect(vm, SIGNAL(positionChanged()),
			this, SLOT(updateLinkPos()));
	connect(cd, SIGNAL(positionChanged()),
			this, SLOT(updateLinkPos()));
}

/**
 * Deconstructor
 */
LinkItem::~LinkItem()
{
}

/**
 * [PUBLIC-SLOT]
 * Update the link position/orientation
 */
void LinkItem::updateLinkPos()
{
	QPointF vmCenter, cdCenter;
	
	//start point
	vmCenter.setX(vm->getSvgPrivate()->scenePos().x() +
			vm->getSvgPrivate()->boundingRect().width() / 2);
	vmCenter.setY(vm->getSvgPrivate()->scenePos().y() + 
			vm->getSvgPrivate()->boundingRect().height() / 2);
	
	//end point
	cdCenter.setX(cd->getSvgPrivate()->scenePos().x() +
			cd->getSvgPrivate()->boundingRect().width() / 2);
	cdCenter.setY(cd->getSvgPrivate()->scenePos().y() +
			cd->getSvgPrivate()->boundingRect().height() / 2);
	
	lineItem->setLine(QLineF(vmCenter, cdCenter));
}
