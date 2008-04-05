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

#include "VmMapper.h"
#include "LabHandler.h"

/**
 * Init the null instance for the singletone controller
 */
VmMapper* VmMapper::instance = NULL;

/**
 * Constructor
 */
VmMapper::VmMapper() : QObject()
{
	addVm = new AddVmForm();
	
	/* Connect signals */
	connect(this, SIGNAL(newMappingCreated(VirtualMachine *)),
			LabHandler::getInstance(), SLOT(addCreatedVmOnTree(VirtualMachine *)));
}

/**
 * Deconstructor
 */
VmMapper::~VmMapper()
{
}

/**
 * Singletone get instance
 */
VmMapper * VmMapper::getInstance()
{
	if (instance == NULL)
	{
		instance = new VmMapper();
	}

	return instance;
}

/**
 * Show the form
 */
void VmMapper::showAddVmForm()
{
	addVm->show();
}

/**
 * Add a new mapping (view, domain) and pass the view item to the scene
 */
void VmMapper::addNewMapping(VirtualMachineItem* vmItem, VirtualMachine* vm)
{
	//add mapping
	mappings.insert(vmItem, vm);
	LabHandler *labHandler = LabHandler::getInstance();
	labHandler->getMainWindow()->graphicsView->scene()->addItem(vmItem);
	labHandler->getMainWindow()->graphicsView->ensureVisible(vmItem);
		
	//emit signal
	emit newMappingCreated(vm);
}
