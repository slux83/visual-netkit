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
 * [PRIVATE-SLOT]
 * Handle the user form confirm
 * pre-operations on the input
 */
void InitPluginsPropertiesDialog::handleUserConfirm()
{
//	QStringList keys = propertiesAssoc.keys();
//	
//	//for each plugin inside "pluginsToManage"
//	for (int j=0; j < pluginsToManage.size(); j++) 
//	{
//		int yesToAll = 0;
//		
//		//scan properties
//		for (int i=0; i < keys.size(); i++) 
//		{
//			QStringList l = keys.at(i).split(SEPARATOR);
//			QString pluginName = l.first();
//			QString propName = l.last();
//			
//			if (pluginsToManage.at(j)->getPlugin()->getName() == pluginName)
//			{
//				QString altMsg;
//				bool allok = true;
//				
//				if (yesToAll==QMessageBox::YesToAll)
//				{
//					pluginsToManage.at(j)->saveProperty(propName, propertiesAssoc.value(keys.at(i))->text());
//				}
//				else
//				{
//					allok = pluginsToManage.at(j)->saveProperty(propName, propertiesAssoc.value(keys.at(i))->text(), &altMsg);
//				}
//				
//				// some warning or error returned by initProperty function
//				if (!allok && (yesToAll!=QMessageBox::YesToAll))
//				{	
//					//Prepare the alert message
//					QString question =
//								tr("The plugin ") + pluginsToManage.at(j)->getPlugin()->getName() +
//								tr(" have returned a warning:\n") +
//								altMsg + ".\n\n" +
//								tr("Ignore this warning?");
//					
//					yesToAll = QMessageBox::question(this, tr("VisualNetkit - Warning"),
//						tr(question.toUtf8()),
//						QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::No,
//						QMessageBox::Yes);
//					
//					if(yesToAll == QMessageBox::Yes)
//					{
//						pluginsToManage.at(j)->saveProperty(propName, propertiesAssoc.value(keys.at(i))->text());
//					}
//					else if(yesToAll == QMessageBox::YesToAll)
//					{
//						pluginsToManage.at(j)->saveProperty(propName, propertiesAssoc.value(keys.at(i))->text());
//						yesToAll = QMessageBox::YesToAll;
//					}
//					else
//					{
//						propertiesAssoc.value(keys.at(i))->selectAll();
//						return;
//					}
//				}
//			}
//		}
//	}
//
//	close();
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
	
	
	
//	pluginsToManage = plugins;
//	
//	QListIterator<PluginProxy *> it(plugins);
//	
//	/* foreach plugin */
//	quint8 tabCount = 0;
//	while(it.hasNext())
//	{
//		// gets the current plugin proxy
//		PluginProxy *proxy = it.next();
//		
//		// if any property has to be set, build the UI
//		if(proxy->getPluginProperties().size() > 0)
//		{
//			QWidget *w = new QWidget();	//the container
//			QVBoxLayout *layout = new QVBoxLayout(w);
//			layout->setMargin(3);
//			layout->setSpacing(3);
//			
//			QListIterator<PluginProperty*> ii(proxy->getPluginProperties());
//			
//			/* foraech property */
//			while(ii.hasNext())
//			{
//				PluginProperty *prop = ii.next();
//				QLabel *label = new QLabel(prop->getName());
//				QLineEdit *lineEdit = new QLineEdit(prop->getDefaultValue());
//				lineEdit->setToolTip(prop->getDescription());
//				layout->addWidget(label);
//				layout->addWidget(lineEdit);
//				
//				// save mapping; the key is "PluginName|000n|PropertyName
//				QString orderString = QString::number(prop->getOrder());
//				orderString.prepend(QString().fill('0', 5 - orderString.size()));
//								
//				propertiesAssoc.insert(proxy->getPlugin()->getName() + 
//										SEPARATOR + orderString + 
//										SEPARATOR + prop->getName(),
//										lineEdit);
//			}
//			/* The plugin have at most one property */
//			QSpacerItem *spacerItem = 
//				new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
//			layout->addItem(spacerItem);
//			pluginsToolBox->insertItem(tabCount++, w, QIcon(QString::fromUtf8(":/small/plugin")),
//				proxy->getPlugin()->getName());
//		}
//	}
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
	
	QStringList splittedId = actionData[1].split(SEPARATOR);
	//PLUGIN_NAME{SEPARATOR}PROPERTY_ID{SEPARATOR}PROPERTY_COPY

	
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
				model->setData(child0, QVariant(p->getName()), Qt::EditRole);
				model->setData(child1, QVariant(p->getDefaultValue()), Qt::EditRole);
				
				if(model->data(child0, Qt::EditRole).toString().isNull())
				{
					qDebug() << "BOOM";
				}
				
				//Expand and resize view
				treeView->expandAll();
				for(int column=0; column < treeView->model()->columnCount(); ++column)
					treeView->resizeColumnToContents(column);

				
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
			}
			else
				qWarning() << "unconsistance";
		}
		else
		{
			//Show a warning
			QMessageBox::warning(this, "Warning - VisualNetkit", response.second, QMessageBox::Ok);
		}
	}
}
