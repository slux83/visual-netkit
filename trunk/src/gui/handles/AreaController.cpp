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

/**
 * Constructor
 */
AreaController::AreaController()
{
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
}