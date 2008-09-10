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

#include "CdPropertyController.h"
#include "CdHandler.h"
#include "CdMapper.h"
#include "../../core/handles/LabFacadeController.h"
#include "../../plugin_framework/PluginRegistry.h"
#include "../../common/CommonConfigs.h"
#include <QMessageBox>

#define CD_NAME "CdName"

/**
 * Constructor
 */
CdPropertyController::CdPropertyController() : AbstractPropertyHandler()
{
	cd = NULL;
}

/**
 * Deconstructor
 */
CdPropertyController::~CdPropertyController()
{
}


/**
 * Save a changed property
 */
bool CdPropertyController::saveChangedProperty(TreeItem *treeItem)
{
	bool ok = true;
	
	if(cd == NULL)
	{
		qWarning() << "CdPropertyController::saveChangedProperty()" << "NULL collision domain";
		return false;
	}
	
	QString itemValue = treeItem->data(1).toString();
	
	/* This field is mine? */
	if(treeItem->getId() == CD_NAME)
	{
		QString itemValue = treeItem->data(1).toString();
		QRegExpValidator nameValidator(QRegExp("^[a-zA-Z0-9]+$"), this);
		int pos = 0;
		
		/* validate name */
		if(ok && nameValidator.validate(itemValue, pos) != QValidator::Acceptable)
		{
			QMessageBox::warning(NULL, tr("Visual Netkit - Warning"),
	                   tr("The Collision domain name must match ^[a-zA-Z0-9]+$"),
	                   QMessageBox::Ok);
			
			return false;
	        
		}
		
		if(CdHandler::getInstance()->cdNameExist(itemValue) && itemValue != cd->getName())
		{
			
			QMessageBox::warning(NULL, tr("Visual Netkit - Warning"),
	                   tr("The collision domain name must be unique!"),
	                   QMessageBox::Ok);
			
			return false;
		}
		
		/* save changes */
		LabFacadeController::getInstance()->getCurrentLab()->updateCdKey(cd->getName(), itemValue, cd);
		CdMapper::getInstance()->getCdItem(cd)->setLabelCdName(itemValue);

		LabHandler::getInstance()->getMainWindow()->writeLogMessage(tr("Collision Domain property saved"));

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
	
	QListIterator<PluginProxy*> i(PluginRegistry::getInstance()->getCdProxies(cd));
	
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

	if(ok)
		LabHandler::getInstance()->getMainWindow()->writeLogMessage(tr("Collision Domain property saved"));

	return ok;
}

/**
 * [IMPL]
 * Build and return the TreeModel to change plugins properties
 */
TreeModel* CdPropertyController::getInitModel(QList<PluginProxy*> plugins)
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
TreeModel* CdPropertyController::getComposedModel()
{
	QStringList header;
	header << tr("Property") << tr("Value");
	TreeModel *model = new TreeModel(header);
	
	if(!cd)
	{
		qWarning() << "CdPropertyController::getComposedModel() NULL cd";
		return model;
	}
	
	TreeItem *root = model->getRootItem();
	
	//vm properties
	root->insertChildren(root->childCount(), 1, root->columnCount());
	root->child(root->childCount() - 1)->setData(0, tr("Name"));
	root->child(root->childCount() - 1)->setData(1, cd->getName());
	TreeItem* last = root->child(root->childCount() - 1);
	last->setIsElementProperty(true);
	last->setId(CD_NAME);
	last->setDescription(tr("The name of the Collision Domain"));
	last->setPropertyHandler(this);
	
	//Get vm plugins
	QList<PluginProxy*> plugins = PluginRegistry::getInstance()->getCdProxies(cd);
	
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
void CdPropertyController::clonePropertyTree(PluginProxy *proxy, QList<PluginProperty*> properties,
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
QString CdPropertyController::removePluginProperty(QString pluginName, QString propertyUniqueId)
{
	if(!cd)
		return QString("ERROR: unknow base element");
	
	QListIterator<PluginProxy*> i(PluginRegistry::getInstance()->getCdProxies(cd));
		
	/* Seach the plugin to send the changes */
	while(i.hasNext())
	{
		PluginProxy *p = i.next();
		if(p->getPlugin()->getName() == pluginName)
			return p->deleteProperty(propertyUniqueId);
	}
	
	qWarning() << "Plugin" << pluginName << "not found on cd" << cd->getName();
	
	return QString("Plugin " + pluginName + " not found on collision domain " + cd->getName());

}

/**
 * [IMPL]
 * Add a pugin property
 */
QPair<PluginProperty*, QString> CdPropertyController::addPluginProperty(QString pluginName, QString propertyIdToAdd,
				QString parentPropertyUniqueId)
{
	if(!cd)
		return QPair<PluginProperty*, QString>(NULL, QString("ERROR: PropertyHandler return a critical error"));
	
	QListIterator<PluginProxy*> i(PluginRegistry::getInstance()->getCdProxies(cd));
		
	/* Seach the plugin to send the changes */
	while(i.hasNext())
	{
		PluginProxy *p = i.next();
		if(p->getPlugin()->getName() == pluginName)
			return p->addProperty(propertyIdToAdd, parentPropertyUniqueId);
	}
	
	qWarning() << "Plugin" << pluginName << "not found on cd" << cd->getName();
	
	return QPair<PluginProperty*, QString>(NULL, QString("ERROR: PropertyHandler return a critical error (Plugin not found)"));
}

/**
 * [MPL]
 * Get the plugin proxy of the current VM
 */
PluginProxy* CdPropertyController::getPluginFromCurrentElement(QString pluginName)
{
	QListIterator<PluginProxy*> i(PluginRegistry::getInstance()->getCdProxies(cd));
			
	/* Seach the plugin proxy */
	while(i.hasNext())
	{
		PluginProxy *p = i.next();
		if(p->getPlugin()->getName() == pluginName)
			return p;
	}
	
	qWarning() << "Plugin" << pluginName << "not found on cd" << cd->getName();
	
	return NULL;
	
}


///**
// * Render lab properties inside property dock
// */
//void CdPropertyController::renderCdProperties(QTableWidget *tableWidget)
//{
//	if(cd == NULL)
//		return;
//	
//	/* render infos inside the property editor */
//	tableWidget->setRowCount(1);
//
//	QTableWidgetItem *property = new QTableWidgetItem();
//	
//	//Cd name
//	property->setData(Qt::DisplayRole, tr("Name"));
//	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
//	tableWidget->setItem(0, 0, property);
//	
//	property = new QTableWidgetItem();
//	property->setData(Qt::DisplayRole, cd->getName());
//	property->setData(Qt::UserRole, CD_NAME);
//	tableWidget->setItem(0, 1, property);
//	
//	QListIterator<PluginProxy*> i(PluginRegistry::getInstance()->getCdProxies(cd));
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
//bool CdPropertyController::saveChangedProperty(QTableWidgetItem *item)
//{
//	bool ok = true;
//	
//	if(cd == NULL)
//		return false;
//	
//	QString itemValue = item->data(Qt::DisplayRole).toString().trimmed();
//	
//	/* This field is mine? */
//	if(item->data(Qt::UserRole).toString() == CD_NAME)
//	{
//		
//		QRegExpValidator nameValidator(QRegExp("^[a-zA-Z0-9]+$"), this);
//		int pos = 0;
//		
//		/* validate name */
//		if(ok && nameValidator.validate(itemValue, pos) != QValidator::Acceptable)
//		{
//			QMessageBox::warning(NULL, tr("Visual Netkit - Warning"),
//                   tr("The collision domain name must match ^[a-zA-Z0-9]+$"),
//                   QMessageBox::Ok);
//			
//			//Restore the value, and alert the user
//			item->setData(Qt::DisplayRole, cd->getName());
//			ok = false;
//	        
//		}
//		
//		if(ok && CdHandler::getInstance()->cdNameExist(itemValue) && itemValue != cd->getName())
//		{
//			
//			QMessageBox::warning(NULL, tr("Visual Netkit - Warning"),
//	                   tr("The collision domain name must be unique!"),
//	                   QMessageBox::Ok);
//			
//			//Restore the value, and alert the user
//			item->setData(Qt::DisplayRole, cd->getName());
//			ok = false;
//		}
//		
//		/* save changes */
//		if(ok)
//		{
//			LabFacadeController::getInstance()->getCurrentLab()->updateCdKey(cd->getName(), itemValue, cd);
//			CdMapper::getInstance()->getCdItem(cd)->setLabelCdName(itemValue);
//		}
//		
//	}
//	else
//	{
//		QRegExp pluginPropValidator("(.+)" + SEPARATOR + "(.+)");
//		if(pluginPropValidator.exactMatch(item->data(Qt::UserRole).toString()) ||
//				item->data(Qt::UserRole).toString().split(SEPARATOR).size() != 2)
//		{	
//			QString pluginName = item->data(Qt::UserRole).toString().split(SEPARATOR)[0];
//			QString propName = item->data(Qt::UserRole).toString().split(SEPARATOR)[1];
//			
//			QListIterator<PluginProxy*> i(PluginRegistry::getInstance()->getCdProxies(cd));
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
//		LabHandler::getInstance()->getMainWindow()->writeLogMessage(tr("Collision domain property saved"));
//	return ok;
//}

