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
	propertyController = new VmPropertyController();
	
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
		propertyController->setVm(vm);
		pluginPropDialog->buildGui(vmPlugins, propertyController);
		pluginPropDialog->setVisible(true);
	}
	
}

/**
 * Handle the vmitem selection and render the properties
 */
void VmHandler::renderVmProperties(VirtualMachineItem *vmItem)
{
	/* Render properties building a new tree model */
	propertyController->setVm(VmMapper::getInstance()->getVm(vmItem));
	
	/* Clear the property editor */
	labHandler->getMainWindow()->clearPropertyDock(propertyController->getComposedModel());
}

/**
 * [SLOT]
 * Save a changed property for a virtual machine
 */
void VmHandler::saveChangedProperty(int row, int column)
{
//	/* Foreward action */
//	propertyController->saveChangedProperty(
//			labHandler->getMainWindow()->propertyTable->item(row, column));
}

void VmHandler::removePlugins(VirtualMachineItem *vmItem, QStringList pluginsToRemove)
{
	VirtualMachine *vm = VmMapper::getInstance()->getVm(vmItem);
	
	if(vm == NULL)
	{
		qWarning() << "Error: VmHandler::removePlugins broker mapping";
		return;
	}
	
	//unregister plugins
	QStringList pathsToDelete;
	
	QList<PluginProxy*> deletedPlugins = 
		PluginRegistry::getInstance()->unregisterVmPlugins(vm, pluginsToRemove, &pathsToDelete);
	
	//undo command
	LabHandler::getInstance()->getUndoStack()->push(new DeleteVmPluginsCommand(vm, deletedPlugins, pathsToDelete.toSet()));
	
	QString pluginsToString;
	/* Remove label inside shared area of the vm item */
	foreach(QString pName, pluginsToRemove)
	{
		vmItem->setPluginLine(pName, "");
		pluginsToString.append(pName).append(", ");
	}
	
	pluginsToString.remove(pluginsToRemove.size()-3, 2);
	
	emit logEvent(QString(vm->getName()).append(": ").append(tr("deleted plugins")).append(" [").append(pluginsToString).append("]"));
}

/**
 * Delete a virtual machine
 */
void VmHandler::deleteVm(VirtualMachineItem *vmItem)
{
	/* Get the low level object */
	VirtualMachine *vm = VmMapper::getInstance()->getMachine(vmItem);
	
	/* Get all links connected */
	QMap<LinkItem*, HardwareInterface*> links = LinkMapper::getInstance()->getMappingsByVm(vm);
	
	/* Destroy the mapper */
	VmMapper::getInstance()->destroyMapper(vmItem);
	
	/* Remove item from scene */
	labHandler->removeItemFromScene(vmItem);
	
	/* Remove connected links from the scene and from the three scene view */
	foreach(LinkItem* link, links.keys())
	{
		labHandler->removeItemFromScene(link);
		SceneTreeMapper::getInstance()->removeLink(link, link->getVirtualMachineItem());
	}
	
	/* get all interfaces proxies */
	QMap<HardwareInterface*, PluginProxy*> hiProxies;	//used with insert-multi
	foreach(HardwareInterface* hi, links.values())
	{
		foreach(PluginProxy *p, PluginRegistry::getInstance()->takeHiProxies(hi))
			hiProxies.insertMulti(hi, p);
	}
	
	QList<PluginProxy*> vmProxies;
	/* Take proxies for this virtual machine */
	vmProxies = PluginRegistry::getInstance()->takeVmProxies(vm);
	
	/* Remove virtual machine from low-level */
	LabFacadeController::getInstance()->removeVm(vm);
	
	/* Create the undo command */
	labHandler->getUndoStack()->push(new DeleteVmCommand(vm, vmItem, vmProxies, links, hiProxies));
	
	/* Remove the Virtual machine from tree scene view */
	SceneTreeMapper::getInstance()->removeVm(vmItem);
	
	/* Clear tree lab */
	QString temp = vm->getName();
	LabHandler::getInstance()->removePathFromTree(temp, true);
	LabHandler::getInstance()->removePathFromTree(temp.append(".startup"), true);
	
	/* Clear property dock content */
	LabHandler::getInstance()->getMainWindow()->clearPropertyDock();
}
