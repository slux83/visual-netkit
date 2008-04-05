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
	undoStack = new UndoStack();
	propertyController = new LabPropertyController();
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
 * Render a new lab on tree (previusely created)
 */
void LabHandler::addCreatedLabOnTree(Laboratory *l)
{
	qDebug() << "new lab ready to render";
	
	QTreeWidgetItem *root = new QTreeWidgetItem();
	QTreeWidgetItem *labConf = new QTreeWidgetItem();
	
	root->setData(0, Qt::DisplayRole, l->getName());
	
	//type of element
	root->setData(0, Qt::UserRole, "root_element");
	root->setIcon(0, QIcon(QString::fromUtf8(":/small/folder_root")));
	
	labConf->setData(0, Qt::DisplayRole, "lab.conf");
	
	//type of element
	labConf->setData(0, Qt::UserRole, "config_file");
	
	//if config_file, this role show the relative path for the config file
	labConf->setData(0, Qt::UserRole + 1, "lab.conf");
	labConf->setIcon(0, QIcon(QString::fromUtf8(":/small/file_conf")));
	
	root->addChild(labConf);
	
	/* fill the tree view */
	mainWindow->labTree->addTopLevelItem(root);
	mainWindow->labTree->expandItem(root);
	
	emit logEvent(tr("Created a new Lab."));
	
	//unlock scene and actions
	mainWindow->unlockSceneAndActions();
}

/**
 * [SLOT]
 * Render a new virtual machine on tree (previusely created)
 */
void LabHandler::addCreatedVmOnTree(VirtualMachine *m)
{
	qDebug() << "new VM ready to render!";
	
	QTreeWidgetItem *elem = new QTreeWidgetItem();
	QTreeWidgetItem *startupConf = new QTreeWidgetItem();
	
	//Properties
	elem->setData(0, Qt::DisplayRole, m->getName());
	elem->setData(0, Qt::UserRole, "vm_element");
	elem->setIcon(0, QIcon(QString::fromUtf8(":/small/folder_vm")));
	
	startupConf->setData(0, Qt::DisplayRole, QString(m->getName() + ".startup"));
	startupConf->setData(0, Qt::UserRole, "config_file");		//type
	startupConf->setIcon(0, QIcon(QString::fromUtf8(":/small/file_conf")));
	
	QTreeWidgetItem *root = mainWindow->labTree->topLevelItem(0);
	
	if(root == NULL)
	{
		qWarning() << "lab tree top element not found!";
		return;
	}
	
	root->addChild(elem);
	root->addChild(startupConf);

	
	emit logEvent(tr("Created a new virtual machine: ") + m->getName());
	
}
/**
 * [SLOT]
 * Manage the selection of an element inside the tree (lab tree)
 */
void LabHandler::labTreeItemSelected(QTreeWidgetItem * item, int column)
{
	Q_UNUSED(column);
	
	Laboratory *lab = LabFacadeController::getInstance()->getCurrentLab();
	
	/* Clear the property editor */
	clearPropertyDock();
	
	if(item->data(0, Qt::UserRole) == "root_element")
	{
		qDebug() << "Laboratory element selected";
		
		renderLabProperties(lab);
		
	}
}

/**
 * [SLOT]
 * Manage the double click of an element inside the tree (lab tree)
 */
void LabHandler::labTreeItemDoubleClicked(QTreeWidgetItem * item, int column)
{
	Q_UNUSED(column);
	
	Laboratory *lab = LabFacadeController::getInstance()->getCurrentLab();
	
	if(item->data(0, Qt::UserRole) == "config_file")
	{
		qDebug() << "Config file double clicked" << item->data(0, Qt::UserRole + 1).toString();
		
		/* the lab is saved? */
		if(!lab->getSaveState())
		{
			QMessageBox::warning(mainWindow,
					"Visual Netkit - Warning",
					tr("You must save the laboratory before edit this config file!"),
					QMessageBox::Ok);
		}
		else
		{
			//TODO: show this config file inside a new tab if none already exist,
			//		otherwise, show the correct tab
		}
	}
}

