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
#include "VmMapper.h"
#include "CdMapper.h"
#include "LinkMapper.h"
#include "AreaController.h"
#include "TabController.h"

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
 * Singleton get instance
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
	closeLab();
	
	LabFacadeController::getInstance()->newLaboratory();
	
	//set the current lab inside the property controller
	propertyController->setLab(LabFacadeController::getInstance()->getCurrentLab());
	
	mainWindow->actionCloseLab->setDisabled(false);
	mainWindow->unlockSceneAndActions();
	QRectF sceneSize(0,0,1000,1000);
	mainWindow->setSceneSize(sceneSize);
}

/**
 * [SLOT]
 * Opens an existent lab
 */
void LabHandler::openLab(QString labPath)
{	
	
	closeLab();
	
	LabFacadeController::getInstance()->newLaboratory();
	Laboratory* currLab = LabFacadeController::getInstance()->getCurrentLab();
	
	propertyController->setLab(currLab);
	
	currLab->setLabPath(labPath);
	
	mainWindow->setWindowTitle(currLab->getLabPath().absolutePath() + " - VisualNetkit");
	mainWindow->writeLogMessage(tr("Lab opened: ") + currLab->getLabPath().absolutePath());
	
	mainWindow->actionCloseLab->setDisabled(false);
}

/**
 * [SLOT]
 * Saves the lab
 */
