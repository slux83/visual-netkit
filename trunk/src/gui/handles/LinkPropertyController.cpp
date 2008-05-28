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
#define HI_NAME "VmName"
#define HI_STATE "HiState"

/**
 * Constructor
 */
LinkPropertyController::LinkPropertyController()
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
 * Render lab properties inside property dock
 */
void LinkPropertyController::renderLinkProperties(QTableWidget *tableWidget)
{
	if(hi == NULL)
		return;
	
	/* render infos inside the property editor */
	tableWidget->setRowCount(2);

	QTableWidgetItem *property = new QTableWidgetItem();
	
	//Hi name
	property->setData(Qt::DisplayRole, tr("Name"));
	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
	tableWidget->setItem(0, 0, property);
	
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, hi->getName());
	property->setData(Qt::UserRole, HI_NAME);
	tableWidget->setItem(0, 1, property);
	
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, tr("Status"));
	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
	tableWidget->setItem(1, 0, property);
	
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole,
			(hi->getState())? QString("up") : QString("down")	);
	property->setData(Qt::UserRole, HI_STATE);
	tableWidget->setItem(1, 1, property);
	
	QListIterator<PluginProxy*> i(PluginRegistry::getInstance()->getHiProxies(hi));
		
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
bool LinkPropertyController::saveChangedProperty(QTableWidgetItem *item)
{

	bool ok = true;
	
	if(hi == NULL)
		return false;
	
	/* This field is mine? */
	if(item->data(Qt::UserRole).toString() == HI_NAME)
	{
		QString itemValue = item->data(Qt::DisplayRole).toString();
		
		/* Some checks */
		QRegExp validateEthName("^eth([0-9]|[1-9][0-9]+)");
		if(ok && !validateEthName.exactMatch(itemValue))
		{
			QMessageBox::warning(NULL, tr("Visual Netkit - Warning"),
               tr("The Hardware interface name must be validated by\n^eth([0-9]|[1-9][0-9]+)\nExamples: eth0 eth1 eth2 ... eth12 ..."),
               QMessageBox::Ok);
			
			//Restore the value, and alert the user
			item->setData(Qt::DisplayRole, hi->getName());
			ok = false;
		}
		
		if(ok && LinkHandler::getInstance()->hiNameExist(hi, itemValue) && itemValue != hi->getName())
		{
			QMessageBox::warning(NULL, tr("Visual Netkit - Warning"),
               tr("The Hardware interface name must be unique!"),
               QMessageBox::Ok);
			
			//Restore the value, and alert the user
			item->setData(Qt::DisplayRole, hi->getName());
			ok = false;
		}
		
		/* save changes */
		if(ok)
		{
			LinkHandler::getInstance()->updateLinkName(hi, itemValue);
			LinkMapper::getInstance()->getLink(hi)->setLinkLabel(itemValue);
		}
		
	}
	else if(item->data(Qt::UserRole).toString() == HI_STATE) //State field?
	{
		QString itemValue = item->data(Qt::DisplayRole).toString().toLower();
		
		/* Validate user input */
		QRegExp validateState("up|down");
		if(!validateState.exactMatch(itemValue))
		{
			QMessageBox::warning(NULL, tr("Visual Netkit - Warning"),
               tr("The Hardware interface state must be \"up\" or \"down\"\n(case insensitive)"),
               QMessageBox::Ok);
			
			//Restore the value, and alert the user
			item->setData(Qt::DisplayRole, (hi->getState())? QString("up") : QString("down")	);
			ok = false;
		}
		
		/* save changes */
		if(ok)
		{
			hi->setState((itemValue == "up"));
			LinkMapper::getInstance()->getLink(hi)->updateLinkState();
		}
	}
	else
	{
		
		QRegExp pluginPropValidator("(.+)" + SEPARATOR + "(.+)");
		if(pluginPropValidator.exactMatch(item->data(Qt::UserRole).toString()) ||
				item->data(Qt::UserRole).toString().split(SEPARATOR).size() != 2)
		{	
			QString pluginName = item->data(Qt::UserRole).toString().split(SEPARATOR)[0];
			QString propName = item->data(Qt::UserRole).toString().split(SEPARATOR)[1];
			
			QListIterator<PluginProxy*> i(PluginRegistry::getInstance()->getHiProxies(hi));
			
			/* Seach the plugin to send the change */
			while(i.hasNext())
			{
				PluginProxy *p = i.next();
				if(p->getPlugin()->getName() == pluginName)
				{
					QString *alert = new QString();
					if(!p->initProperty(propName, item->data(Qt::DisplayRole).toString(), alert))
					{
						int userChoice;
						
						userChoice = QMessageBox::question(NULL, tr("Visual Netkit - Warning"),
							tr(("Plugin Property error: " + *alert + "\n\n" + "Do you want ignore this warning?").toUtf8()),
							QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
						
						if(userChoice == QMessageBox::Yes)
						{
							//ok, store the value and shut up!
							ok = p->initProperty(propName, item->data(Qt::DisplayRole).toString());
						}	
					} else {
						ok = true;
					}
					delete alert;
					break;
				}
			}
			
		}
		else
			qWarning() << "Unknown property user role:" << item->data(Qt::UserRole).toString();
	}
	
	if (ok)
		LabHandler::getInstance()->getMainWindow()->writeLogMessage(tr("Link property saved"));
	return ok;
}
