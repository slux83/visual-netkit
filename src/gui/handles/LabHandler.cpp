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
 * Creates a new lab
 */
void LabHandler::newLab()
{
	LabFacadeController::getInstance()->newLaboratory();
	
	//set the current lab inside the property controller
	propertyController->setLab(LabFacadeController::getInstance()->getCurrentLab());
}

/**
 * [SLOT]
 * Opens an existent lab
 */
void LabHandler::openLab()
{
	LabFacadeController::getInstance()->openLab();
}

/**
 * [SLOT]
 * Saves the lab
 */
void LabHandler::saveLab(const QStringList &selectedFiles)
{
	qDebug() << "where save the lab:" <<  selectedFiles.first();
			
	if(LabFacadeController::getInstance()->saveLab(selectedFiles.first()))
	{
		//ok, lab saved! save state and refresh window header text
		Laboratory *currLab = LabFacadeController::getInstance()->getCurrentLab(); 
		currLab->setSavedState(true);
		currLab->setLabPath(selectedFiles.first() + "/" + currLab->getName());
		mainWindow->setWindowTitle(currLab->getLabPath().absolutePath() + " - VisualNetkit");
		mainWindow->writeLogMessage(tr("Lab saved as: ") + currLab->getLabPath().absolutePath());
	}
	else
	{
		/* Show a warning message */
		QMessageBox::warning(mainWindow, tr("VisualNetkit - Error"),
				tr("There was a problem saving the laboratory data.\nPlease check your access rights on the directory that you have choosed."),
				QMessageBox::Ok);
	}
}


/**
 * [SLOT]
 * Renders a new lab on tree (previusely created)
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
	
	/* Add on scene tree */
	SceneTreeMapper::getInstance()->createRootElement();
}

/**
 * [SLOT]
 * Render a new virtual machine on tree (previously created)
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
	startupConf->setData(0, Qt::UserRole + 1, QString(m->getName() + ".startup"));	//path
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
 * Prepare the handler and foreward the render action to propertyController
 */
void LabHandler::prepareRenderLabProperties()
{
	/* Clear the property editor */
	mainWindow->clearPropertyDock();
	
	/* Disconnect the old handler */
	disconnect(mainWindow->propertyTable, SIGNAL(cellChanged(int, int)), 0, 0);
	
	/* Render properties */
	propertyController->renderLabProperties(mainWindow->propertyTable);
	
	/* Connect the correct handler dinamically */
	connect(mainWindow->propertyTable, SIGNAL(cellChanged(int, int)), 
		this, SLOT(saveChangedProperty(int, int)));
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

/**
 * Get the lab save state
 */
bool LabHandler::getLabState()
{
	if(LabFacadeController::getInstance()->getCurrentLab() == NULL)
		return false;
	
	return LabFacadeController::getInstance()->getCurrentLab()->getSaveState();
}

/**
 * [SLOT]
 * Adds path to the tree.
 * If a node in the path is in the tree, it is not replaced but the rest of the path is 
 * appended as a child.
 */
void LabHandler::addPathToTree(QString path)
{
	// se path non è vuoto
	if (!path.isEmpty())
	{
		QTreeWidgetItem *root = mainWindow->labTree->topLevelItem(0);
		QStringList pathNodes = path.split("/");
		addPathToNode(pathNodes, root);
	}
}

/**
 * Recursively try to add a path (as a tree) to the passed node.
 */
void LabHandler::addPathToNode(QStringList path, QTreeWidgetItem *node)
{
	if (path.size()>0 && node!=NULL)
	{
		// per ogni figlio del nodo corrente
		//for (int c=0; c<node->childCount(); c++)
		//{
			mainWindow->labTree->setCurrentItem(node, 0);
			qDebug() << "nodo corrente settato: " << node->text(0);
			qDebug() << "'--> n° figli:" << node->childCount();
			
			qDebug() << "labtree currentItem --- " << mainWindow->labTree->currentItem()->text(0);
			qDebug() << "'--> n° figli:" << mainWindow->labTree->currentItem()->childCount();
			
			// se il nodo non è presente tra i figli del nodo corrente
			QMap<QString, QTreeWidgetItem*> childs = findItems(path.first(), mainWindow->labTree);
			qDebug() << "(figli con nome" << path.first() << ":" << childs << ")";
			
			if (childs.size() <= 0)
			{
				qDebug() << "nodo" << path.first() << "non presente tra i figli di " << node->text(0);
				
				QTreeWidgetItem *elem = new QTreeWidgetItem();
				
				// se il path è di un solo elemento lo aggiungo come foglia
				if (path.size()==1) 
				{
					qDebug() << "path: " << path;
					qDebug() << "path.first: " << path.first();
					elem->setData(0, Qt::DisplayRole, path.first());
					elem->setData(0, Qt::UserRole, "config_file");		//type
					elem->setData(0, Qt::UserRole + 1, path.first());	//path
					elem->setIcon(0, QIcon(QString::fromUtf8(":/small/file_conf")));

					// aggiungo il nuovo nodo al nodo corrente
					node->addChild(elem);

					return;
				}
				// altrimenti
				else
				{	
					elem->setData(0, Qt::DisplayRole, path.first());
					elem->setData(0, Qt::UserRole, "vm_element");
					elem->setIcon(0, QIcon(QString::fromUtf8(":/small/folder_vm")));
					
					qDebug() << "path: " << path;
					
					// applico la ricorsione con path senza primo nodo
					path.removeFirst();
					qDebug() << "path - first: " << path;
					
					// aggiungo il nuovo nodo al nodo corrente
					node->addChild(elem);
					qDebug() << "nodo corrente: " << node->text(0);
					
					if (!path.isEmpty())
					{
						qDebug() << "ricorsione con: path=" << path << "  elem=" << elem->text(0);
						addPathToNode(path, elem);
					}
				}
			}
			// se il nodo è presente tra i figli del nodo corrente
			else if (childs.size() == 1)
			{
				qDebug() << "il nodo è tra i figli di: " << node->text(0);
				// applico la ricorsione con path senza primo nodo
				// passando come nodo corrente il nodo figlio corrente
				path.removeFirst();
				if (!path.isEmpty())
				{
					qDebug() << "ricorsione con: path=" << path << "  elem=" << childs.value(path.first());
					addPathToNode(path, childs.value(path.first()));
				}
			}
			else 
			{
				qWarning() << "LabHandler::addPathToNode: nodo replicato nel labTree (" << path.first() << ")";
			}
		//}
	}
	else 
	{
		qWarning() << "LabHandler::addPathtoNode: empty QStringList path or null QWidgetItem node";
	}
}

QMap<QString, QTreeWidgetItem*> LabHandler::findItems(QString nodeName, QTreeWidget *node)
{
	QMap<QString, QTreeWidgetItem*> map;
	QTreeWidgetItem *curr = node->currentItem();
	
	//qDebug() << "---- LabHandler::findItems ----";
	
	// scorro tutti i figli del nodo passato
	for (int i=0; i<curr->childCount(); i++) 
	{
		//qDebug() << "nodeName:" << nodeName << "curr->text(0)" << curr->child(i)->text(0);
		//se uno dei figli ha lo stesso nome del nodo tester nodeName
		if (nodeName == curr->child(i)->text(0))
		{
			// lo aggiungo alla mappa
			map.insert(curr->child(i)->text(0), curr->child(i));
		}
	}
	return map;
}

