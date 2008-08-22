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

#include "LinkPropertyController.h"
#include "LinkHandler.h"
#include "../../plugin_framework/PluginRegistry.h"
#include "../../common/CommonConfigs.h"

//Base properties for a link
#define HI_NAME "HiName"
#define HI_STATE "HiState"

/**
 * Constructor
 */
LinkPropertyController::LinkPropertyController() : AbstractPropertyHandler()
{
	hi = NULL;
}

/**
 * Deconstructor
 */
LinkPropertyController::~LinkPropertyController()
{
}


/**
 * Save a changed property
 */
bool LinkPropertyController::saveChangedProperty(TreeItem *treeItem)
{
	bool ok = true;
	
	if(hi == NULL)
	{
		qWarning() << "LinkPropertyController::saveChangedProperty()" << "NULL Link";
		return false;
	}
	
	QString itemValue = treeItem->data(1).toString();
	
	/* This field is mine? */
	if(treeItem->getId() == HI_NAME)
	{
		/* Some checks */
		QRegExp validateEthName("^eth([0-9]|[1-9][0-9]+)");
		if(ok && !validateEthName.exactMatch(itemValue))
		{
			QMessageBox::warning(NULL, tr("Visual Netkit - Warning"),
               tr("The Hardware interface name must be validated by\n^eth([0-9]|[1-9][0-9]+)\nExamples: eth0 eth1 eth2 ... eth12 ..."),
               QMessageBox::Ok);
			
			return false;
		}
		
		if(LinkHandler::getInstance()->hiNameExist(hi, itemValue) && itemValue != hi->getName())
		{
			QMessageBox::warning(NULL, tr("Visual Netkit - Warning"),
               tr("The Hardware interface name must be unique!"),
               QMessageBox::Ok);
			
			return false;
		}
		
		/* save changes */
		LinkHandler::getInstance()->updateLinkName(hi, itemValue);
		LinkMapper::getInstance()->getLink(hi)->setLinkLabel(itemValue);
		
		return true;
		
	}
	
	if(treeItem->getId() == HI_STATE) //State field?
	{		
		/* Validate user input */
		QRegExp validateState("up|down");
		if(!validateState.exactMatch(itemValue))
		{
			QMessageBox::warning(NULL, tr("Visual Netkit - Warning"),
               tr("The Hardware interface state must be \"up\" or \"down\"\n(case insensitive)"),
               QMessageBox::Ok);
			
			return false;
		}
		
		/* save changes */
		hi->setState((itemValue == "up"));
		LinkMapper::getInstance()->getLink(hi)->updateLinkState();
		
		return true;
	}
	
	QStringList itemDataSplitted = treeItem->getId().split(SEPARATOR);
	if(itemDataSplitted.size() != 2)
	{
		qWarning() << "tree item id not standard:" << treeItem->getId();
		return false;
	}
	
	QString pluginName = itemDataSplitted[0];
	QString propUid = itemDataSplitted[1];
	
	QListIterator<PluginProxy*> i(PluginRegistry::getInstance()->getHiProxies(hi));
	
	/* Seach the plugin to send the change */
	while(i.hasNext())
	{
		PluginProxy *p = i.next();
		if(p->getPlugin()->getName() == pluginName)
		{
			QString alert;
			if(!p->saveProperty(propUid, itemValue, &alert))
			{
				int userChoice;
				
				userChoice = QMessageBox::question(NULL, tr("Visual Netkit - Warning"),
					tr(("Plugin Property error: " + alert + "\n\n" + "Do you want ignore this warning?").toUtf8()),
					QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
				
				ok = false;
				
				if(userChoice == QMessageBox::Yes)
				{
					//ok, store the value and shut up!
					ok = p->saveProperty(propUid, itemValue);
				}	
			}
			else
				ok = true;
			
			break;
		}
	}

	return ok;
}

/**
 * [IMPL]
 * Build and return the TreeModel to change plugins properties
 */
TreeModel* LinkPropertyController::getInitModel(QList<PluginProxy*> plugins)
{
	QStringList header;
	header << tr("Property") << tr("Value");
	TreeModel *model = new TreeModel(header);
	
	TreeItem *root = model->getRootItem();
	
	//now, for each plugin, map each property in a TreeItem.
	foreach(PluginProxy *proxy, plugins)
	{
		QVector<QVariant> pluginHeader;
		pluginHeader << "Plugin" << proxy->getPlugin()->getName();
		
		root->insertChildren(root->childCount(), 1, root->columnCount());
		for (int column = 0; column < pluginHeader.size(); ++column)
			root->child(root->childCount() - 1)->setData(column, pluginHeader.at(column));
		
		TreeItem* parent = root->child(root->childCount() - 1);
		
		parent->setProperty(true);	//Plugin node is a property root container; it could have actions.
		parent->setIsPluginHeader(true);
		
		parent->setPropertyHandler(this);
		
		parent->setId(proxy->getPlugin()->getName() + SEPARATOR);
		
		parent->appendChildsDescription(proxy->getPropertyExpert()->getRootChilds());
				
		clonePropertyTree(proxy, proxy->getPlugin()->getPluginProperties(), parent, root, true);
	}
	
	return model;
}

/**
 * [IMPL]
 * Build and return a tree model with also vm properties
 */
TreeModel* LinkPropertyController::getComposedModel()
{
	QStringList header;
	header << tr("Property") << tr("Value");
	TreeModel *model = new TreeModel(header);
	
	if(!hi)
	{
		qWarning() << "LinkPropertyController::getComposedModel() NULL link";
		return model;
	}
	
	TreeItem *root = model->getRootItem();
	
	//link properties
	root->insertChildren(root->childCount(), 1, root->columnCount());
	root->child(root->childCount() - 1)->setData(0, tr("Name"));
	root->child(root->childCount() - 1)->setData(1, hi->getName());
	TreeItem* last = root->child(root->childCount() - 1);
	last->setIsElementProperty(true);
	last->setId(HI_NAME);
	last->setDescription(tr("The name of the Handware Interface"));
	last->setPropertyHandler(this);
	
	root->insertChildren(root->childCount(), 1, root->columnCount());
	root->child(root->childCount() - 1)->setData(0, tr("State"));
	root->child(root->childCount() - 1)->setData(1, (hi->getState())? QString("up") : QString("down"));
	last = root->child(root->childCount() - 1);
	last->setIsElementProperty(true);
	last->setId(HI_STATE);
	last->setDescription(tr("The state of the Handware Interface: 'up' or 'down'"));
	last->setPropertyHandler(this);
	
	//Get vm plugins
	QList<PluginProxy*> plugins = PluginRegistry::getInstance()->getHiProxies(hi);
	
	//now, for each plugin, map each property in a TreeItem.
	foreach(PluginProxy *proxy, plugins)
	{
		QVector<QVariant> pluginHeader;
		pluginHeader << "Plugin" << proxy->getPlugin()->getName();
		
		root->insertChildren(root->childCount(), 1, root->columnCount());
		for (int column = 0; column < pluginHeader.size(); ++column)
			root->child(root->childCount() - 1)->setData(column, pluginHeader.at(column));
		
		TreeItem* parent = root->child(root->childCount() - 1);
		
		parent->setProperty(true);	//Plugin node is a property root container; it could have actions.
		parent->setIsPluginHeader(true);
		
		parent->setPropertyHandler(this);
		
		parent->setId(proxy->getPlugin()->getName() + SEPARATOR);
		
		parent->appendChildsDescription(proxy->getPropertyExpert()->getRootChilds());
				
		clonePropertyTree(proxy, proxy->getPlugin()->getPluginProperties(), parent, root);
	}
	
	return model;
}

/**
 * [PRIVATE]
 * Create a clone of preperties but using tree items
 */
void LinkPropertyController::clonePropertyTree(PluginProxy *proxy, QList<PluginProperty*> properties,
		TreeItem* parent, TreeItem *root, bool init)
{	
	foreach(PluginProperty* prop, properties)
	{
		QVector<QVariant> propertyHeader;
		propertyHeader << prop->getName() << ((init)? prop->getDefaultValue() : prop->getValue());
		
		parent->insertChildren(parent->childCount(), 1, root->columnCount());
		
		for(int column = 0; column < propertyHeader.size(); ++column)
			parent->child(parent->childCount() - 1)->setData(column, propertyHeader.at(column));
		
		TreeItem *last = parent->child(parent->childCount() - 1);
		last->setProperty(true);
		
		//PLUGIN_NAME{SEPARATOR}PROPERTY_UID
		last->setId(proxy->getPlugin()->getName() + SEPARATOR +	prop->getUniqueId());
		
		last->appendChildsDescription(proxy->getPropertyExpert()->getChildsByParentId(prop->getId()));
		
		last->setPropertyHandler(this);
		
		last->setDescription(prop->getDescription());
		
		//recursive call
		clonePropertyTree(proxy, prop->getChilds(), last, root, init);
	}
}

/**
 * [IMPL]
 * Search the plugin inside the vm and foreward the action
 */
QString LinkPropertyController::removePluginProperty(QString pluginName, QString propertyUniqueId)
{
	if(!hi)
		return QString("ERROR: unknow base element");
	
	QListIterator<PluginProxy*> i(PluginRegistry::getInstance()->getHiProxies(hi));
		
	/* Seach the plugin to send the changes */
	while(i.hasNext())
	{
		PluginProxy *p = i.next();
		if(p->getPlugin()->getName() == pluginName)
			return p->deleteProperty(propertyUniqueId);
	}
	
	qWarning() << "Plugin" << pluginName << "not found on link" << hi->getName();
	
	return QString("Plugin " + pluginName + " not found on link " + hi->getName());
}

/**
 * [IMPL]
 * Add a pugin property
 */
QPair<PluginProperty*, QString> LinkPropertyController::addPluginProperty(QString pluginName, QString propertyIdToAdd,
				QString parentPropertyUniqueId)
{
	if(!hi)
		return QPair<PluginProperty*, QString>(NULL, QString("ERROR: PropertyHandler return a critical error"));
	
	QListIterator<PluginProxy*> i(PluginRegistry::getInstance()->getHiProxies(hi));
		
	/* Seach the plugin to send the changes */
	while(i.hasNext())
	{
		PluginProxy *p = i.next();
		if(p->getPlugin()->getName() == pluginName)
			return p->addProperty(propertyIdToAdd, parentPropertyUniqueId);
	}
	
	qWarning() << "Plugin" << pluginName << "not found on link" << hi->getName();
	
	return QPair<PluginProperty*, QString>(NULL, QString("ERROR: PropertyHandler return a critical error (Plugin not found)"));
}

/**
 * [MPL]
 * Get the plugin proxy of the current LINK
 */
PluginProxy* LinkPropertyController::getPluginFromCurrentElement(QString pluginName)
{
	QListIterator<PluginProxy*> i(PluginRegistry::getInstance()->getHiProxies(hi));
			
	/* Seach the plugin proxy */
	while(i.hasNext())
	{
		PluginProxy *p = i.next();
		if(p->getPlugin()->getName() == pluginName)
			return p;
	}
	
	qWarning() << "Plugin" << pluginName << "not found on link" << hi->getName();
	
	return NULL;
	
}

///**
// * Render lab properties inside property dock
// */
//void LinkPropertyController::renderLinkProperties(QTableWidget *tableWidget)
//{
//	if(hi == NULL)
//		return;
//	
//	/* render infos inside the property editor */
//	tableWidget->setRowCount(2);
//
//	QTableWidgetItem *property = new QTableWidgetItem();
//	
//	//Hi name
//	property->setData(Qt::DisplayRole, tr("Name"));
//	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
//	tableWidget->setItem(0, 0, property);
//	
//	property = new QTableWidgetItem();
//	property->setData(Qt::DisplayRole, hi->getName());
//	property->setData(Qt::UserRole, HI_NAME);
//	tableWidget->setItem(0, 1, property);
//	
//	property = new QTableWidgetItem();
//	property->setData(Qt::DisplayRole, tr("Status"));
//	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
//	tableWidget->setItem(1, 0, property);
//	
//	property = new QTableWidgetItem();
//	property->setData(Qt::DisplayRole,
//			(hi->getState())? QString("up") : QString("down")	);
//	property->setData(Qt::UserRole, HI_STATE);
//	tableWidget->setItem(1, 1, property);
//	
//	QListIterator<PluginProxy*> i(PluginRegistry::getInstance()->getHiProxies(hi));
//		
//	/* show plugins properties */
//	while(i.hasNext())
//	{
//		PluginProxy *p = i.next();
//		QListIterator<PluginProperty*> j(p->getPluginProperties());
//		
//		/* Build header for this plugin (span 2 columns) */
//		if(j.hasNext())
//		{
//			tableWidget->setRowCount(tableWidget->rowCount() + 1);
//			property = new QTableWidgetItem();
//			property->setFlags(property->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsSelectable);
//			property->setData(Qt::DisplayRole, tr("Plugin: ") + p->getPlugin()->getName());
//			property->setBackgroundColor(Qt::gray);
//			property->setForeground(Qt::blue);
//			property->setFont(QFont("Sand Serif", 9, QFont::Bold));
//			tableWidget->setItem(tableWidget->rowCount() - 1, 0, property);
//			
//			property = new QTableWidgetItem();
//			property->setFlags(property->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsSelectable);
//			property->setBackgroundColor(Qt::gray);
//			tableWidget->setItem(tableWidget->rowCount() - 1, 1, property);
//		}
//		
//		/* render propertyes for this plugin */
//		while(j.hasNext())
//		{
//			PluginProperty *pp = j.next();
//			
//			//property name
//			tableWidget->setRowCount(tableWidget->rowCount() + 1);
//			property = new QTableWidgetItem();
//			property->setData(Qt::DisplayRole, pp->getName());
//			property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
//			tableWidget->setItem(tableWidget->rowCount() - 1, 0, property);
//			
//			//property value
//			property = new QTableWidgetItem();
//			property->setData(Qt::DisplayRole, pp->getValue());
//			property->setData(Qt::ToolTipRole, pp->getDescription());
//			property->setData(Qt::UserRole, p->getPlugin()->getName() + SEPARATOR + pp->getName());
//			tableWidget->setItem(tableWidget->rowCount() - 1, 1, property);
//		}
//	}
//}
//
///**
// * Save a changed property
// */
//bool LinkPropertyController::saveChangedProperty(QTableWidgetItem *item)
//{
//
//	bool ok = true;
//	
//	if(hi == NULL)
//		return false;
//	
//	/* This field is mine? */
//	if(item->data(Qt::UserRole).toString() == HI_NAME)
//	{
//		QString itemValue = item->data(Qt::DisplayRole).toString();
//		
//		/* Some checks */
//		QRegExp validateEthName("^eth([0-9]|[1-9][0-9]+)");
//		if(ok && !validateEthName.exactMatch(itemValue))
//		{
//			QMessageBox::warning(NULL, tr("Visual Netkit - Warning"),
//               tr("The Hardware interface name must be validated by\n^eth([0-9]|[1-9][0-9]+)\nExamples: eth0 eth1 eth2 ... eth12 ..."),
//               QMessageBox::Ok);
//			
//			//Restore the value, and alert the user
//			item->setData(Qt::DisplayRole, hi->getName());
//			ok = false;
//		}
//		
//		if(ok && LinkHandler::getInstance()->hiNameExist(hi, itemValue) && itemValue != hi->getName())
//		{
//			QMessageBox::warning(NULL, tr("Visual Netkit - Warning"),
//               tr("The Hardware interface name must be unique!"),
//               QMessageBox::Ok);
//			
//			//Restore the value, and alert the user
//			item->setData(Qt::DisplayRole, hi->getName());
//			ok = false;
//		}
//		
//		/* save changes */
//		if(ok)
//		{
//			LinkHandler::getInstance()->updateLinkName(hi, itemValue);
//			LinkMapper::getInstance()->getLink(hi)->setLinkLabel(itemValue);
//		}
//		
//	}
//	else if(item->data(Qt::UserRole).toString() == HI_STATE) //State field?
//	{
//		QString itemValue = item->data(Qt::DisplayRole).toString().toLower();
//		
//		/* Validate user input */
//		QRegExp validateState("up|down");
//		if(!validateState.exactMatch(itemValue))
//		{
//			QMessageBox::warning(NULL, tr("Visual Netkit - Warning"),
//               tr("The Hardware interface state must be \"up\" or \"down\"\n(case insensitive)"),
//               QMessageBox::Ok);
//			
//			//Restore the value, and alert the user
//			item->setData(Qt::DisplayRole, (hi->getState())? QString("up") : QString("down")	);
//			ok = false;
//		}
//		
//		/* save changes */
//		if(ok)
//		{
//			hi->setState((itemValue == "up"));
//			LinkMapper::getInstance()->getLink(hi)->updateLinkState();
//		}
//	}
//	else
//	{
//		
//		QRegExp pluginPropValidator("(.+)" + SEPARATOR + "(.+)");
//		if(pluginPropValidator.exactMatch(item->data(Qt::UserRole).toString()) ||
//				item->data(Qt::UserRole).toString().split(SEPARATOR).size() != 2)
//		{	
//			QString pluginName = item->data(Qt::UserRole).toString().split(SEPARATOR)[0];
//			QString propName = item->data(Qt::UserRole).toString().split(SEPARATOR)[1];
//			
//			QListIterator<PluginProxy*> i(PluginRegistry::getInstance()->getHiProxies(hi));
//			
//			/* Seach the plugin to send the change */
//			while(i.hasNext())
//			{
//				PluginProxy *p = i.next();
//				if(p->getPlugin()->getName() == pluginName)
//				{
//					QString *alert = new QString();
//					if(!p->saveProperty(propName, item->data(Qt::DisplayRole).toString(), alert))
//					{
//						int userChoice;
//						
//						userChoice = QMessageBox::question(NULL, tr("Visual Netkit - Warning"),
//							tr(("Plugin Property error: " + *alert + "\n\n" + "Do you want ignore this warning?").toUtf8()),
//							QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
//						
//						ok = false;
//						
//						if(userChoice == QMessageBox::Yes)
//						{
//							//ok, store the value and shut up!
//							ok = p->saveProperty(propName, item->data(Qt::DisplayRole).toString());
//						}	
//					} else {
//						ok = true;
//					}
//					delete alert;
//					break;
//				}
//			}
//			
//		}
//		else
//			qWarning() << "Unknown property user role:" << item->data(Qt::UserRole).toString();
//	}
//	
//	if (ok)
//		LabHandler::getInstance()->getMainWindow()->writeLogMessage(tr("Link property saved"));
//	return ok;
//}
