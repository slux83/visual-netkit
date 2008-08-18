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

#include "VmPropertyController.h"
#include "VmHandler.h"
#include "VmMapper.h"
#include "../../core/handles/LabFacadeController.h"
#include "../../plugin_framework/PluginRegistry.h"
#include "../../common/CommonConfigs.h"

#include <QMessageBox>

#define VM_NAME "VmName"


/**
 * Constructor
 */
VmPropertyController::VmPropertyController() : AbstractPropertyHandler()
{
	vm = NULL;
}

/**
 * Deconstructor
 */
VmPropertyController::~VmPropertyController()
{
}

/**
 * Render lab properties inside property dock
 */
//void VmPropertyController::renderVmProperties(QTableWidget *tableWidget)
//{
//	if(vm == NULL)
//		return;
//	
//	/* render infos inside the property editor */
//	tableWidget->setRowCount(1);
//
//	QTableWidgetItem *property = new QTableWidgetItem();
//	
//	//Vm name
//	property->setData(Qt::DisplayRole, tr("Name"));
//	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
//	tableWidget->setItem(0, 0, property);
//	
//	property = new QTableWidgetItem();
//	property->setData(Qt::DisplayRole, vm->getName());
//	property->setData(Qt::UserRole, VM_NAME);
//	tableWidget->setItem(0, 1, property);
//	
//	QListIterator<PluginProxy*> i(PluginRegistry::getInstance()->getVmProxies(vm));
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

/**
 * Save a changed property
 */
