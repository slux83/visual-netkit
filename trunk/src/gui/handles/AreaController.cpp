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

#include "AreaController.h"
#include "LabHandler.h"
#include "../AreaItem.h"
#include "../undoCommands/UndoCommands.h"

/* Init the NULL instance */
AreaController* AreaController::instance = NULL;

#define AREA_TEXT "AREA_TEXT"

/**
 * Constructor
 */
AreaController::AreaController()
{
	currentArea = NULL;
}

/**
 * Deconstructor
 */
AreaController::~AreaController()
{
}

/**
 * [STATIC]
 * Controller singletone get instance
 */
AreaController* AreaController::getInstance()
{
	if(instance == NULL)
		instance = new AreaController();
	
	return instance;
}

/**
 * Add a new area inside the areas list
 */
void AreaController::addArea(AreaItem* newArea)
{
	areas.append(newArea);
	
	//Set the log
	LabHandler::getInstance()->getMainWindow()->writeLogMessage(tr("Added an area"));
	
	//push to undo stack
	LabHandler::getInstance()->getUndoStack()->push(new AddAreaCommand(newArea));
}

/**
 * Foward to lab handler
 */
void AreaController::setChangedLabState()
{
	LabHandler::getInstance()->setChangedLabState();
}

/**
 * Destroy an area
 */
void AreaController::deleteArea(AreaItem *aItem)
{
	if(!aItem->scene())
	{
		qWarning() << "AreaController::deleteArea()" << "Area item have NULL scene";
		return;
	}
	
	//delete
	aItem->scene()->removeItem(aItem);
	areas.removeAll(aItem);
	
	//Set the log
	LabHandler::getInstance()->getMainWindow()->writeLogMessage(tr("Area deleted"));
	
	//push to undo stack
	LabHandler::getInstance()->getUndoStack()->push(new DeleteAreaCommand(aItem));
	
	//clear the property dock
	LabHandler::getInstance()->getMainWindow()->clearPropertyDock();
}

/**
 * Handle the link item selection and render the properties
 */
void AreaController::renderAreaProperties(AreaItem *area)
{
//	LabHandler *labHandler = LabHandler::getInstance();
//	
//	/* Disconnect the old handler */
//	disconnect(labHandler->getMainWindow()->propertyTable, 
//			SIGNAL(cellChanged(int, int)), 0, 0);
//	
//	/* Clear the property editor */
//	labHandler->getMainWindow()->clearPropertyDock();
//	
//	/* Render properties */
//	currentArea = area;
//	renderProperties(labHandler->getMainWindow()->propertyTable);
//	
//	/* Connect the correct handler dinamically */
//	connect(labHandler->getMainWindow()->propertyTable, SIGNAL(cellChanged(int, int)), 
//		this, SLOT(saveChangedProperty(int, int)));
}

/**
 * [SLOT]
 * Save the property changed
 */
void AreaController::saveChangedProperty(int row, int column)
{
//	QTableWidget *tWidget = LabHandler::getInstance()->getMainWindow()->propertyTable;
//	QTableWidgetItem *tItem = tWidget->item(row, column);
//	
//	if(tItem->data(Qt::UserRole).toString() == AREA_TEXT)
//	{
//		currentArea->setLabel(tItem->data(Qt::DisplayRole).toString());
//		LabHandler::getInstance()->getMainWindow()->writeLogMessage(tr("Area property saved"));
//	}
}

/**
 * [PRIVATE]
 * Render area properties inside the property dock
 */
/**
 * Render lab properties inside property dock
 */
void AreaController::renderProperties(QTableWidget *tableWidget)
{
//	if(currentArea == NULL)
//		return;
//	
//	/* render infos inside the property editor */
//	tableWidget->setRowCount(1);
//
//	QTableWidgetItem *property = new QTableWidgetItem();
//	
//	//Area text
//	property->setData(Qt::DisplayRole, tr("Text"));
//	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
//	tableWidget->setItem(0, 0, property);
//	
//	property = new QTableWidgetItem();
//	property->setData(Qt::DisplayRole, currentArea->getLabel());
//	property->setData(Qt::UserRole, AREA_TEXT);
//	tableWidget->setItem(0, 1, property);
}

/**
 * Clear the controller
 */
void AreaController::clear()
{
	currentArea = NULL;
	areas.clear();
}

/**
 * Load (create) a saved area
 */
void AreaController::loadNewArea(QPointF pos, qreal width,
		qreal height, QString color, QString text)
{
	//make a new area
	AreaItem *area = new AreaItem();
	area->setPos(pos);
	area->setRect(0, 0, width, height);
	
	//create the color
	QStringList rgb = color.split(',');
	QColor areaBg;
	if(rgb.size() == 3)
	{
		areaBg.setRed(rgb[0].toInt());
		areaBg.setGreen(rgb[1].toInt());
		areaBg.setBlue(rgb[2].toInt());
	}
	
	//attach the color if valid
	if(areaBg.isValid())
		area->setCurrentCorol(areaBg);
	else
		qWarning() << "Color" << color << "is invalid as area background." << "Setting default color.";
	
	//text
	area->setLabel(text);
	
	//save and draw
	areas.append(area);
	LabHandler::getInstance()->getMainWindow()->getLabScene()->addItem(area);
	
}
