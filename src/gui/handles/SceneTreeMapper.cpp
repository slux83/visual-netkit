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
#include "CdHandler.h"
#include "VmHandler.h"
#include "LinkHandler.h"

/* Init che instance field to NULL */
SceneTreeMapper* SceneTreeMapper::instance = NULL;

/**
 * Constructor
 */
SceneTreeMapper::SceneTreeMapper() : QObject()
{
	rootElement = new QTreeWidgetItem();
	sceneTree = LabHandler::getInstance()->getMainWindow()->sceneTree;
	
	//connect the handler
	connect(sceneTree, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
			this, SLOT(handleItemClicked(QTreeWidgetItem*, int)));
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
	//fixed to null by clear() function
	if(rootElement == NULL)
		rootElement = new QTreeWidgetItem();
	
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
	
	//show the collision domain name
	elem->setData(0, Qt::DisplayRole, cdItem->getLabel());
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
	
	//show the ethernet name
	elem->setData(0, Qt::DisplayRole,
			linkItem->getLabelItemPrivate()->text());
	elem->setIcon(0, QIcon(ETH_ICON));
	
	//add as child of its vm
	vmMap.value(linkItem->getVirtualMachineItem())->addChild(elem);
	vmMap.value(linkItem->getVirtualMachineItem())->setExpanded(true);
	
	/* save inside the map */
	linkMap.insert(linkItem, elem);
}

/**
 * [PRIVATE-SLOT]
 * - handle che tree click
 * - connect the future handler (for property changes)
 * - delegate the property render
 */
void SceneTreeMapper::handleItemClicked(QTreeWidgetItem *item, int column)
{
	Q_UNUSED(column);
	
	/* find where *item is mapped */
	if(item == rootElement)
	{
		//mapped with the laboratory
		LabHandler::getInstance()->prepareRenderLabProperties();
	}
	
	if(linkMap.key(item, NULL) != NULL)
	{
		//mapped with a linkItem
		LinkHandler::getInstance()->renderLinkProperties(linkMap.key(item));
	}
	
	if(cdMap.key(item, NULL) != NULL)
	{
		//mapped with a collision domain item
		CdHandler::getInstance()->renderCdProperties(cdMap.key(item));
	}
	
	if(vmMap.key(item, NULL) != NULL)
	{
		VmHandler::getInstance()->renderVmProperties(vmMap.key(item));
	}
}

/**
 * Change the label for a collision domain
 */
void SceneTreeMapper::changeCdLabel(CollisionDomainItem *cdItem, QString newLabel)
{
	QTreeWidgetItem *node = cdMap.value(cdItem, NULL);
	if(node == NULL)
	{
		qWarning() << "SceneTreeMapper::changeCdLabel Broken mapping.";
		return;
	}
	
	node->setData(0, Qt::DisplayRole, newLabel);
}

/**
 * Change the label for a virtual machine
 */
void SceneTreeMapper::changeVmLabel(VirtualMachineItem *vmItem, QString newLabel)
{
	QTreeWidgetItem *node = vmMap.value(vmItem, NULL);
	if(node == NULL)
	{
		qWarning() << "SceneTreeMapper::changeVmLabel Broken mapping.";
		return;
	}
	
	node->setData(0, Qt::DisplayRole, newLabel);
}

/**
 * Change the label for a Link
 */
void SceneTreeMapper::changeLinkLabel(LinkItem *linkItem, QString newLabel)
{
	QTreeWidgetItem *node = linkMap.value(linkItem, NULL);
	if(node == NULL)
	{
		qWarning() << "SceneTreeMapper::changeLinkLabel Broken mapping.";
		return;
	}
	
	node->setData(0, Qt::DisplayRole, newLabel);
}

/**
 * [SLOT]
 * Clear mappings and contents
 */
void SceneTreeMapper::clear()
{
	sceneTree->clear();
	vmMap.clear();
	linkMap.clear();
	cdMap.clear();
	
	//root element is deleted by sceneTree clear() call
	rootElement = NULL;
}

/**
 * Remove a link from tree and other structures
 */
void SceneTreeMapper::removeLink(LinkItem *linkItem, VirtualMachineItem *parent)
{
	/* Get the parent node */
	QTreeWidgetItem *parentNode = vmMap.value(parent);
	if(!parentNode)
	{
		qWarning() << "SceneTreeMapper::removeLink unknown parent.";
		return;
	}
	
	/* Remove mapping */
	QTreeWidgetItem *node = linkMap.take(linkItem);
	if(!node)
	{
		qWarning() << "SceneTreeMapper::removeLink unknown link node.";
		return;
	}
	
	/* remove child */
	parentNode->removeChild(node);
	
	delete node;
}

/**
 * Remove a collision domain from tree and other structures
 */
void SceneTreeMapper::removeCd(CollisionDomainItem *cdItem)
{
	/* Get the Cd node */
	QTreeWidgetItem *cdNode = cdMap.take(cdItem);
	if(!cdNode)
	{
		qWarning() << "SceneTreeMapper::removeCd unknown cdNode.";
		return;
	}
	
	/* remove child */
	rootElement->removeChild(cdNode);
	
	delete cdNode;
}

