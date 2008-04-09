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

#include "VirtualMachineItem.h"
#include "../common/CommonConfigs.h"
#include <QGraphicsScene>

/**
 * Contructor
 * by default, this svg item is showed as VmHost
 */
VirtualMachineItem::VirtualMachineItem(QString label, VmType type) 
	: QGraphicsItemGroup()
{
	/* Fill the filemap */
	svgFiles.insert(Host, QString::fromUtf8(":/svg/vm_host"));
	svgFiles.insert(Router, QString::fromUtf8(":/svg/vm_router"));
	
	/* Set the default svg file: VmHost */
	vmSvg = new SvgItemPrivate(svgFiles.value(type));
	vmNameLabel = new QGraphicsSimpleTextItem(label);
	vmNameLabel->setPos(0, 52);
	vmNameLabel->setFont(GRAPHICS_FONT);
	
	/* Add svg and label to this group */	
	addToGroup(vmSvg);
	addToGroup(vmNameLabel);
	
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
	setZValue(1000);
}

/**
 * Deconstructor
 */
VirtualMachineItem::~VirtualMachineItem()
{
}

/**
 * [PUBLIC]
 * Change the visualization for this vm
 */
void VirtualMachineItem::changeSvgFile(VmType type)
{
	if(!svgFiles.contains(type))
	{
		qWarning() << "VmType" << type << "NOT FOUND INSIDE THE MAP";
		return;
	}
	
	vmSvg->renderer()->load(svgFiles.value(type));
}

/**
 * [PROTECTED]
 * Control the moviment of this item and don't permit that it's drowed outside
 * the scene rect
 */
QVariant VirtualMachineItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemPositionChange && scene())
	{
		// value is the new position.
		QPointF newPos = value.toPointF();
		QRectF rect = scene()->sceneRect();

		// Keep the item inside the scene rect. (- 2 is a padding)
		newPos.setX(qMin(rect.right() - boundingRect().width() - 2,
				qMax(newPos.x(), rect.left())));
		newPos.setY(qMin(rect.bottom()- boundingRect().height() - 2,
				qMax(newPos.y(), rect.top())));

		return newPos;
	}
	
	return QGraphicsItem::itemChange(change, value);
}

