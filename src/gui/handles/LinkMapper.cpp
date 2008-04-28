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

#include "LinkMapper.h"

/**
 * Init the null instance for the singletone controller
 */
LinkMapper* LinkMapper::instance = NULL;

/**
 * Constructor
 */
LinkMapper::LinkMapper() : QObject()
{
	addLinkForm = new AddLinkForm();
}

/**
 * Deconstructor
 */
LinkMapper::~LinkMapper()
{
}


/**
 * Singletone get instance
 */
LinkMapper* LinkMapper::getInstance()
{
	if (instance == NULL)
	{
		instance = new LinkMapper();
	}

	return instance;
}

/**
 * [PUBLIC-SLOT]
 * Show the dialog and refresh it
 */
void LinkMapper::showAddLinkForm(VirtualMachineItem *vm, CollisionDomainItem* cd)
{
	addLinkForm->updateItems(vm, cd);
	addLinkForm->show();
}

/**
 * Adde the new mapping passed
 */
void LinkMapper::addNewMapping(LinkItem *linkItem, HardwareInterface* hi)
{
	//add mapping
	mappings.insert(linkItem, hi);
	LabHandler *labHandler = LabHandler::getInstance();
	
	//add item inside the scene
	labHandler->getMainWindow()->graphicsView->scene()->addItem(linkItem);
	labHandler->getMainWindow()->graphicsView->ensureVisible(linkItem);
	
	//update system log
	labHandler->getMainWindow()->writeLogMessage(
			tr("Added a new link: ") + hi->getMyVirtualMachine()->getName()
			+ "[" + hi->getName() + "] ---> " + hi->getMyCollisionDomain()->getName());
}

