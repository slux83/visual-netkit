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
#include <QTreeWidgetItem>
#include <QTableWidgetItem>
#include <QIcon>

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
	
	//Disable the action (need a "close lab action")
	//mainWindow->actionNewLab->setEnabled(false);
}

void LabHandler::labTreeItemSelected(QTreeWidgetItem * item, int column)
{
	Q_UNUSED(column);
	
	if(item->data(0, Qt::UserRole) == "root_element")
	{
		qDebug() << "Laboratory element selected";
		Laboratory *lab = LabFacadeController::getInstance()->getCurrentLab();
		
		if(lab == NULL)
			return;
		
		//qDebug() << "rows:" << mainWindow->propertyTable->rowCount();
		/* render infos inside the property editor */
		mainWindow->propertyTable->setRowCount(mainWindow->propertyTable->rowCount() + 1);

		QTableWidgetItem *property = new QTableWidgetItem();
		//Lab name
		property->setData(Qt::DisplayRole, tr("name"));
		
		mainWindow->propertyTable->setItem(0, 0, property);
		property = new QTableWidgetItem();
		property->setData(Qt::DisplayRole, lab->getName());
		mainWindow->propertyTable->setItem(0, 1, property);
		//TODO: need a refactoring...
	}
}
