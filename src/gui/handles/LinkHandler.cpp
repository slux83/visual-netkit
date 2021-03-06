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
	propertyController = new LinkPropertyController();
	pluginPropDialog = new InitPluginsPropertiesDialog(
			PluginRegistry::getInstance()->getAllPluginFactories());
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
		QStringList selectedPlugins, bool manuallyInit, QString ethName, bool state)
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
	
	/* Create plugins and attach these to the vm */
	QList<PluginProxy *> linkPlugins;
	QStringListIterator iter(selectedPlugins);
	PluginRegistry* registry = PluginRegistry::getInstance();
	
	/* Save proxies for undo command */
	while(iter.hasNext())
	{
		PluginProxy* proxy;
		if((proxy = registry->registerPlugin(iter.next(), interface)) != NULL)
			linkPlugins.append(proxy);
	}
	
	/* the undo command (redo) can accomplish the action */
	labHandler->getUndoStack()->push(new AddLinkCommand(linkItem, interface, linkPlugins));
	
	/* reset the default action (manage graph) */
	labHandler->getMainWindow()->forceManageGraphAction();
	
	/* now check if the user want init manually the plugins properties */
	if(manuallyInit && selectedPlugins.size() > 0)
	{
		propertyController->setHi(interface);
		pluginPropDialog->buildGui(linkPlugins, propertyController);
		pluginPropDialog->setVisible(true);
	}
	
}

/**
 * Check if hi with passed name exist
 */
bool LinkHandler::hiNameExist(HardwareInterface *hi, QString toCheck)
{
	return hi->getMyVirtualMachine()->getInterfaces().contains(toCheck);
}

/**
 * Update a link name
 */
void LinkHandler::updateLinkName(HardwareInterface *hi, QString newName)
{
	hi->getMyVirtualMachine()->changeHiName(hi, newName);
}

/**
 * Handle the link item selection and render the properties
 */
void LinkHandler::renderLinkProperties(LinkItem *linkItem)
{
	/* Render properties building a new tree model */
	propertyController->setHi(LinkMapper::getInstance()->getHardwareIterface(linkItem));
	
	/* Clear the property editor */
	labHandler->getMainWindow()->clearPropertyDock(propertyController->getComposedModel());
}

/**
 * Delete link
 */
void LinkHandler::deleteLink(LinkItem *link)
{
	LinkMapper *mapper = LinkMapper::getInstance();
	
	/* Get the low level object */
	HardwareInterface *hi = mapper->getHardwareIterface(link);
	
	/* Destroy the mapper */
	mapper->destroyMapper(link);
	
	/* Remove item from scene */
	labHandler->removeItemFromScene(link);
	
	/* Decrease the collision domain link counter */
	link->getCollisionDomainItem()->increaseDecreaseLinkCounter(false);
	
	/* Take proxies for this link */
	QList<PluginProxy*> proxies = PluginRegistry::getInstance()->takeHiProxies(hi);
	
	/* Create the undo command */
	labHandler->getUndoStack()->push(new DeleteLinkCommand(hi, proxies, link));
	
	/* Remove link from vm (lov level) */
	hi->getMyVirtualMachine()->deleteHi(hi->getName());
	
	/* Remove the ethernet from tree scene view */
	SceneTreeMapper::getInstance()->removeLink(link, link->getVirtualMachineItem());
	
	/* Clear property dock content */
	LabHandler::getInstance()->getMainWindow()->clearPropertyDock();

}
