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


/**
 * Contructor
 * by default, this svg item is showed as VmHost
 */
VirtualMachineItem::VirtualMachineItem(VmType type) 
	: QGraphicsSvgItem(QString::fromUtf8(":/svg/vm_host"))
{
	/* Fill the filemap */
	svgFiles.insert(Host, QString::fromUtf8(":/svg/vm_host"));
	svgFiles.insert(Router, QString::fromUtf8(":/svg/vm_router"));
	
	/* Set the default svg file: VmHost */
	renderer()->load(svgFiles.value(type));
	
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
 * [PUBLIC-SLOT]
 * Change the visualization for this vm
 */
void VirtualMachineItem::changeSvgFile(VmType type)
{
	if(!svgFiles.contains(type))
	{
		qWarning() << "VmType" << type << "NOT FOUND INSIDE THE MAP";
		return;
	}
	
	renderer()->load(svgFiles.value(type));
}


//----  ADDED  BY  PAOLO  ----//

QList<SvgItemLink *> VirtualMachineItem::getLinks()
{
	return links;
}

void VirtualMachineItem::removeLink(SvgItemLink *link) {
	links.removeAt(links.indexOf(link));
}

void VirtualMachineItem::removeLinks()
{
    foreach (SvgItemLink *link, links) {
        //link->startNode()->removeLink(link);
        //link->endNode()->removeLink(link);
        
    	//faccio il downcasting dei QGraphicsItem a seconda del loro tipo
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

void VirtualMachineItem::addLink(SvgItemLink *link)
{
    links.append(link);
}

void VirtualMachineItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    //scene()->clearSelection();
    setSelected(true);
    //myContextMenu->exec(event->screenPos());
}

QVariant VirtualMachineItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        foreach (SvgItemLink *link, links) {
            link->updatePosition();
        }
    }

    return value;
}


