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
#include "../../common/CommonConfigs.h"

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
	
	//set the current lab inside the property controller
	propertyController->setLab(LabFacadeController::getInstance()->getCurrentLab());
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
	root->setData(0, Qt::UserRole, "lab_element");
	root->setIcon(0, QIcon(QString::fromUtf8(":/small/folder_root")));
	
	labConf->setData(0, Qt::DisplayRole, LAB_CONF);
	
	//type of element
	labConf->setData(0, Qt::UserRole, "config_file");
	
	//if config_file, this role show the relative path for the config file
	labConf->setData(0, Qt::UserRole + 1, LAB_CONF);
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
	startupConf->setData(0, Qt::UserRole +1 , QString(m->getName() + ".startup"));	//path
	startupConf->setIcon(0, QIcon(QString::fromUtf8(":/small/file_conf")));
	
	/* create subdirs if daemons are found */
	if(m->getMyType() == Router)
	{
		// etc dir
		QTreeWidgetItem *etc = new QTreeWidgetItem();
		etc->setData(0, Qt::DisplayRole, ETC_PATH);
		etc->setIcon(0, QIcon(QString::fromUtf8(":/small/folder_vm")));
		elem->addChild(etc);
		
		// zebra dir
		QTreeWidgetItem *zebraDir = new QTreeWidgetItem();
		zebraDir->setData(0, Qt::DisplayRole, ZEBRA_PATH);
		zebraDir->setIcon(0, QIcon(QString::fromUtf8(":/small/folder_vm")));
		
		etc->addChild(zebraDir);
		
		// config files
		QTreeWidgetItem *daemonFile, *bgpdFile, *ripdFile;
		
		daemonFile = new QTreeWidgetItem();
		daemonFile->setData(0, Qt::DisplayRole, ZEBRA_DAEMONS_CONF);
		daemonFile->setData(0, Qt::UserRole, "config_file");		//type
		daemonFile->setData(0, Qt::UserRole +1 , 
				QString(m->getName() + "/" +
						ETC_PATH + "/" + ZEBRA_PATH + "/" +
						ZEBRA_DAEMONS_CONF));	//path
		daemonFile->setIcon(0, QIcon(QString::fromUtf8(":/small/file_conf")));
		zebraDir->addChild(daemonFile);
		
		//bgp?
		if(m->getDm()->getDaemonState(Bgpd))
		{
			bgpdFile = new QTreeWidgetItem();
			bgpdFile->setData(0, Qt::DisplayRole, BGPD_CONF);
			bgpdFile->setData(0, Qt::UserRole, "config_file");		//type
			bgpdFile->setData(0, Qt::UserRole +1 , 
					QString(m->getName() + "/" +
							ETC_PATH + "/" + ZEBRA_PATH + "/" +
							BGPD_CONF));	//path
			bgpdFile->setIcon(0, QIcon(QString::fromUtf8(":/small/file_conf")));
			zebraDir->addChild(bgpdFile);
		}
		
		//rip?
		if(m->getDm()->getDaemonState(Ripd))
		{
			ripdFile = new QTreeWidgetItem();
			ripdFile->setData(0, Qt::DisplayRole, RIPD_CONF);
			ripdFile->setData(0, Qt::UserRole, "config_file");		//type
			ripdFile->setData(0, Qt::UserRole +1 , 
					QString(m->getName() + "/" +
							ETC_PATH + "/" + ZEBRA_PATH + "/" +
							RIPD_CONF));	//path
			ripdFile->setIcon(0, QIcon(QString::fromUtf8(":/small/file_conf")));
			zebraDir->addChild(ripdFile);
		}

				
	}
	
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
	
	/* Clear the property editor */
	propertyController->clearPropertyDock(mainWindow->propertyTable);
	
	if(item->data(0, Qt::UserRole) == "lab_element")
	{
		qDebug() << "Laboratory element selected";
		
		propertyController->renderLabProperties(mainWindow->propertyTable);
		
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
					tr("You must save the laboratory before edit this config file."),
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
 * Save a changed property for lab
 */
void LabHandler::saveChangedProperty(int row, int column)
{
	/* Foreward action */
	propertyController->saveChangedProperty(mainWindow->propertyTable->item(row, column));
}

