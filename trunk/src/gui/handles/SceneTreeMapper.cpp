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

#include "SceneTreeMapper.h"

/* Init che instance field to NULL */
SceneTreeMapper* SceneTreeMapper::instance = NULL;

/**
 * Constructor
 */
SceneTreeMapper::SceneTreeMapper() : QObject()
{
	rootElement = new QTreeWidgetItem();
	sceneTree = LabHandler::getInstance()->getMainWindow()->sceneTree;
}

/**
 * Deconstructor
 */
SceneTreeMapper::~SceneTreeMapper()
{
}

/**
 * Singletone get instance
 */
SceneTreeMapper* SceneTreeMapper::getInstance()
{
	if (instance == NULL)
	{
		instance = new SceneTreeMapper();
	}

	return instance;
}

/**
 * Create the root element (the lab)
 */
void SceneTreeMapper::createRootElement()
{
	rootElement->setIcon(0, QIcon(LAB_ICON));
	sceneTree->addTopLevelItem(rootElement);
}

/**
 * Add a new vm element inside the tree as child of root and save the mapping
 */
void SceneTreeMapper::addVmElement(VirtualMachineItem* vmItem)
{
	QTreeWidgetItem* elem = new QTreeWidgetItem();
	
	elem->setData(0, Qt::DisplayRole, vmItem->getLabel());
	elem->setIcon(0, QIcon(VM_ICON));
	rootElement->addChild(elem);
	rootElement->setExpanded(true);
	
	/* save inside the map */
	vmMap.insert(vmItem, elem);
}

/**
 * Add a new cd element inside the tree as child of root and save the mapping
 */
void SceneTreeMapper::addCdElement(CollisionDomainItem* cdItem)
{
	QTreeWidgetItem* elem = new QTreeWidgetItem();
	
	//show only the collision domain name, not the entire network
	elem->setData(0, Qt::DisplayRole, cdItem->getLabel().split("\n").first());
	elem->setIcon(0, QIcon(CD_ICON));
	rootElement->addChild(elem);
	rootElement->setExpanded(true);
	
	/* save inside the map */
	cdMap.insert(cdItem, elem);
}

/**
 * Add a new link (ethernet) element inside the tree as
 * child of its virtual machine and save the mapping
 */
void SceneTreeMapper::addEthernetElement(LinkItem* linkItem)
{
	QTreeWidgetItem* elem = new QTreeWidgetItem();
	
	//show only the ethernet name, not the entire address
	elem->setData(0, Qt::DisplayRole,
			linkItem->getLabelItemPrivate()->text().split(" ").first());
	elem->setIcon(0, QIcon(ETH_ICON));
	
	//add as child of its vm
	vmMap.value(linkItem->getVirtualMachineItem())->addChild(elem);
	vmMap.value(linkItem->getVirtualMachineItem())->setExpanded(true);
	
	/* save inside the map */
	linkMap.insert(linkItem, elem);
}
