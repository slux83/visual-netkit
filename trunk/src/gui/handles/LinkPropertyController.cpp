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
	property->setData(Qt::DisplayRole, tr("Is Up"));
	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
	tableWidget->setItem(1, 0, property);
	
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole,
			(hi->getState())? QString("up") : QString("down")	);
	property->setData(Qt::UserRole, HI_STATE);
	tableWidget->setItem(1, 1, property);
	
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
		//TODO
	}
	else
	{
		//TODO: send this field to plugins's proxy		
	}
	
	return ok;
}