bool VmPropertyController::saveChangedProperty(TreeItem *treeItem)
{
	bool ok = true;
	
	if(vm == NULL)
	{
		qWarning() << "VmPropertyController::saveChangedProperty()" << "NULL VirtualMachine";
		return false;
	}
	
	QString itemValue = treeItem->data(1).toString();
	//	/* This field is mine? */
	//	if(item->data(Qt::UserRole).toString() == VM_NAME)
	//	{
	//		QRegExpValidator nameValidator(QRegExp("^[a-zA-Z0-9]+$"), this);
	//		int pos = 0;
	//		
	//		/* validate name */
	//		if(ok && nameValidator.validate(itemValue, pos) != QValidator::Acceptable)
	//		{
	//			QMessageBox::warning(NULL, tr("Visual Netkit - Warning"),
	//	                   tr("The Virtual machine name must match ^[a-zA-Z0-9]+$"),
	//	                   QMessageBox::Ok);
	//			
	//			//Restore the value, and alert the user
	//			item->setData(Qt::DisplayRole, vm->getName());
	//			ok = false;
	//	        
	//		}
	//		
	//		if(ok && VmHandler::getInstance()->vmNameExist(itemValue) && itemValue != vm->getName())
	//		{
	//			QMessageBox::warning(NULL, tr("Visual Netkit - Warning"),
	//	                   tr("The Virtual machine name must be unique!"),
	//	                   QMessageBox::Ok);
	//			
	//			//Restore the value, and alert the user
	//			item->setData(Qt::DisplayRole, vm->getName());
	//			ok = false;
	//		}
	//		
	//		/* save changes */
	//		if(ok)
	//		{
	//			//change tree lab names (vm dir and .startup file name)
	//			LabHandler::getInstance()->getMainWindow()->changeTreeNodeName(vm->getName(), itemValue);
	//			LabHandler::getInstance()->getMainWindow()->changeTreeNodeName(vm->getName() + ".startup", itemValue + ".startup");
	//			
	//			//save on low level and scene level
	//			LabFacadeController::getInstance()->getCurrentLab()->updateVmKey(vm->getName(), itemValue, vm);
	//			VmMapper::getInstance()->getVmItem(vm)->setLabelVmName(itemValue);
	//		}
	//		
	//	}
	//	else
	//	{
	
	QStringList itemDataSplitted = treeItem->getId().split(SEPARATOR);
	if(itemDataSplitted.size() != 3)
	{
		qWarning() << "tree item id not standard:" << treeItem->getId();
		return false;
	}
	
	QString pluginName = itemDataSplitted[0];
	QString propName = itemDataSplitted[1];
	QString propCopy = itemDataSplitted[2];
	
	QListIterator<PluginProxy*> i(PluginRegistry::getInstance()->getVmProxies(vm));
	
	/* Seach the plugin to send the change */
	while(i.hasNext())
	{
		PluginProxy *p = i.next();
		if(p->getPlugin()->getName() == pluginName)
		{
			QString alert;
			if(!p->saveProperty(propName, itemValue, &alert))
			{
				int userChoice;
				
				userChoice = QMessageBox::question(NULL, tr("Visual Netkit - Warning"),
					tr(("Plugin Property error: " + alert + "\n\n" + "Do you want ignore this warning?").toUtf8()),
					QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
				
				ok = false;
				
				if(userChoice == QMessageBox::Yes)
				{
					//ok, store the value and shut up!
					ok = p->saveProperty(propName, itemValue);
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
TreeModel* VmPropertyController::getInitModel(QList<PluginProxy*> plugins)
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
		
		parent->setId(proxy->getPlugin()->getName() + SEPARATOR + SEPARATOR);
		
		parent->appendChildsDescription(proxy->getPropertyExpert()->getRootChilds());
		
		clonePropertyTree(proxy, proxy->getPlugin()->getPluginProperties(), parent, root);
	}
	
	return model;
}

/**
 * [PRIVATE]
 * Create a clone of preperties but using tree items
 */
void VmPropertyController::clonePropertyTree(PluginProxy *proxy, QList<PluginProperty*> properties,
		TreeItem* parent, TreeItem *root)
{	
	foreach(PluginProperty* prop, properties)
	{
		QVector<QVariant> propertyHeader;
		propertyHeader << prop->getName() << prop->getDefaultValue();
		
		parent->insertChildren(parent->childCount(), 1, root->columnCount());
		
		for(int column = 0; column < propertyHeader.size(); ++column)
			parent->child(parent->childCount() - 1)->setData(column, propertyHeader.at(column));
		
		TreeItem *last = parent->child(parent->childCount() - 1);
		last->setProperty(true);
		
		//PLUGIN_NAME{SEPARATOR}PROPERTY_ID{SEPARATOR}PROPERTY_COPY
		last->setId(proxy->getPlugin()->getName() + SEPARATOR +
				prop->getId() + SEPARATOR +
				QString::number(prop->getCopy()));
		
		last->appendChildsDescription(proxy->getPropertyExpert()->getChildsByParentId(prop->getId()));
		
		last->setPropertyHandler(this);
		
		//recursive call
		clonePropertyTree(proxy, prop->getChilds(), last, root);
	}
}

/**
 * [IMPL]
 * Search the plugin inside the vm and foreward the action
 */
QString VmPropertyController::removePluginProperty(
		QString pluginName, QString propertyId, quint16 propertyCopy)
{
	if(!vm)
		return QString("ERROR: unknow base element");
	
	QListIterator<PluginProxy*> i(PluginRegistry::getInstance()->getVmProxies(vm));
		
	/* Seach the plugin to send the changes */
	while(i.hasNext())
	{
		PluginProxy *p = i.next();
		if(p->getPlugin()->getName() == pluginName)
			return p->deleteProperty(propertyId, propertyCopy);
	}
	
	qWarning() << "Plugin" << pluginName << "not found on vm" << vm->getName();
	
	return QString("Plugin " + pluginName + " not found on virtual machine " + vm->getName());

}

/**
 * [IMPL]
 * Add a pugin property
 */
QPair<PluginProperty*, QString> VmPropertyController::addPluginProperty(QString pluginName, QString propertyIdToAdd,
				QString parentPropertyId, quint16 parentPropertyCopy)
{
	if(!vm)
		return QPair<PluginProperty*, QString>(NULL, QString("ERROR: PropertyHandler return a critical error"));
	
	QListIterator<PluginProxy*> i(PluginRegistry::getInstance()->getVmProxies(vm));
		
	/* Seach the plugin to send the changes */
	while(i.hasNext())
	{
		PluginProxy *p = i.next();
		if(p->getPlugin()->getName() == pluginName)
			return p->addProperty(propertyIdToAdd, parentPropertyId, parentPropertyCopy);
	}
	
	qWarning() << "Plugin" << pluginName << "not found on vm" << vm->getName();
	
	return QPair<PluginProperty*, QString>(NULL, QString("ERROR: PropertyHandler return a critical error (Plugin not found)"));
}

/**
 * [MPL]
 * Get the plugin proxy of the current VM
 */
PluginProxy* VmPropertyController::getPluginFromCurrentElement(QString pluginName)
{
	QListIterator<PluginProxy*> i(PluginRegistry::getInstance()->getVmProxies(vm));
			
	/* Seach the plugin proxy */
	while(i.hasNext())
	{
		PluginProxy *p = i.next();
		if(p->getPlugin()->getName() == pluginName)
			return p;
	}
	
	qWarning() << "Plugin" << pluginName << "not found on vm" << vm->getName();
	
	return NULL;
	
}