/**
 * [SLOT]
 * Clear the content inside the property dock, and reset to 0 the rows count
 */
void LabHandler::clearPropertyDock()
{
	/* Clear all table items and reset the view-size */
	mainWindow->propertyTable->clearContents();		//just only this slot!
	mainWindow->propertyTable->setRowCount(0);		//resize (reset) the view	

	//clear mappings
	propertyController->clearMapping();
}

/**
 * [SLOT]
 * Save a changed property for lab
 */
void LabHandler::saveChangedProperty(int row, int column)
{
	/* Foreward action */
	propertyController->saveChangedProperty(mainWindow->propertyTable->item(row, column));
}

/**
 * [PRIVATE]
 * Render a lab inside property table
 */
void LabHandler::renderLabProperties(Laboratory *l)
{
	clearPropertyDock();
		
	/* render infos inside the property editor */
	mainWindow->propertyTable->setRowCount(mainWindow->propertyTable->rowCount() + 1);

	QTableWidgetItem *property = new QTableWidgetItem();
	
	//Lab name
	property->setData(Qt::DisplayRole, tr("Name"));
	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
	mainWindow->propertyTable->setItem(0, 0, property);
	propertyController->addProperty(property);
	
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, l->getName());
	property->setData(Qt::UserRole, Name);
	mainWindow->propertyTable->setItem(0, 1, property);
	propertyController->addProperty(property);
	
	mainWindow->propertyTable->setRowCount(mainWindow->propertyTable->rowCount() + 1);
	
	//Lab version
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, tr("Version"));
	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
	mainWindow->propertyTable->setItem(1, 0, property);
	propertyController->addProperty(property);
	
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, l->getVersion());
	property->setData(Qt::UserRole, Version);
	mainWindow->propertyTable->setItem(1, 1, property);
	propertyController->addProperty(property);
	
	mainWindow->propertyTable->setRowCount(mainWindow->propertyTable->rowCount() + 1);
	
	//Lab date
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, tr("Date"));
	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
	mainWindow->propertyTable->setItem(2, 0, property);
	propertyController->addProperty(property);
	
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, l->getDate());
	property->setData(Qt::UserRole, Date);
	mainWindow->propertyTable->setItem(2, 1, property);
	propertyController->addProperty(property);
	
	mainWindow->propertyTable->setRowCount(mainWindow->propertyTable->rowCount() + 1);

	//Lab description
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, tr("Description"));
	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
	mainWindow->propertyTable->setItem(3, 0, property);
	propertyController->addProperty(property);
	
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, l->getDescription());
	property->setData(Qt::UserRole, Description);
	mainWindow->propertyTable->setItem(3, 1, property);
	propertyController->addProperty(property);
	
	mainWindow->propertyTable->setRowCount(mainWindow->propertyTable->rowCount() + 1);

	//Lab authors
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, tr("Authors"));
	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
	mainWindow->propertyTable->setItem(4, 0, property);
	propertyController->addProperty(property);
	
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, l->getAuthors());
	property->setData(Qt::UserRole, Authors);
	mainWindow->propertyTable->setItem(4, 1, property);
	propertyController->addProperty(property);
	
	mainWindow->propertyTable->setRowCount(mainWindow->propertyTable->rowCount() + 1);

	//Lab email
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, tr("E-Mail"));
	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
	mainWindow->propertyTable->setItem(5, 0, property);
	propertyController->addProperty(property);
	
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, l->getEmail());
	property->setData(Qt::UserRole, Email);
	mainWindow->propertyTable->setItem(5, 1, property);
	propertyController->addProperty(property);
	
	mainWindow->propertyTable->setRowCount(mainWindow->propertyTable->rowCount() + 1);

	//Lab Website
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, tr("Website"));
	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
	mainWindow->propertyTable->setItem(6, 0, property);
	propertyController->addProperty(property);
	
	property = new QTableWidgetItem();
	property->setData(Qt::DisplayRole, l->getWebsite());
	property->setData(Qt::UserRole, Website);
	mainWindow->propertyTable->setItem(6, 1, property);
	propertyController->addProperty(property);
}
