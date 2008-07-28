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
#include "handles/LinkHandler.h"

/**
 * Constructor: the scene size is Normal = 1000x1000
 */
LabScene::LabScene() : QGraphicsScene(0, 0, 1000, 1000)
{
	initBorder();
	
	connect(this, SIGNAL(sceneRectChanged(QRectF)), this, SLOT(adjustSceneBorder(QRectF)));
	
	link = NULL;
	selectionRect = NULL;
	resizing = false;
	resizeItem = NULL;
	
}

/**
 * Deconstructor
 */
LabScene::~LabScene()
{
}

/**
 * [PROTECTED-REIMPLE]
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
	
	/* The user want to add an area? */
	if(mouseEvent->button() == Qt::LeftButton && m->actionAddArea->isChecked())
	{
		AreaItem *aItem = new AreaItem();
		aItem->setPos(mouseEvent->scenePos());
		addItem(aItem);
		//TODO: save a refer to area item
		
		m->forceManageGraphAction();
	}

	/* default action when the arrow icon is checked */
	if(m->actionManageGraph->isChecked())
	{
		QGraphicsScene::mousePressEvent(mouseEvent);
	}
	
	/* selection tool active */
	if (m->actionSelectionTool->isChecked()) 
	{
		// draws the selection area on mouse move
		QRectF rect(mouseEvent->scenePos(), mouseEvent->scenePos());
		selectionRect = new QGraphicsRectItem();
		selectionRect->setRect(rect);
		selectionRect->setPen(QPen(Qt::black, 1, Qt::DashLine, Qt::SquareCap, Qt::RoundJoin));
    	addItem(selectionRect);
	}
	 
    /* User want resize an area */
    QGraphicsItem *item = itemAt(mouseEvent->scenePos());
    if(mouseEvent->button() == Qt::LeftButton && 
    		readyToResize(item, mouseEvent->scenePos()))
    {
    	resizeItem = static_cast<AreaItem*>(item);
    	resizing = true;
    	QGraphicsScene::mousePressEvent(mouseEvent);
    }

}

/**
 * [PROTECTED-REIMPLE]
 * Mouse move event
 */
void LabScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	MainWindow *m = LabHandler::getInstance()->getMainWindow();

	/* user is adding a new link? */
	if(link != NULL)
	{
		QLineF linkNewPos(link->line().p1(), mouseEvent->scenePos());
		link->setLine(linkNewPos);
	}
	
	// user is selecting items
	if (selectionRect != NULL && m->actionSelectionTool->isChecked())
	{
    	// updates selection area dimensions
    	QRectF newRect(selectionRect->rect().topLeft(), mouseEvent->scenePos());
    	selectionRect->setRect(newRect);
	}
	
	/* Resize the area */
	if(resizeItem && resizing && m->actionManageGraph->isChecked())
	{		
		//calculate resize factor
		qreal width = resizeItem->rect().width() +
			(mouseEvent->scenePos().x() - (resizeItem->scenePos().x() + resizeItem->rect().width()));
		qreal height = resizeItem->rect().height() +
			(mouseEvent->scenePos().y() - (resizeItem->scenePos().y() + resizeItem->rect().height()));
		
		/* Size control */
		if(width < 20)
			width = resizeItem->rect().width();
		if(height < 20)
			height = resizeItem->rect().height();
		
		resizeItem->setRect(0, 0, width , height);
		
	}
	
	if(!resizing)
		QGraphicsScene::mouseMoveEvent(mouseEvent);
}

/**
 * [PROTECTED-REIMPLE]
 * Mouse release event
 */
void LabScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	MainWindow *m = LabHandler::getInstance()->getMainWindow();

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
	
	if (selectionRect != NULL && m->actionSelectionTool->isChecked()) 
	{
    	// finds the items inside the selection rect
		QPainterPath path;
    	path.addRect(selectionRect->rect());
    	QList<QGraphicsItem*> itemList = items(path);
    	
    	// selects only the groups
    	for (int i=0; i<itemList.size(); i++)
    	{
    		if (itemList.at(i)->group() == NULL)
    		{
    			itemList.at(i)->setSelected(true);
    		}
    	}
    	
    	//clear
    	removeItem(selectionRect);
    	delete selectionRect;
    	selectionRect = NULL;
    	m->forceManageGraphAction();
    }
	
	//Stop resizing area
	if(resizing)
	{
		resizing = false;
		resizeItem = NULL;
	}
	
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

/**
 * [PROTECTED-REIMPLE]
 * Mouse double click event
 */
void LabScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	MainWindow *m = LabHandler::getInstance()->getMainWindow();
	
	/* The user double click */
	if(mouseEvent->button() == Qt::LeftButton && 
			m->actionManageGraph->isChecked())
	{
		/* Get one selected item */
		QList<QGraphicsItem*> selectedItems = this->selectedItems();
		
		if (!selectedItems.isEmpty())
		{
			QGraphicsItem *selectedItem = selectedItems.first();
			if(selectedItem != NULL)
			{
				switch(selectedItem->type())
				{
					//Virtual Machine item
					case QGraphicsItem::UserType + VmItem:
						VmHandler::getInstance()->renderVmProperties(
							dynamic_cast<VirtualMachineItem*>(selectedItem));
						break;
						
					//Collision Domain item
					case QGraphicsItem::UserType + CdItem:
						CdHandler::getInstance()->renderCdProperties(
							dynamic_cast<CollisionDomainItem*>(selectedItem));
						 break;
						
					//Link item
					case QGraphicsItem::UserType + LnkItem:
						LinkHandler::getInstance()->renderLinkProperties(
								dynamic_cast<LinkItem*>(selectedItem));
						break;
				}
			}
		}
	}
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
	link->setPen(QPen(Qt::yellow, 6, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	link->setZValue(20);
	
	return link;
}

/**
 * Clear the scene
 */
void LabScene::clearScene()
{
	clear();
	initBorder();
}

/**
 * [PRIVATE]
 * Init the border of the scene
 */
void LabScene::initBorder()
{
	border = new QGraphicsRectItem();
		
	//the pen & flags
	QPen pen(Qt::darkRed, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
	border->setFlags(QGraphicsItem::ItemClipsToShape);
	border->setPen(pen);
	//set the rect
	border->setRect(sceneRect());
	border->setZValue(1);
	
	/* Adds the border to this scene */
	addItem(border);
}

/**
 * [PRIVATE]
 * If the mouse is on the bottom-right corner of an area, the user can resize it
 */
bool LabScene::readyToResize(QGraphicsItem *item, QPointF mousePoint)
{
	AreaItem *aItem = static_cast<AreaItem*>(item);
	if(!aItem)
	{
		QApplication::restoreOverrideCursor();
		return false;
	}
	
	QPointF mousePosOnArea = item->mapFromScene(mousePoint);
	QRectF resizeRegion;
	QRectF itemBrect = item->boundingRect();
	resizeRegion.setX(itemBrect.width() - (pointSize * 2));	//+1 is the border size
	resizeRegion.setY(itemBrect.height() - (pointSize * 2));
	resizeRegion.setWidth(pointSize * 2);
	resizeRegion.setHeight(pointSize * 2);
		
	return resizeRegion.contains(mousePosOnArea);
}

