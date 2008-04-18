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
	
	/* Test line */
/*	QGraphicsLineItem *line = new QGraphicsLineItem(150, 99, 100, 200);
	line->setZValue(1000);
	addItem(line);
	QGraphicsSimpleTextItem *label = new QGraphicsSimpleTextItem("bao bao");
	addItem(label);
	
	label->setPos(155, 100);
	label->rotate(line->line().angle(QLineF(0,0,1000,0)));
	label->rotate(180);
	
	qDebug() << "angle:" << line->line().angle(QLineF(0,0,1000,0));
*/
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
	
	/* The user want to add an element? */
	if(mouseEvent->button() == Qt::LeftButton && 
			m->actionAddVirtualMachine->isChecked())
	{
		qDebug() << "Adding a new virtual machine.";
		
		VmMapper::getInstance()->showAddVmForm(mouseEvent->scenePos());
	}
	
	if(mouseEvent->button() == Qt::LeftButton && 
			m->actionAddCollisionDomain->isChecked())
	{
		qDebug() << "Adding a new collision domain.";
		
		CdMapper::getInstance()->showAddCdForm(mouseEvent->scenePos());
	}
	
	QGraphicsScene::mousePressEvent(mouseEvent);
}

/**
 * Mouse move event
 */
void LabScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	QGraphicsScene::mouseMoveEvent(mouseEvent);
}

/**
 * Mouse release event
 */
void LabScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
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
