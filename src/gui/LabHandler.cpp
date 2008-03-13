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

#include "LabHandler.h"
#include "../core/LabFacadeController.h"
#include "LabPropertyController.h"
#include <QTreeWidgetItem>
#include <QTableWidgetItem>
#include <QIcon>
#include <QMessageBox>
/**
 * Init the null instance for the singletone controller
 */
LabHandler* LabHandler::instance = NULL;

/**
 * Constructor
 */
LabHandler::LabHandler() : QObject()
{
	mainWindow = NULL;
}

/**
 * Deconstructor
 */
LabHandler::~LabHandler()
{
}

/**
 * Set the main window
 */
void LabHandler::setMainWindow(MainWindow *w)
{
	if(mainWindow == NULL)
		mainWindow = w;
}

/**
 * Singletone get instance
 */
LabHandler* LabHandler::getInstance()
{
	if (instance == NULL)
	{
		instance = new LabHandler();
	}

	return instance;
}

/**
 * [SLOT]
 * Create a new lab
 */
void LabHandler::newLab()
{
	LabFacadeController::getInstance()->newLaboratory();
}

/**
 * [SLOT]
 * Render a new lab (previusely created)
 */
void LabHandler::showCreatedLab(Laboratory *l)
{
	qDebug() << "new lab ready to render";
	
	QTreeWidgetItem *root = new QTreeWidgetItem();
	root->setData(0, Qt::DisplayRole, l->getName());
	root->setData(0, Qt::UserRole, "root_element");
	root->setIcon(0,QIcon(QString::fromUtf8(":/small/folder_root")));
	
	
	/* fill the tree view */
	mainWindow->labTree->addTopLevelItem(root);
}

/**
 * [SLOT]
 * Manage the selection of a element inside the tree (lab tree)
 */
void LabHandler::labTreeItemSelected(QTreeWidgetItem * item, int column)
{
	Q_UNUSED(column);
	
	if(item->data(0, Qt::UserRole) == "root_element")
	{
		qDebug() << "Laboratory element selected";
		Laboratory *lab = LabFacadeController::getInstance()->getCurrentLab();
		
		if(lab == NULL)
			return;
		
		renderLabProperties(lab);
		
	}
}

/**
 * [SLOT]
 * Clear the content inside the property editor, and reset to 0 the rows count
 */
void LabHandler::clearPropertyEditor()
{
	/* Clear all table items and reset the view-size */
	mainWindow->propertyTable->clearContents();		//just only this slot!
	mainWindow->propertyTable->setRowCount(0);		//resize (reset) the view	

	//clear mappings
	LabPropertyController::getInstance()->clearMapping();
}

/**
 * [SLOT]
 * Save a changed property for lab
 */
void LabHandler::saveChangedProperty(int row, int column)
{
	/* get the target property */
	QTableWidgetItem *changed = mainWindow->propertyTable->item(row, column);
	
	/* search if this property is correlated with the Laboratory */
	Laboratory *l = LabPropertyController::getInstance()->getLabGivedView(changed);
	
	/* Is this property mapped with a Laboratory? */
	if(l == NULL)
		return;
		
	/* Select the correct property to sove inside the domain current lab */
	switch (changed->data(Qt::UserRole).toInt())
	{
		case Name:
			if(changed->data(Qt::DisplayRole).toString().isEmpty())
			{
				//Restore the value, and alert the user
				changed->setData(Qt::DisplayRole, l->getName());
				QMessageBox::warning(NULL, tr("Visual Netkit"),
		                   tr("The laboratory name must be not empty!"),
		                   QMessageBox::Ok);
			}
			else
			{
				l->setName(changed->data(Qt::DisplayRole).toString());
				//TODO: edit the name on tree view
			}
			
			break;
		
		case Version: l->setVersion(changed->data(Qt::DisplayRole).toString());
			break;
		
		case Date: l->setDate(changed->data(Qt::DisplayRole).toString());
			break;

		case Description: l->setDescription(changed->data(Qt::DisplayRole).toString());
			break;
		
		case Authors: l->setAuthors(changed->data(Qt::DisplayRole).toString());
			break;
		
		case Email: l->setEmail(changed->data(Qt::DisplayRole).toString());
			break;
		
		case Website: l->setWebsite(changed->data(Qt::DisplayRole).toString());
			break;
	}
}

/**
 * [PRIVATE]
 * Render a lab inside property table
 */
