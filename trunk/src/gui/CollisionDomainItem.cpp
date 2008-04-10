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

#include "CollisionDomainItem.h"
#include "../common/CommonConfigs.h"

/**
 * Contructor
 */
CollisionDomainItem::CollisionDomainItem(QString label) 
	: QObject(), QGraphicsItemGroup()
{
	/* init label and svg item */
	myLabel = new LabelItemPrivate(label);
	myLabel->setFont(GRAPHICS_FONT);
	myLabel->setPos(0, 35);
	
	//by default the svg collision domain is marked as unconnected icon
	collisionDomainSvg = new SvgItemPrivate(QString::fromUtf8(":/svg/cs_off"), this);
	
	addToGroup(collisionDomainSvg);
	addToGroup(myLabel);
	
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
	setZValue(1000);
}

/**
 * Deconstructor
 */
CollisionDomainItem::~CollisionDomainItem()
{
}
