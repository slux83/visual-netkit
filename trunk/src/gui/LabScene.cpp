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

#include "LabScene.h"
#include "handles/LabHandler.h"
#include "handles/VmMapper.h"
#include "handles/CdMapper.h"
#include "handles/LinkMapper.h"

/**
 * Constructor: the scene size is Normal = 1000x1000
 */
LabScene::LabScene() : QGraphicsScene(0, 0, 1000, 1000)
{
	border = new QGraphicsRectItem();
	
	//the pen & flags
	QPen pen(Qt::green, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
	border->setFlags(QGraphicsItem::ItemClipsToShape);
	border->setPen(pen);
	//set the rect
	border->setRect(sceneRect());
	border->setFlags(QGraphicsItem::ItemClipsToShape);
	border->setZValue(100);
	
	/* Adde the border to this scene */
	addItem(border);
	
	connect(this, SIGNAL(sceneRectChanged(QRectF)), this, SLOT(adjustSceneBorder(QRectF)));
	
	link = NULL;
}

/**
 * Deconstructor
 */
LabScene::~LabScene()
{
}

/**
 * Mouse press event
 */
void LabScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{	
	MainWindow *m = LabHandler::getInstance()->getMainWindow();
	
	/* The user want to add an element? (virtual machine) */
	if(mouseEvent->button() == Qt::LeftButton && 
			m->actionAddVirtualMachine->isChecked())
	{
		qDebug() << "Adding a new virtual machine.";
		
		VmMapper::getInstance()->showAddVmForm(mouseEvent->scenePos());
	}
	
	/* The user want to add an element? (collision domain) */
	if(mouseEvent->button() == Qt::LeftButton && 
			m->actionAddCollisionDomain->isChecked())
	{
		qDebug() << "Adding a new collision domain.";
		
		CdMapper::getInstance()->showAddCdForm(mouseEvent->scenePos());
	}
	
	/* The user want to add a link? */
	if(mouseEvent->button() == Qt::LeftButton && 
			m->actionAddLink->isChecked() &&
			itemAt(mouseEvent->scenePos())->type() == QGraphicsItem::UserType + SvgItem)
	{
		link = initNewLinkLine();
		link->setLine(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
		addItem(link);	//the fake link is added inside the scene...
	}
	
	/* default action when the arrow icon is checked */
	if(m->actionManageGraph->isChecked())
	{
		QGraphicsScene::mousePressEvent(mouseEvent);	
	}

}

/**
 * Mouse move event
 */
void LabScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	/* user is adding a new link? */
	if(link != NULL)
	{
		QLineF linkNewPos(link->line().p1(), mouseEvent->scenePos());
		link->setLine(linkNewPos);
	}
	
	QGraphicsScene::mouseMoveEvent(mouseEvent);
}

/**
 * Mouse release event
 */
void LabScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	/* user is adding a new link? */
	if(link != NULL)
	{
		/* Get start and end nodes */
		QGraphicsItem *startNode = itemAt(link->line().p1());
		QGraphicsItem *endNode = itemAt(link->line().p2());
		
		if(startNode == NULL || endNode == NULL)
		{
			removeItem(link);
			delete link;
			link = NULL;
			return;
		}
		
		/* cast nodes dynamically */
		SvgItemPrivate *startSvgNode = dynamic_cast<SvgItemPrivate*>(startNode);
		SvgItemPrivate *endSvgNode = dynamic_cast<SvgItemPrivate*>(endNode);
		
		if(startSvgNode == NULL || endSvgNode == NULL)
		{
			removeItem(link);
			delete link;
			link = NULL;
			return;
		}
		
		/* now, get the source and destination groups */
		CollisionDomainItem *cdItem;
		VirtualMachineItem *vmItem;
		if(startSvgNode->getGroup()->type() == QGraphicsItem::UserType + VmItem)
		{
			vmItem = dynamic_cast<VirtualMachineItem*>(startSvgNode->getGroup());
			cdItem = dynamic_cast<CollisionDomainItem*>(endSvgNode->getGroup());
		}
		else if(startSvgNode->getGroup()->type() == QGraphicsItem::UserType + CdItem)
		{
			vmItem = dynamic_cast<VirtualMachineItem*>(endSvgNode->getGroup());
			cdItem = dynamic_cast<CollisionDomainItem*>(startSvgNode->getGroup());
		}
		else
		{
			qWarning()	<< "casting failed during add link:"
						<< "startType =" << startSvgNode->getGroup()->type()
						<< "endType =" << endSvgNode->getGroup()->type();
			
			removeItem(link);
			delete link;
			link = NULL;
			
			return;
		}
		
		/* another test */
		if(cdItem == NULL || vmItem == NULL)
		{
			removeItem(link);
			delete link;
			link = NULL;
			
			return;
		}
		
		/* show the gui */
		LinkMapper::getInstance()->showAddLinkForm(vmItem, cdItem);
		
		/* and finally, clear the fake line */
		removeItem(link);
		delete link;
		link = NULL;
	}
	
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

/**
 * [PRIVATE-SLOT]
 * Adjust the border of this scene when the user resize the scene rect
 */
void LabScene::adjustSceneBorder(QRectF r)
{
	border->setRect(r);
}

/**
 * [PRIVATE]
 * Create a new instance of line (che future link beetween two items)
 */
QGraphicsLineItem* LabScene::initNewLinkLine()
{
	//this link is only a fake. the true line will be created later...
	QGraphicsLineItem *link = new QGraphicsLineItem();
	link->setPen(QPen(Qt::blue, 4, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
	
	return link;
}