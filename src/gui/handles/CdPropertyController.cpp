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
CdPropertyController::CdPropertyController() : QObject()
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
 * Render lab properties inside property dock
 */
void CdPropertyController::renderCdProperties(QTableWidget *tableWidget)
{
	if(cd == NULL)
		return;
	
	/* render infos inside the property editor */
	tableWidget->setRowCount(1);

	QTableWidgetItem *property = new QTableWidgetItem();
	
	//Cd name
	property->setData(Qt::DisplayRole, tr("Name"));
	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
	tableWidget->setItem(0, 0, property);
	
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, cd->getName());
	property->setData(Qt::UserRole, CD_NAME);
	tableWidget->setItem(0, 1, property);
	
	QListIterator<PluginProxy*> i(PluginRegistry::getInstance()->getCdProxies(cd));
	
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
bool CdPropertyController::saveChangedProperty(QTableWidgetItem *item)
{
	bool ok = true;
	
	if(cd == NULL)
		return false;
	
	QString itemValue = item->data(Qt::DisplayRole).toString().trimmed();
	
	/* This field is mine? */
	if(item->data(Qt::UserRole).toString() == CD_NAME)
	{
		
		/* Some checks */
		if(ok && itemValue == "")
		{
			QMessageBox::warning(NULL, tr("Visual Netkit - Warning"),
	                   tr("The collision domain name must be not empty!"),
	                   QMessageBox::Ok);
			
			//Restore the value, and alert the user
			item->setData(Qt::DisplayRole, cd->getName());
			ok = false;
	        
		}
		
		if(ok && CdHandler::getInstance()->cdNameExist(itemValue) && itemValue != cd->getName())
		{
			
			QMessageBox::warning(NULL, tr("Visual Netkit - Warning"),
	                   tr("The collision domain name must be unique!"),
	                   QMessageBox::Ok);
			
			//Restore the value, and alert the user
			item->setData(Qt::DisplayRole, cd->getName());
			ok = false;
		}
		
		/* save changes */
		if(ok)
		{
			LabFacadeController::getInstance()->getCurrentLab()->updateCdKey(cd->getName(), itemValue, cd);
			CdMapper::getInstance()->getCdItem(cd)->setLabelCdName(itemValue);
		}
		
	}
	else
	{
		//TODO: send this field to plugins's proxy		
	}
	
	if (ok)
		LabHandler::getInstance()->getMainWindow()->writeLogMessage(tr("Collision domain property saved"));
	return ok;
}

