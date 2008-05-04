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

#include "CdHandler.h"
#include "../../core/CollisionDomain.h"
#include "../CollisionDomainItem.h"
#include "../undoCommands/UndoCommands.h"

/* Init che instance field to NULL */
CdHandler* CdHandler::instance = NULL;

/**
 * Constructor
 */
CdHandler::CdHandler()
{
	/* Get the controller (Domain side) */
	labFacadeController = LabFacadeController::getInstance();
	
	/* view side */
	labHandler = LabHandler::getInstance();
	propertyController = new CdPropertyController();
}

/**
 * Deconstructor
 */
CdHandler::~CdHandler()
{
}

/**
 * Singletone get instance
 */
CdHandler* CdHandler::getInstance()
{
	if (instance == NULL)
	{
		instance = new CdHandler();
	}

	return instance;
}

/**	
 * Check if the name passed is owned by a collision domain
 */
bool CdHandler::cdNameExist(QString cdNameToCheck)
{
	return labFacadeController->getCurrentLab()->cdExist(cdNameToCheck);
}

/**
 * [PUBLIC-SLOT]
 * Handle the signal for inserte a new collision domain
 */
void CdHandler::handleAddNewCd(QString cdName, NetworkAddress *subnet, QPointF pos)
{	
	/* Create the view and domain objects */
	CollisionDomain *cd = new CollisionDomain(cdName, subnet);
	
	CollisionDomainItem *cdItem = new CollisionDomainItem(cdName + "\n" + subnet->toString(true));
	
	cdItem->setPos(pos);	//place the item wher user clicked
	
	labFacadeController->getCurrentLab()->addCollisionDomain(cd);
	
	/* the undo command (redo) can accomplish the action */
	labHandler->getUndoStack()->push(new AddCdCommand(cdItem, cd));
	
	/* reset the default action (manage graph) */
	labHandler->getMainWindow()->forceManageGraphAction();
}

/**
 * Handle the cditem selection and render the properties
 */
void CdHandler::renderCdProperties(CollisionDomainItem *cdItem)
{
	/* Disconnect the old handler */
	disconnect(labHandler->getMainWindow()->propertyTable, 
			SIGNAL(cellChanged(int, int)), 0, 0);
	
	/* Clear the property editor */
	labHandler->getMainWindow()->clearPropertyDock();
	
	/* Render properties */
	propertyController->setCd(CdMapper::getInstance()->getCD(cdItem));
	propertyController->renderCdProperties(labHandler->getMainWindow()->propertyTable);
	
	/* Connect the correct handler dinamically */
	connect(labHandler->getMainWindow()->propertyTable, SIGNAL(cellChanged(int, int)), 
		this, SLOT(saveChangedProperty(int, int)));
}

/**
 * [SLOT]
 * Save a changed property for a collision domain
 */
void CdHandler::saveChangedProperty(int row, int column)
{
	/* Foreward action */
	propertyController->saveChangedProperty(
			labHandler->getMainWindow()->propertyTable->item(row, column));
}
