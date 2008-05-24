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

#ifndef LINKITEM_H_
#define LINKITEM_H_

#include <math.h>
#include <QGraphicsItemGroup>
#include <QGraphicsLineItem>

#include "LabelItemPrivate.h"
#include "VirtualMachineItem.h"
#include "CollisionDomainItem.h"
#include "LineItemPrivate.h"
#include "../common/Types.h"
#include "PluginsSharedArea.h"

/* Pi-greco */
static const double Pi = 3.14159;

/**
 * This class is a graphic group element of type Link
 * and cantain a label and a graphics line item.
 * (it's a QGraphicsItemGroup)
 */
class LinkItem : public QObject, public QGraphicsItemGroup
{
	Q_OBJECT

private:
	LineItemPrivate *lineItem;
	LabelItemPrivate *myLabel;
	PluginsSharedArea *pluginsSharedArea;
	VirtualMachineItem* vm;
	CollisionDomainItem* cd;	//start and end of this "line"
		
public:
	LinkItem(VirtualMachineItem* vmItem, CollisionDomainItem* cdItem, QString label);
	virtual ~LinkItem();
	
	LineItemPrivate* getLineItemPrivate() { return lineItem; };
	LabelItemPrivate* getLabelItemPrivate() { return myLabel; };
	VirtualMachineItem* getVirtualMachineItem() { return vm; };
	CollisionDomainItem* getCollisionDomainItem() { return cd; };
	
	/* Reimpl */
	int type() const { return UserType + LnkItem; };
	//QRectF boundingRect() const;
	
public slots:
	void updateLinkPos();
	void updateLinkState();
	void setPluginLine(QString pluginName, QString content);
};

#endif /*LINKITEM_H_*/