void LabHandler::renderLabProperties(Laboratory *l)
{
	clearPropertyEditor();
		
	/* render infos inside the property editor */
	mainWindow->propertyTable->setRowCount(mainWindow->propertyTable->rowCount() + 1);

	QTableWidgetItem *property = new QTableWidgetItem();
	
	//Lab name
	property->setData(Qt::DisplayRole, tr("Name"));
	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
	mainWindow->propertyTable->setItem(0, 0, property);
	LabPropertyController::getInstance()->addMapping(
			QPair<QTableWidgetItem *, Laboratory *>(property, l));
	
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, l->getName());
	property->setData(Qt::UserRole, Name);
	mainWindow->propertyTable->setItem(0, 1, property);
	LabPropertyController::getInstance()->addMapping(
				QPair<QTableWidgetItem *, Laboratory *>(property, l));
	
	mainWindow->propertyTable->setRowCount(mainWindow->propertyTable->rowCount() + 1);
	
	//Lab version
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, tr("Version"));
	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
	mainWindow->propertyTable->setItem(1, 0, property);
	LabPropertyController::getInstance()->addMapping(
				QPair<QTableWidgetItem *, Laboratory *>(property, l));
	
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, l->getVersion());
	property->setData(Qt::UserRole, Version);
	mainWindow->propertyTable->setItem(1, 1, property);
	LabPropertyController::getInstance()->addMapping(
				QPair<QTableWidgetItem *, Laboratory *>(property, l));
	
	mainWindow->propertyTable->setRowCount(mainWindow->propertyTable->rowCount() + 1);
	
	//Lab date
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, tr("Date"));
	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
	mainWindow->propertyTable->setItem(2, 0, property);
	LabPropertyController::getInstance()->addMapping(
					QPair<QTableWidgetItem *, Laboratory *>(property, l));
	
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, l->getDate());
	property->setData(Qt::UserRole, Date);
	mainWindow->propertyTable->setItem(2, 1, property);
	LabPropertyController::getInstance()->addMapping(
					QPair<QTableWidgetItem *, Laboratory *>(property, l));
	
	mainWindow->propertyTable->setRowCount(mainWindow->propertyTable->rowCount() + 1);

	//Lab description
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, tr("Description"));
	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
	mainWindow->propertyTable->setItem(3, 0, property);
	LabPropertyController::getInstance()->addMapping(
					QPair<QTableWidgetItem *, Laboratory *>(property, l));
	
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, l->getDescription());
	property->setData(Qt::UserRole, Description);
	mainWindow->propertyTable->setItem(3, 1, property);
	LabPropertyController::getInstance()->addMapping(
					QPair<QTableWidgetItem *, Laboratory *>(property, l));
	
	mainWindow->propertyTable->setRowCount(mainWindow->propertyTable->rowCount() + 1);

	//Lab authors
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, tr("Authors"));
	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
	mainWindow->propertyTable->setItem(4, 0, property);
	LabPropertyController::getInstance()->addMapping(
					QPair<QTableWidgetItem *, Laboratory *>(property, l));
	
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, l->getAuthors());
	property->setData(Qt::UserRole, Authors);
	mainWindow->propertyTable->setItem(4, 1, property);
	LabPropertyController::getInstance()->addMapping(
					QPair<QTableWidgetItem *, Laboratory *>(property, l));
	
	mainWindow->propertyTable->setRowCount(mainWindow->propertyTable->rowCount() + 1);

	//Lab email
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, tr("E-Mail"));
	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
	mainWindow->propertyTable->setItem(5, 0, property);
	LabPropertyController::getInstance()->addMapping(
			QPair<QTableWidgetItem *, Laboratory *>(property, l));
	
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, l->getEmail());
	property->setData(Qt::UserRole, Email);
	mainWindow->propertyTable->setItem(5, 1, property);
	LabPropertyController::getInstance()->addMapping(
					QPair<QTableWidgetItem *, Laboratory *>(property, l));
	
	mainWindow->propertyTable->setRowCount(mainWindow->propertyTable->rowCount() + 1);

	//Lab Website
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, tr("Website"));
	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
	mainWindow->propertyTable->setItem(6, 0, property);
	LabPropertyController::getInstance()->addMapping(
					QPair<QTableWidgetItem *, Laboratory *>(property, l));
	
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, l->getWebsite());
	property->setData(Qt::UserRole, Website);
	mainWindow->propertyTable->setItem(6, 1, property);
	LabPropertyController::getInstance()->addMapping(
					QPair<QTableWidgetItem *, Laboratory *>(property, l));
}
