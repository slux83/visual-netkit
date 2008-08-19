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

#include "InitPluginsPropertiesDialog.h"
#include <QLineEdit>
#include <QScrollArea>
#include <QMessageBox>

#include "../plugin_framework/PluginProxy.h"
#include "../common/CommonConfigs.h"

/**
 * Constructor
 */
InitPluginsPropertiesDialog::InitPluginsPropertiesDialog(
		const QList<PluginLoaderFactory *> plugins, QWidget *parent) : QDialog(parent)
{
	/* Connect the UI resource to this QWidget */
	setupUi(this);
	
	/* save the factories (for a fast access) */
	availablePlugins = plugins;
	
	handler = NULL;
	
	//init actian menu
	actionMenu = new QMenu();
	actionsToolButton->setMenu(actionMenu);
	
	/* Connects */
	connect(treeView, SIGNAL(clicked(const QModelIndex&)),
		this, SLOT(itemClicked(const QModelIndex&)));
	
	connect(actionMenu, SIGNAL(triggered(QAction*)),
		this, SLOT(handleAction(QAction*)));
		
}

/**
 * Decontructor
 */
InitPluginsPropertiesDialog::~InitPluginsPropertiesDialog()
{	
}

/**
 * Build the tree view based on plugins properties's structure
 * getting the tree model from newHandler
 */
void InitPluginsPropertiesDialog::buildGui(QList<PluginProxy*> plugins,
		AbstractPropertyHandler *newHandler)
{	
	handler = newHandler;
	pluginsToManage = plugins;
	
	clearTreeView(handler->getInitModel(plugins));
	
	treeView->expandAll();
	
	for(int column=0; column < treeView->model()->columnCount(); ++column)
		treeView->resizeColumnToContents(column);
}

/**
 * [PRIVATE]
 * Clear the tree view from old model and set a newone
 */
void InitPluginsPropertiesDialog::clearTreeView(TreeModel *newModel)
{
	QAbstractItemModel *oldModel = treeView->model();
	
	treeView->setModel(newModel);
	
	if(oldModel)
		delete oldModel;
}

/**
 * [PRIVATE-SLOT]
 * Build/active/deactive the menu action based on node selected and its childs
 */
void InitPluginsPropertiesDialog::itemClicked(const QModelIndex& index)
{
	TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

	//invalid item
	if(!item)
	{
		actionsToolButton->setEnabled(false);
		return;
	}
	
	//Enable if item is a property
	actionsToolButton->setEnabled(item->isProperty());
	
	actionMenu->clear();
	
	if(!item->isPluginHeaderNode())
	{
		QAction *delAction = new QAction(QIcon(":/menu/list_remove"), tr("Remove selected property"), actionMenu);
		QStringList actionData; 
		actionData << "delete" << item->getId();
		delAction->setData(actionData);
		actionMenu->addAction(delAction);
		actionMenu->addSeparator();
	}
	
	foreach(ChildDescription cd, item->getChildDescription())
	{
		QAction *action = new QAction(QIcon(":/menu/list_add"), tr("Add") + " property: " + cd.name, actionMenu);
		QStringList actionData;
		actionData << "add" << item->getId() << cd.id; 
		
		action->setData(actionData);
		actionMenu->addAction(action);
	}
}

/**
 * [PRIVATE-SLOT]
 * Handle a property action
 */