void LabHandler::saveLabAs(const QStringList &selectedFiles)
{
	if(LabFacadeController::getInstance()->saveLab(selectedFiles.first()))
	{
		//ok, lab saved! save state and refresh window header text
		Laboratory *currLab = LabFacadeController::getInstance()->getCurrentLab(); 
		currLab->setLabPath(selectedFiles.first());
		currLab->setSavedState(true);
		currLab->setChangedState(false);
		setMainWindowTitle();
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
 * Save the current lab (because something is changed)
 */
void LabHandler::saveLab()
{
	if(LabFacadeController::getInstance()->saveChandegLab())
	{
		//ok, lab saved! save state and refresh window header text
		Laboratory *currLab = LabFacadeController::getInstance()->getCurrentLab(); 
		currLab->setSavedState(true);
		currLab->setChangedState(false);
		setMainWindowTitle();
		mainWindow->writeLogMessage(tr("Lab saved"));
		
	}
	else
	{
		/* Show a warning message */
		QMessageBox::warning(mainWindow, tr("VisualNetkit - Error"),
				tr("There was a problem saving the laboratory data.\nPlease check your access rights on the directory that you have choosed."),
				QMessageBox::Ok);
	}
	
	setMainWindowTitle();
}

/**
 * [SLOT]
 * Renders a new lab on tree (previusely created)
 */
void LabHandler::addCreatedLabOnTree(Laboratory *l)
{
	Q_UNUSED(l)
	
	qDebug() << "new lab ready to render";
	
	QTreeWidgetItem *root = new QTreeWidgetItem();
	QTreeWidgetItem *labConf = new QTreeWidgetItem();
	
	//type of element
	root->setData(0, Qt::UserRole, "lab_element");
	root->setIcon(0, QIcon(QString::fromUtf8(":/small/folder_root")));
	
	labConf->setData(0, Qt::DisplayRole, LAB_CONF);
	
	//type of element
	labConf->setData(0, Qt::UserRole, "config_file");
	
	//if config_file, this role show the relative path for the config file
	labConf->setData(0, Qt::UserRole + 1, LAB_CONF);
	labConf->setIcon(0, QIcon(QString::fromUtf8(":/small/file_conf")));
	labConf->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	labConf->setCheckState(0, Qt::Checked);
	labConf->setToolTip(0, tr("Check to Save this file."));
	
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
	elem->setData(0, Qt::UserRole, "vm_element");	//don't touch this value!
	elem->setIcon(0, QIcon(QString::fromUtf8(":/small/folder_vm")));
	
	startupConf->setData(0, Qt::DisplayRole, QString(m->getName() + ".startup"));
	startupConf->setData(0, Qt::UserRole, "config_file");		//type (don't change this value)
	startupConf->setData(0, Qt::UserRole + 1, QString(m->getName() + ".startup"));	//path
	startupConf->setIcon(0, QIcon(QString::fromUtf8(":/small/file_conf")));
	startupConf->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	startupConf->setCheckState(0, Qt::Checked);
	startupConf->setToolTip(0, tr("Check to Save this file."));
	
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
			QString pathToOpen = lab->getLabPath().absolutePath().append("/").append(item->data(0, Qt::UserRole + 1).toString());
			
			if(!TabController::getInstance()->openTab(pathToOpen))
				QMessageBox::warning(mainWindow,
					"Visual Netkit - Error",
					tr("Cannot open the file").append(" ").append(pathToOpen),
					QMessageBox::Ok);
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
 * Get the lab changed state
 */
bool LabHandler::getLabChangedState()
{
	if(LabFacadeController::getInstance()->getCurrentLab() == NULL)
		return false;
	
	return LabFacadeController::getInstance()->getCurrentLab()->getChangedState();

}

/**
 * Remove a path from tree
 * Post-conditions: if a vm is inside the path, this will not be deleted
 */
void LabHandler::removePathFromTree(QString &path, bool removeVm)
{
	QTreeWidgetItem *currentNode = mainWindow->labTree->topLevelItem(0);
	QTreeWidgetItem *rootToDelete = NULL;
	QStringList splittedPath = path.split("/");
		
	/* Search the root of the path (no vm dir) to delete */
	foreach(QString piece, splittedPath)
	{
		if(rootToDelete != NULL)
			break;
				
		mainWindow->labTree->setCurrentItem(currentNode, 0);	//update current node to searc in deep
		
		QMap<QString, QTreeWidgetItem*> childs = findItems(piece, mainWindow->labTree);
		
		QMapIterator<QString, QTreeWidgetItem*> childsIterator(childs);
		while(childsIterator.hasNext())
		{
			childsIterator.next();
			
			/* this node is a virtual machine dir? remove? */
			if(!removeVm && childsIterator.value()->data(0, Qt::UserRole).toString() == "vm_element")
			{				
				currentNode = childsIterator.value();	//update current
				break;	//do not delete the vm element
			}
			else
			{
				QTreeWidgetItem *toDelete = childsIterator.value();
								
				rootToDelete = toDelete;
				break;
			}
		}
	}
	
	/* delete from last current node */
	if(rootToDelete != NULL)
	{
		currentNode->removeChild(rootToDelete);
	}
}

/**
 * [SLOT]
 * Adds path to the tree.
 * If a node in the path is in the tree, it is not replaced but the rest of the path is 
 * appended as a child.
 */
void LabHandler::addPathToTree(QString path)
{
	// if path is not empty
	if (!path.isEmpty())
	{
		QTreeWidgetItem *root = mainWindow->labTree->topLevelItem(0);
		QStringList pathNodes = path.split("/");
		addPathToNode(pathNodes, root, path);
	}
}

/**
 * Recursively adds a path (as a tree) to the passed node.
 */
void LabHandler::addPathToNode(QStringList path, QTreeWidgetItem *node, QString fullPath)
{
	// if the path is not empty and the passed tree in not null
	if (path.size()>0 && node!=NULL)
	{
		mainWindow->labTree->setCurrentItem(node, 0);
		
		// if the node is among the children of current node
		QMap<QString, QTreeWidgetItem*> childs = findItems(path.first(), mainWindow->labTree);
		if (childs.size() <= 0)
		{
			QTreeWidgetItem *elem = new QTreeWidgetItem();
			
			// if the path has only one node
			if (path.size()==1) 
			{
				elem->setData(0, Qt::DisplayRole, path.first());
				elem->setData(0, Qt::UserRole, "config_file");		//type
				elem->setData(0, Qt::UserRole + 1, fullPath);		//path
				elem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
				elem->setCheckState(0, Qt::Checked);
				elem->setIcon(0, QIcon(QString::fromUtf8(":/small/file_conf")));
				elem->setToolTip(0, tr("Check to Save this file."));

				// adds the new node to the current node
				node->addChild(elem);

				return;
			}
			// else
			else
			{	
				elem->setData(0, Qt::DisplayRole, path.first());
				elem->setData(0, Qt::UserRole, "generic_element");
				elem->setIcon(0, QIcon(QString::fromUtf8(":/small/folder_vm")));
				
				// apply recursion to the path (without first element)
				path.removeFirst();
				
				// adds the new node to the current node
				node->addChild(elem);
				
				if (!path.isEmpty())
					addPathToNode(path, elem, fullPath);
			}
		}
		// if the node is among the children of the current node
		else if (childs.size() == 1)
		{
			// apply recursion to path (without first node)
			// and the current child node
			QString currNode = path.takeFirst();
			if (!path.isEmpty())
				addPathToNode(path, childs.value(currNode), fullPath);
		}
		else 
		{
			qWarning() << "LabHandler::addPathToNode: replicated node inside labTree (" << path.first() << ")";
		}
	}
}

/**
 * Finds and returns all the children nodes in the passed tree whose name is nodeName
 */
QMap<QString, QTreeWidgetItem*> LabHandler::findItems(QString nodeName, QTreeWidget *tree)
{
	QMap<QString, QTreeWidgetItem*> map;
	QTreeWidgetItem *curr = tree->currentItem();
	
	// visit all children for the passed node
	for (int i=0; i<curr->childCount(); i++) 
	{
		// if a child has the same name as the tester node "nodeName
		if (nodeName == curr->child(i)->text(0))
		{
			// adds it to the map
			map.insert(curr->child(i)->text(0), curr->child(i));
		}
	}
	return map;
}

/**
 * Confirm the lab close
 * if abort is passed, the function set abort to TRUE if the user choosed a
 * abort response (Cancel or No during "close lab" question) 
 */
bool LabHandler::confirmCloseLab(bool *abort)
{
	if(!isCurrentLab())		//there's none lab active
			return true;
		
	/* Check if another lab exist and need to be saved */
	//qDebug() << "changestate:" << getLabChangedState() << "savestate:" << getLabState();
	if(!getLabState() || getLabChangedState())
	{
		qDebug() << "lab exist and need to be saved";
		
		int resp = QMessageBox::question(mainWindow,
				"Visual Netkit - Question",
				tr("The lab is changed or not saved.\nDo you want save it?"),
				QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);
		
		if(resp == QMessageBox::Yes)
		{
			/* Save or save as? */			
			if(!getLabState())	//save as
			{
				mainWindow->showSaveFileDialog();
				return false;
			}
			
			if(getLabChangedState())	//save changes
			{
				saveLab();
			}
		}
		
		if(resp == QMessageBox::Cancel)
		{
			if(abort != NULL)
				*abort = true;
			return false;
		}
		
		return true;	//close the lab anyway
	}
	
	/* Close the current lab if any */
	int resp = QMessageBox::question(mainWindow,
			"Visual Netkit - Question",
			tr("The lab is allready saved. Do you want close it?"),
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	
	if(resp == QMessageBox::No)
	{
		if(abort != NULL)
			*abort = true;
		return false;
	}
	
	return true;
	
}

/**
 * [SLOT]
 * Close the lab: clear mappings, scene, graphics items and Main gui.
 */
void LabHandler::closeLab()
{
	if(!confirmCloseLab())
		return;
	
	/* Close all tabs */
	TabController::getInstance()->closeAllTabs();
	
	//Destroy mappings
	VmMapper::getInstance()->clear();
	CdMapper::getInstance()->clear();
	LinkMapper::getInstance()->clear();
	AreaController::getInstance()->clear();
	
	//Destroy all items
	mainWindow->getLabScene()->clearScene();
	
	//reset tree views
	mainWindow->labTree->clear();
	SceneTreeMapper::getInstance()->clear();
	
	//other gui stuffs
	mainWindow->clearPropertyDock();
	mainWindow->setWindowTitle("VisualNetkit");
	mainWindow->lockSceneAndActions();
	
	//Destroy low level stuff
	LabFacadeController::getInstance()->closeLowLevelLab();
	
	mainWindow->writeLogMessage("Lab Closed");
	
	mainWindow->actionCloseLab->setDisabled(true);
	
	/* Destroy undo stack */
	undoStack->clear();
}

/**
 * Close the lab (forced): clear mappings, scene, graphics items and Main gui.
 */
void LabHandler::closeLabForced()
{
	/* Close all tabs */
	TabController::getInstance()->closeAllTabs();
	
	//Destroy mappings
	VmMapper::getInstance()->clear();
	CdMapper::getInstance()->clear();
	LinkMapper::getInstance()->clear();
	
	//Destroy all items
	mainWindow->getLabScene()->clearScene();
	
	//reset tree views
	mainWindow->labTree->clear();
	SceneTreeMapper::getInstance()->clear();
	
	//other gui stuffs
	mainWindow->clearPropertyDock();
	mainWindow->setWindowTitle("VisualNetkit");
	mainWindow->lockSceneAndActions();
	
	//Destroy low level stuff
	LabFacadeController::getInstance()->closeLowLevelLab();
	
	mainWindow->writeLogMessage("Lab Closed");
	
	mainWindow->actionCloseLab->setDisabled(true);
	
	/* Destroy undo stack */
	undoStack->clear();
}

/**
 * Set the lab scene size
 */
void LabHandler::setSceneSize(QRectF &size)
{
	setChangedLabState();
	mainWindow->setSceneSize(size);
}

/**
 * Change the save state of the currect lab
 */
void LabHandler::setSaveLabState(bool state)
{
	LabFacadeController::getInstance()->getCurrentLab()->setSavedState(state);
}

/**
 * Set the 'changed' state of the current lab
 */
void LabHandler::setChangedLabState(bool state)
{
	Laboratory* l = LabFacadeController::getInstance()->getCurrentLab();
	
	if(l == NULL)
		return;
	
	if(l->getSaveState() && state && !l->getChangedState())
	{
		l->setChangedState(state);
		//mark with a "changed"
		mainWindow->setWindowTitle(l->getLabPath().absolutePath().append(" - VisualNetkit") + " (" + tr("changed") + ")");
	}
}

/**
 * Check if a current lab exist
 */
bool LabHandler::isCurrentLab()
{
	return (LabFacadeController::getInstance()->getCurrentLab() != NULL);
}

/**
 * Set the main window title
 */
void LabHandler::setMainWindowTitle()
{
	Laboratory* currLab = LabFacadeController::getInstance()->getCurrentLab();
	
	if(isCurrentLab())
	{
		mainWindow->setWindowTitle(currLab->getLabPath().absolutePath() + " - VisualNetkit");
	}
}

/**
 * Remove an item from the lab scene
 */
void LabHandler::removeItemFromScene(QGraphicsItem *item)
{
	mainWindow->graphicsView->scene()->removeItem(item);
}

/**
 * Return a list of excluded paths (unchecked nodes)
 * Recursive tree visit
 */
QStringList LabHandler::getExcludePaths(QTreeWidgetItem *currNode)
{
	QStringList paths;
	QTreeWidget *tree = mainWindow->labTree;
	
	if(!currNode)
		tree->setCurrentItem(tree->topLevelItem(0), 0);
	else
		tree->setCurrentItem(currNode, 0);
	
	QTreeWidgetItem *curr = tree->currentItem();
	
	// visit all children for the passed node
	for(int i=0; i<curr->childCount(); i++) 
	{
		//it's a selected leaf?
		if(curr->child(i)->data(0, Qt::UserRole).toString() == "config_file" &&
			curr->child(i)->checkState(0) == Qt::Unchecked)
		{
			paths << curr->child(i)->data(0, Qt::UserRole + 1).toString();
		}
		
		//it's a folder?
		if(curr->child(i)->data(0, Qt::UserRole).toString() == "vm_element" || 
			curr->child(i)->data(0, Qt::UserRole).toString() == "generic_element")
		{
			paths << getExcludePaths(curr->child(i));
		}
	}
	
	return paths;
}
