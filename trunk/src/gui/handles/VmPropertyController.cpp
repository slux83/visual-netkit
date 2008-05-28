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
VmPropertyController::VmPropertyController() : QObject()
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
void VmPropertyController::renderVmProperties(QTableWidget *tableWidget)
{
	if(vm == NULL)
		return;
	
	/* render infos inside the property editor */
	tableWidget->setRowCount(1);

	QTableWidgetItem *property = new QTableWidgetItem();
	
	//Vm name
	property->setData(Qt::DisplayRole, tr("Name"));
	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
	tableWidget->setItem(0, 0, property);
	
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, vm->getName());
	property->setData(Qt::UserRole, VM_NAME);
	tableWidget->setItem(0, 1, property);
	
	QListIterator<PluginProxy*> i(PluginRegistry::getInstance()->getVmProxies(vm));
	
	/* show plugins properties */
	while(i.hasNext())
	{
		PluginProxy *p = i.next();
		QMapIterator<QString, PluginProperty*> j(p->getPluginProperties());
		
		/* Build header for this plugin (span 2 columns) */
		if(j.hasNext())
		{
			tableWidget->setRowCount(tableWidget->rowCount() + 1);
			property = new QTableWidgetItem();
			property->setFlags(!Qt::ItemIsSelectable || !Qt::ItemIsEditable);
			property->setData(Qt::DisplayRole, tr("Plugin: ") + p->getPlugin()->getName());
			property->setBackgroundColor(Qt::gray);
			property->setForeground(Qt::blue);
			property->setFont(QFont("Sand Serif", 9, QFont::Bold));
			tableWidget->setItem(tableWidget->rowCount() - 1, 0, property);
			tableWidget->setSpan(tableWidget->rowCount() - 1, 0, 1, 2);
		}
		
		/* render propertyes for this plugin */
		while(j.hasNext())
		{
			j.next();
			//property name
			tableWidget->setRowCount(tableWidget->rowCount() + 1);
			property = new QTableWidgetItem();
			property->setData(Qt::DisplayRole, j.key());
			property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
			tableWidget->setItem(tableWidget->rowCount() - 1, 0, property);
			
			//property value
			property = new QTableWidgetItem();
			property->setData(Qt::DisplayRole, j.value()->getValue());
			property->setData(Qt::ToolTipRole, j.value()->getDescription());
			property->setData(Qt::UserRole, p->getPlugin()->getName() + SEPARATOR + j.key());
			tableWidget->setItem(tableWidget->rowCount() - 1, 1, property);
		}
	}
}

/**
 * Save a changed property
 */
bool VmPropertyController::saveChangedProperty(QTableWidgetItem *item)
{

	bool ok = true;
	
	if(vm == NULL)
		return false;
	
	QString itemValue = item->data(Qt::DisplayRole).toString().trimmed();
	
	/* This field is mine? */
	if(item->data(Qt::UserRole).toString() == VM_NAME)
	{
		
		/* Some checks */
		if(ok && itemValue == "")
		{
			QMessageBox::warning(NULL, tr("Visual Netkit - Warning"),
	                   tr("The Virtual machine name must be not empty!"),
	                   QMessageBox::Ok);
			
			//Restore the value, and alert the user
			item->setData(Qt::DisplayRole, vm->getName());
			ok = false;
	        
		}
		
		if(ok && VmHandler::getInstance()->vmNameExist(itemValue) && itemValue != vm->getName())
		{
			QMessageBox::warning(NULL, tr("Visual Netkit - Warning"),
	                   tr("The Virtual machine name must be unique!"),
	                   QMessageBox::Ok);
			
			//Restore the value, and alert the user
			item->setData(Qt::DisplayRole, vm->getName());
			ok = false;
		}
		
		/* save changes */
		if(ok)
		{
			//change tree lab names (vm dir and .startup file name)
			LabHandler::getInstance()->getMainWindow()->changeTreeNodeName(vm->getName(), itemValue);
			LabHandler::getInstance()->getMainWindow()->changeTreeNodeName(vm->getName() + ".startup", itemValue + ".startup");
			
			//save on low level and scene level
			LabFacadeController::getInstance()->getCurrentLab()->updateVmKey(vm->getName(), itemValue, vm);
			VmMapper::getInstance()->getVmItem(vm)->setLabelVmName(itemValue);
		}
		
	}
	else
	{
		//TODO: send this field to plugins's proxy		
	}
	
	if (ok)
		LabHandler::getInstance()->getMainWindow()->writeLogMessage(tr("Virtual machine property saved"));
	return ok;
}

