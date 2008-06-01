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

#include "ManagePluginsDialog.h"
#include "../plugin_framework/PluginLoaderFactory.h"
#include "../plugin_framework/PluginRegistry.h"
#include "handles/VmMapper.h"
#include "handles/LinkMapper.h"
#include "handles/CdMapper.h"

/**
 * Constructor
 */
ManagePluginsDialog::ManagePluginsDialog(QGraphicsItemGroup *bElement, QWidget *parent)
	: QDialog(parent)
{
	/* Connect the UI resource to this QWidget */
	setupUi(this);
	
	baseElement = bElement;
	allPlugins = PluginRegistry::getInstance()->getAllPluginFactories();
	
	/* Connects */
	connect(pluginsList, SIGNAL(itemClicked(QListWidgetItem *)),
		this, SLOT(showPluginInfos(QListWidgetItem *)));
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(handleUserConfirm()));
}

/**
 * Deconstructor
 */
ManagePluginsDialog::~ManagePluginsDialog()
{
}

/**
 * Build the gui
 */
void ManagePluginsDialog::buildGui()
{
	clearPluginsList();
	elemPlugins.clear();
	
	QString pluginType;
	QList<PluginProxy*> usedPlugins;
	PluginRegistry *registry = PluginRegistry::getInstance();
	
	/* Downcasting */
	VirtualMachineItem *vmItem = dynamic_cast<VirtualMachineItem*>(baseElement);
	if(vmItem != NULL)
	{
		pluginType = "vm";
		usedPlugins = registry->getVmProxies(VmMapper::getInstance()->getVm(vmItem));
	}
	
	CollisionDomainItem *cdItem = dynamic_cast<CollisionDomainItem*>(baseElement);
	if(cdItem != NULL)
	{
		pluginType = "cd";
		usedPlugins = registry->getCdProxies(CdMapper::getInstance()->getCD(cdItem));
	}
	
	LinkItem *linkItem = dynamic_cast<LinkItem*>(baseElement);
	if(linkItem != NULL)
	{
		pluginType = "link";
		usedPlugins = registry->getHiProxies(LinkMapper::getInstance()->getHardwareIterface(linkItem));
	}
	
	/* LIST to MAP for a fast access inside the build loop */
	QMap<QString, PluginProxy*> usedPluginsMap;
	QListIterator<PluginProxy*> iter(usedPlugins);
	while(iter.hasNext())
	{
		PluginProxy *p = iter.next();
		usedPluginsMap.insert(p->getPlugin()->getName(), p);
		elemPlugins << p->getPlugin()->getName();
	}
	
	/* Building */
	QListIterator<PluginLoaderFactory*> it(allPlugins);
	while(it.hasNext())
	{
		PluginLoaderFactory *factory = it.next();

		if(factory->getType() != pluginType)
			continue;
		
		//create entry
		QListWidgetItem *pluginItem = new QListWidgetItem();
		pluginItem->setIcon(QIcon(QString::fromUtf8(":/small/plugin")));
		pluginItem->setData(Qt::DisplayRole, factory->getName());	//it's the unique ID
		pluginItem->setData(Qt::ToolTipRole, tr("Select this plugin to show extra infos."));
		pluginItem->setCheckState(
				(usedPluginsMap.contains(factory->getName()))? Qt::Checked : Qt::Unchecked);
		
		pluginsList->addItem(pluginItem);
	}
}

/**
 * [PRIVATE-SLOT]
 * show the infos of the selected plugin
 */
void ManagePluginsDialog::showPluginInfos(QListWidgetItem *item)
{
	QString selectedPluginName = item->data(Qt::DisplayRole).toString();
	QListIterator<PluginLoaderFactory*> it(allPlugins);
	while(it.hasNext())
	{
		PluginLoaderFactory *factory = it.next();
		
		if(factory->getName() == selectedPluginName)
		{
			/* Render infos */
			pName->setText(factory->getName());
			pDescription->setText(factory->getDescription());
			pDeps->setText(factory->getDeps());
			pAuthor->setText(factory->getAuthor());
			pVersion->setText(factory->getVersion());
			
			break;
		}
	}
}

/**
 * [PRIVATE]
 * Clear all q tree widget items
 */
void ManagePluginsDialog::clearPluginsList()
{
	for(int i=0; i<pluginsList->count(); i++)
	{
		QListWidgetItem *item = pluginsList->takeItem(i);
		if(item != NULL)
			delete item;
	}
}

/**
 * [PRIVATE-SLOT]
 * Handle the user confirm
 */
void ManagePluginsDialog::handleUserConfirm()
{
	QStringList toAdd, toDelete;
	toAdd = getAddedPlugins();
	toDelete = getDeletedPlugins();
	
	QString question;
	if(toAdd.size() > 0)
		question += tr("Plugins to add:") + "\n" + toAdd.join(", ") + "\n";
	if(toDelete.size() > 0)
		question += tr("Plugins to delete:") + "\n" + toDelete.join(", ") + "\n";
	
	if(toAdd.size() > 0 || toDelete.size() > 0)
		question += "\nDo you want apply this changes?";
	
	if(question.isEmpty())
		return;
	
	QMessageBox::StandardButton resp =
		QMessageBox::question(this, tr("Visual Netkit - question"), question,
		QMessageBox::Apply | QMessageBox::Cancel, QMessageBox::Cancel);
	
	if(resp != QMessageBox::Apply)
		return;
	
	/* delegate add and delete actions to the rispective handler */
	/* Downcasting */
	VirtualMachineItem *vmItem = dynamic_cast<VirtualMachineItem*>(baseElement);
	if(vmItem != NULL)
	{
		VmHandler::getInstance()->removePlugins(vmItem, toDelete);
	}
	
	CollisionDomainItem *cdItem = dynamic_cast<CollisionDomainItem*>(baseElement);
	if(cdItem != NULL)
	{
	
	}
	
	LinkItem *linkItem = dynamic_cast<LinkItem*>(baseElement);
	if(linkItem != NULL)
	{
	
	}
}

/**
 * [PRIVATE]
 * Get the added plugins
 */
QStringList ManagePluginsDialog::getAddedPlugins()
{
	QStringList addedPlugins;
	
	/* Get all list items and select only selected */
	QList<QListWidgetItem *> listItems =
		pluginsList->findItems(".+", Qt::MatchRegExp);
	
	QListIterator<QListWidgetItem *> itemIter(listItems);
	while(itemIter.hasNext())
	{
		QListWidgetItem * item = itemIter.next();
		//save the plugin name if the plugin is "added"
		if(item->checkState() == Qt::Checked &&
				!elemPlugins.contains(item->data(Qt::DisplayRole).toString()))
			addedPlugins << item->data(Qt::DisplayRole).toString();
	}
	
	return addedPlugins;
}

/**
 * [PRIVATE]
 * Get the deleted plugins
 */
QStringList ManagePluginsDialog::getDeletedPlugins()
{
	QStringList deletedPlugins;
	
	/* Get all list items and select only selected */
	QList<QListWidgetItem *> listItems =
		pluginsList->findItems(".+", Qt::MatchRegExp);
	
	QListIterator<QListWidgetItem *> itemIter(listItems);
	while(itemIter.hasNext())
	{
		QListWidgetItem * item = itemIter.next();
		//save the plugin name if the plugin is "deleted"
		if(item->checkState() == Qt::Unchecked &&
				elemPlugins.contains(item->data(Qt::DisplayRole).toString()))
			deletedPlugins << item->data(Qt::DisplayRole).toString();
	}
	
	return deletedPlugins;
}
