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
	tableWidget->setRowCount(2);

	QTableWidgetItem *property = new QTableWidgetItem();
	
	//Cd name
	property->setData(Qt::DisplayRole, tr("Name"));
	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
	tableWidget->setItem(0, 0, property);
	
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, cd->getName());
	property->setData(Qt::UserRole, CdName);
	tableWidget->setItem(0, 1, property);
	
	//Cd network
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, tr("Network"));
	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
	tableWidget->setItem(1, 0, property);
	
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, cd->getSubnet()->toString(true));
	property->setData(Qt::UserRole, CdNetwork);
	property->setData(Qt::ToolTipRole, cd->getSubnet()->toString(false));
	tableWidget->setItem(1, 1, property);

}

/**
 * Save a changed property
 */
bool CdPropertyController::saveChangedProperty(QTableWidgetItem *item)
{
	qDebug() << "Todo:" << item->data(Qt::DisplayRole);
	
	return true;
}

