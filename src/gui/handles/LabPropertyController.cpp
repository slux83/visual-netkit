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

#include "LabPropertyController.h"
#include "LabHandler.h"

/**
 * Constructor
 */
LabPropertyController::LabPropertyController() : QObject()
{
	lab = NULL;
}

/**
 * Constructor
 */
LabPropertyController::~LabPropertyController()
{
}

/**
 * Save the property passed, if it's mine
 * Return true if the property is saved correctly
 */
bool LabPropertyController::saveChangedProperty(QTableWidgetItem * item)
{
	bool returnVal = false;
	
	/* Is this property mapped with a Laboratory? */
	if(lab == NULL)
		return false;
		
	/* Select the correct property to save inside the domain current lab */
	switch (item->data(Qt::UserRole).toInt())
	{
		case Name:
			if(item->data(Qt::DisplayRole).toString().trimmed().isEmpty())
			{
				//Restore the value, and alert the user
				item->setData(Qt::DisplayRole, lab->getName());
				QMessageBox::warning(NULL, tr("Visual Netkit - Warning"),
		                   tr("The laboratory name must be not empty!"),
		                   QMessageBox::Ok);
			}
			else
			{				
				//Update the tree lab view
				LabHandler::getInstance()->getMainWindow()->changeTreeNodeName(lab->getName(),
						item->data(Qt::DisplayRole).toString(), true);
				
				lab->setName(item->data(Qt::DisplayRole).toString());
				
				returnVal = true;
			}
			
			break;
		
		case
			Version: lab->setVersion(item->data(Qt::DisplayRole).toString());
			returnVal = true;
			break;
		
		case 
			Date: lab->setDate(item->data(Qt::DisplayRole).toString());
			returnVal = true;
			break;

		case Description: lab->setDescription(item->data(Qt::DisplayRole).toString());
			break;
		
		case 
			Authors: lab->setAuthors(item->data(Qt::DisplayRole).toString());
			returnVal = true;
			break;
		
		case 
			Email: lab->setEmail(item->data(Qt::DisplayRole).toString());
			returnVal = true;
			break;
		
		case 
			Website: lab->setWebsite(item->data(Qt::DisplayRole).toString());
			returnVal = true;
			break;
	}

	LabHandler::getInstance()->getMainWindow()->writeLogMessage(tr("Lab property saved"));
	return returnVal;
}

/**
 * Render lab properties inside property dock
 */
void LabPropertyController::renderLabProperties(QTableWidget *tableWidget)
{
	/* render infos inside the property editor */
	tableWidget->setRowCount(7);

	QTableWidgetItem *property = new QTableWidgetItem();
	
	//Lab name
	property->setData(Qt::DisplayRole, tr("Name"));
	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
	tableWidget->setItem(0, 0, property);
	tableWidget->setSpan(0, 0, 1, 1);
	
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, lab->getName());
	property->setData(Qt::UserRole, Name);
	tableWidget->setItem(0, 1, property);
	tableWidget->setSpan(0, 1, 1, 1);
	
	//Lab version
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, tr("Version"));
	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
	tableWidget->setItem(1, 0, property);
	tableWidget->setSpan(1, 0, 1, 1);
	
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, lab->getVersion());
	property->setData(Qt::UserRole, Version);
	tableWidget->setItem(1, 1, property);
	tableWidget->setSpan(1, 1, 1, 1);
		
	//Lab date
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, tr("Date"));
	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
	tableWidget->setItem(2, 0, property);
	tableWidget->setSpan(2, 0, 1, 1);
	
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, lab->getDate());
	property->setData(Qt::UserRole, Date);
	tableWidget->setItem(2, 1, property);
	tableWidget->setSpan(2, 1, 1, 1);
	
	//Lab description
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, tr("Description"));
	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
	tableWidget->setItem(3, 0, property);
	tableWidget->setSpan(3, 0, 1, 1);
	
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, lab->getDescription());
	property->setData(Qt::UserRole, Description);
	tableWidget->setItem(3, 1, property);
	tableWidget->setSpan(3, 1, 1, 1);
	
	//Lab authors
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, tr("Authors"));
	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
	tableWidget->setItem(4, 0, property);
	tableWidget->setSpan(4, 0, 1, 1);
	
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, lab->getAuthors());
	property->setData(Qt::UserRole, Authors);
	tableWidget->setItem(4, 1, property);
	tableWidget->setSpan(4, 1, 1, 1);
	
	//Lab email
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, tr("E-Mail"));
	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
	tableWidget->setItem(5, 0, property);
	tableWidget->setSpan(5, 0, 1, 1);
	
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, lab->getEmail());
	property->setData(Qt::UserRole, Email);
	tableWidget->setItem(5, 1, property);
	tableWidget->setSpan(5, 1, 1, 1);
	
	//Lab Website
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, tr("Website"));
	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
	tableWidget->setItem(6, 0, property);
	tableWidget->setSpan(6, 0, 1, 1);
	
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, lab->getWebsite());
	property->setData(Qt::UserRole, Website);
	tableWidget->setItem(6, 1, property);
	tableWidget->setSpan(6, 1, 1, 1);
}

