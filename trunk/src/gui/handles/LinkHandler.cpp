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

#include "LinkHandler.h"

/**
 * Init the null instance for the singletone controller
 */
LinkHandler* LinkHandler::instance = NULL;

/**
 * Constructor
 */
LinkHandler::LinkHandler() : QObject()
{
	/* get domain side controllers */
	vmFacadeController = VmFacadeController::getInstance();
	
	/* get gui side controllers/mappers */
	vmMapper = VmMapper::getInstance();
	cdMapper = CdMapper::getInstance();
	labHandler = LabHandler::getInstance();
}

/**
 * Deconstructor
 */
LinkHandler::~LinkHandler()
{
}

/**
 * Singletone get instance
 */
LinkHandler* LinkHandler::getInstance()
{
	if (instance == NULL)
	{
		instance = new LinkHandler();
	}

	return instance;
}

/**
 * [PUBLIC-SLOT]
 * Handle the request for a new link beetween vm and cd
 */
void LinkHandler::createLink(VirtualMachineItem *vmItem, CollisionDomainItem *cdItem,
		QString ethName, bool state)
{	
	/* Get from mappers the domain objects */
	VirtualMachine *vm = vmMapper->getMachine(vmItem);
	CollisionDomain *cd = cdMapper->getCD(cdItem);
	
	//test mappings consistent
	if(vm == NULL || cd == NULL)
	{
		qWarning()	<< "LinkHandler::createLink()"
					<< "Mapping brocken Vm - Cd:" << vm << cd;
		return;
	}
	
	/**
	 * VmFacadeController is the creator for a new hardware interface
	 * for a virtual machine passed
	 */
	HardwareInterface *interface = 
		vmFacadeController->createNewHardwareIterface(vm, ethName, state, cd);
	
	/* create the link item (view side) */
	QString linkLabel(ethName);
	LinkItem *linkItem = new LinkItem(vmItem, cdItem, linkLabel);
	
	/* the undo command (redo) can accomplish the action */
	labHandler->getUndoStack()->push(new AddLinkCommand(linkItem, interface));
	
	/* reset the default action (manage graph) */
	labHandler->getMainWindow()->forceManageGraphAction();
	
}