void InitPluginsPropertiesDialog::handleAction(QAction * action)
{
	/* Get infos from action */
	QStringList actionData = action->data().toStringList();
	
	//PLUGIN_NAME{SEPARATOR}PROPERTY_ID{SEPARATOR}PROPERTY_COPY
	QStringList splittedId = actionData[1].split(SEPARATOR);

	if(actionData[0] == "delete")
	{
		QString response = handler->removePluginProperty(splittedId[0], splittedId[1], (quint16)splittedId[2].toInt());
		if(response.isEmpty())
		{
			//remove the tree item from tree view
			QModelIndex current = treeView->currentIndex();
			QAbstractItemModel *model = treeView->model();
			bool esite = model->removeRows(current.row(), 1, current.parent());
			actionsToolButton->setDisabled(esite);
			
			if(!esite)
				qWarning() << "WARNING: property view unconsistence; deleted the plugin property, but not the view item";
		}
		else
		{
			//Show a warning
			QMessageBox::warning(this, "Warning - VisualNetkit", response, QMessageBox::Ok);
		}
	}
	
	if(actionData[0] == "add")
	{
		QPair<PluginProperty*, QString> response = 
			handler->addPluginProperty(splittedId[0], actionData[2], splittedId[1],
					(quint16)splittedId[2].toInt());
		if(response.second.isEmpty())
		{
			//add a new tree item
			QModelIndex index = treeView->selectionModel()->currentIndex();
			TreeModel *model = static_cast<TreeModel*>(treeView->model());

			if(model->insertRows(0, 1, index))
			{
				PluginProperty *p = response.first;
				
				//Set columns value
				QModelIndex child0 = model->index(0, 0, index);
				QModelIndex child1 = model->index(0, 1, index);
				
				//This is an hack to prevent a stupid Qt? bug.
				while(!model->setData(child0, QVariant(p->getName()), Qt::EditRole)) {};
				while(!model->setData(child1, QVariant(p->getDefaultValue()), Qt::EditRole)) {};
				
				if(model->data(child0, Qt::EditRole).toString().isNull())
				{
					qDebug() << "OMFG OBSCURE BUG!!!";
				}
				
				//Expand and resize view
				treeView->expandAll();
				for(int column=0; column < treeView->model()->columnCount(); ++column)
					treeView->resizeColumnToContents(column);
				
				//Tooltip
				model->setData(child0, QVariant(p->getDescription()), Qt::ToolTipRole);
				model->setData(child1, QVariant(p->getDescription()), Qt::ToolTipRole);
				
				//Init the low level model (tree item of child0/1)
				TreeItem *ti = model->getItem(child0);
				ti->setProperty(true);
				
				//PLUGIN_NAME{SEPARATOR}PROPERTY_ID{SEPARATOR}PROPERTY_COPY
				ti->setId(splittedId[0] + SEPARATOR +
						p->getId() + SEPARATOR +
						QString::number(p->getCopy()));
				
				PluginProxy *proxy = handler->getPluginFromCurrentElement(splittedId[0]);
				if(!proxy)
				{
					qWarning() << "WARNING: [NULL PROXY] property view unconsistence; added the plugin property, but not the view item";
					return;
				}
				
				ti->appendChildsDescription(proxy->getPropertyExpert()->getChildsByParentId(p->getId()));
				
				ti->setPropertyHandler(handler);
				
				buildViewChildsDeeply(child0, p, proxy); //Recursive build
			}
			else
				qWarning() << "Fail creating view row.";
		}
		else
		{
			//Show a warning
			QMessageBox::warning(this, "Warning - VisualNetkit", response.second, QMessageBox::Ok);
		}
	}
}

/**
 * [PRIVATE]
 * Build view deeply starting from property childs
 */
void InitPluginsPropertiesDialog::buildViewChildsDeeply(
		QModelIndex &child, PluginProperty *prop, PluginProxy *proxy)
{
	TreeModel *model = static_cast<TreeModel*>(treeView->model());
	
	foreach(PluginProperty *p, prop->getChilds())
	{
		/* Add a sub-child */
		if(model->insertRows(0, 1, child))
		{		
			//Set columns value
			QModelIndex child0 = model->index(0, 0, child);
			QModelIndex child1 = model->index(0, 1, child);
			
			//This is an hack to prevent a stupid Qt? bug.
			while(!model->setData(child0, QVariant(p->getName()), Qt::EditRole)) {};
			while(!model->setData(child1, QVariant(p->getDefaultValue()), Qt::EditRole)) {};
			
			if(model->data(child0, Qt::EditRole).toString().isNull())
				qDebug() << "--> OMFG OBSCURE BUG!!!";
			
			//Tooltip
			model->setData(child0, QVariant(p->getDescription()), Qt::ToolTipRole);
			model->setData(child1, QVariant(p->getDescription()), Qt::ToolTipRole);
			
			//Expand and resize view
			treeView->expandAll();
			for(int column=0; column < treeView->model()->columnCount(); ++column)
				treeView->resizeColumnToContents(column);
		
			
			//Init the low level model (tree item of child0/1)
			TreeItem *ti = model->getItem(child0);
			ti->setProperty(true);
			
			//PLUGIN_NAME{SEPARATOR}PROPERTY_ID{SEPARATOR}PROPERTY_COPY
			ti->setId(proxy->getPlugin()->getName() + SEPARATOR +
					p->getId() + SEPARATOR +
					QString::number(p->getCopy()));
			
			ti->appendChildsDescription(proxy->getPropertyExpert()->getChildsByParentId(p->getId()));
			
			ti->setPropertyHandler(handler);
			
			buildViewChildsDeeply(child0, p, proxy);
		}
		else
			qWarning() << "Fail creating view row.";
	}
}
