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

#include "VmHandler.h"
#include "../../plugin_framework/PluginRegistry.h"
#include "../undoCommands/UndoCommands.h"

/**
 * Init the null instance for the singletone controller
 */
VmHandler* VmHandler::instance = NULL;

/**
 * Constructor
 */
VmHandler::VmHandler() : QObject()
{
	/* Get the controller (Domain side) */
	labFacadeController = LabFacadeController::getInstance();
	vmFacadeController = VmFacadeController::getInstance();
	
	/* Get controller (View side) */
	labHandler = LabHandler::getInstance();
	
	pluginPropDialog = new InitPluginsPropertiesDialog(
			PluginRegistry::getInstance()->getAllPluginFactories());
}

/**
 * Deconstructor
 */
VmHandler::~VmHandler()
{
}

/**
 * Singletone get instance
 */
VmHandler* VmHandler::getInstance()
{
	if (instance == NULL)
	{
		instance = new VmHandler();
	}

	return instance;
}

/**
 * Check if the name passed is owned by a virtual machine
 */ 
bool VmHandler::vmNameExist(QString vmNameToCheck)
{
	//controller->lab->check
	return labFacadeController->getCurrentLab()->vmExist(vmNameToCheck);
}

/**
 * [SLOT]
 * Create a new vm
 */
void VmHandler::createVm(QString vmNewName, QStringList selectedPlugins, 
		bool manuallyInit, QPointF pos)
{
	/* Create the view and domain objects */
	VirtualMachine *vm = vmFacadeController->createNewVirtualMachine(vmNewName);
	
	VirtualMachineItem *vmItem = new VirtualMachineItem(vm->getName());
	
	vmItem->setPos(pos);	//place the new machine where user clicked
	
	/* Create plugins and attach these to the vm */
	QList<PluginProxy *> vmPlugins;
	QStringListIterator iter(selectedPlugins);
	PluginRegistry* registry = PluginRegistry::getInstance();
	
	/* Save proxies for undo command */
	while(iter.hasNext())
	{
		PluginProxy* proxy;
		if((proxy = registry->registerPlugin(iter.next(), vm)) != NULL)
			vmPlugins.append(proxy);
	}
	
	labFacadeController->getCurrentLab()->addMachine(vm);
	
	/* the undo command (redo) can accomplish the action */
	labHandler->getUndoStack()->push(new AddVmCommand(vmItem, vm, vmPlugins));
	
	/* reset the default action (manage graph) */
	labHandler->getMainWindow()->forceManageGraphAction();
	
	/* now check if the user want init manually the plugins properties */
	if(manuallyInit && selectedPlugins.size() > 0)
	{
		pluginPropDialog->buildGui(vmPlugins);
		pluginPropDialog->setVisible(true);
	}
	
}


