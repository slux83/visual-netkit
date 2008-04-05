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
	
	if(!myProperties.contains(item))
	{
		//not mine
		return returnVal;		
	}
	
	/* Is this property mapped with a Laboratory? */
	if(lab == NULL)
		return false;
		
	/* Select the correct property to sove inside the domain current lab */
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
				lab->setName(item->data(Qt::DisplayRole).toString());
				
				//Update the tree lab view
				LabHandler::getInstance()->getMainWindow()->labTree->topLevelItem(0)->setData(0, Qt::DisplayRole, lab->getName());
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
	
	return returnVal;
}
