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
#include "CdMapper.h"
#include "SceneTreeMapper.h"

/**
 * Init the null instance for the singletone controller
 */
LinkMapper* LinkMapper::instance = NULL;

/**
 * Constructor
 */
LinkMapper::LinkMapper() : QObject()
{
	addLinkForm = NULL;
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
	//lazy init
	if(addLinkForm == NULL)
		addLinkForm = new AddLinkForm();
	
	addLinkForm->updateItems(vm, cd);
	addLinkForm->show();
}

/**
 * Adds the new mapping passed
 */
void LinkMapper::addNewMapping(LinkItem *linkItem, HardwareInterface* hi)
{
	//add mapping
	mappings.insert(linkItem, hi);
	
	//update the line style (up or down)
	linkItem->updateLinkState();
	
	LabHandler *labHandler = LabHandler::getInstance();
	
	//add item inside the scene
	labHandler->getMainWindow()->graphicsView->scene()->addItem(linkItem);
	labHandler->getMainWindow()->graphicsView->ensureVisible(linkItem);
	
	//update the collision domain item state
	CdMapper::getInstance()->updateCdCounter(hi->getMyCollisionDomain(), true);
	
	//add the eth/link inside the scene tree
	SceneTreeMapper::getInstance()->addEthernetElement(linkItem);
	
	//update system log
	labHandler->getMainWindow()->writeLogMessage(
			tr("Added a new link: ") + hi->getMyVirtualMachine()->getName()
			+ "[" + hi->getName() + "] ---> " + hi->getMyCollisionDomain()->getName());
}

/**
 * Returns a QList of QGraphicsItemGroup objects. These objects are graphics 
 * elments associed with the mapping.
 */
QList<LinkItem*> LinkMapper::getLinkItems()
{
	return mappings.keys();
}

/**
 * Changes the label associated to the pluginName for the LinkItem associated with li.
 */
void LinkMapper::changeGraphicsLabelHi(HardwareInterface *hi, QString pluginName, QString label)
{
	LinkItem *li = mappings.key(hi);
	if (li == NULL)
	{
		qWarning() << "LinkMapper::changeGraphicsLabel: null pointer to LinkItem.";
		return;
	}
	
	li->setPluginLine(pluginName, label);
	
}

/**
 * [SLOT]
 * Clear mapping
 */
void LinkMapper::clear()
{
	qDeleteAll(mappings.keys());	//values is destroyed by low level objects
	mappings.clear();
}

/**
 * Get the virtual machine name associated to the passed link
 */
QString LinkMapper::getVmName(LinkItem *linkItem)
{
	QString returnName;
	
	HardwareInterface *hi = mappings.value(linkItem);
	if(hi == NULL)
		qWarning() << "LinkMapper::getVmName: hi is NULL.";
	else
		returnName = hi->getMyVirtualMachine()->getName();
	
	return returnName;
	
}

/**
 * Get a filtred map that contain all pairs connected to the collision domain passed
 */
QMap<LinkItem*, HardwareInterface*> LinkMapper::getMappingsByCd(const CollisionDomain *cd)
{
	QMap<LinkItem*, HardwareInterface*> retVal(mappings);
	QMapIterator<LinkItem*, HardwareInterface*> it(retVal);
	
	/* Remove all paris that doesn't connected to *cd */
	while(it.hasNext())
	{
		it.next();
		if(!(it.value()->getMyCollisionDomain() == cd))
			retVal.remove(it.key());
	}
	
	return retVal;
}

/**
 * Get a filtred map that contain all pairs connected to the virtual machine passed
 */
QMap<LinkItem*, HardwareInterface*> LinkMapper::getMappingsByVm(const VirtualMachine *vm)
{
	QMap<LinkItem*, HardwareInterface*> retVal(mappings);
	QMapIterator<LinkItem*, HardwareInterface*> it(retVal);
	
	/* Remove all paris that doesn't connected to *cd */
	while(it.hasNext())
	{
		it.next();
		if(!(it.value()->getMyVirtualMachine() == vm))
			retVal.remove(it.key());
	}
	
	return retVal;
}
