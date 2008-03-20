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

#include "MainWindow.h"
#include "LabHandler.h"
#include "GraphicsView.h"
#include <QHeaderView>
#include <QActionGroup>
/**
 * Constructor
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	/* Get the reference to controllers */
	labHandler = LabHandler::getInstance();
	labHandler->setMainWindow(this);
	
	/* Connect the UI resource to this QMainWindow */
	setupUi(this);
	
	/* Some settings */
	showMaximized();
	createActionGroups();
	propertyTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	
	//by default don't show the dock for logging
	dockLog->setVisible(false);
	
	//populte the 'View' menu
	populateViewMenu();
	
	//connections and scene
	createConnections();
	createScene();
	
	//status bar show a ready state
	statusBar()->showMessage(tr("Ready"));
}

/**
 * Deconstructor
 */
MainWindow::~MainWindow()
{
}

/**
 * This function populate the 'View' menu' that show/hide docks
 * (by default this menu' in under context menu' on QMainWindow
 */
void MainWindow::populateViewMenu()
{
	menuView->addAction(dockLab->toggleViewAction());
	menuView->addAction(dockProperty->toggleViewAction());
	menuView->addAction(dockMiniature->toggleViewAction());
	menuView->addAction(dockTreeView->toggleViewAction());
	menuView->addAction(dockLog->toggleViewAction());
	menuView->addSeparator();
	menuView->addAction(toolBarGeneralActions->toggleViewAction());
	menuView->addAction(toolBarDrawing->toggleViewAction());
}

/**
 * [PRIVATE]
 * Create connctions
 */
void MainWindow::createConnections()
{
	//connect: new lab action
	connect(actionNewLab, SIGNAL(triggered()), labHandler, SLOT(newLab()));
	
	//connect: item tree lab selected
	connect(labTree, SIGNAL(itemClicked(QTreeWidgetItem * , int)),
			labHandler, SLOT(labTreeItemSelected(QTreeWidgetItem * , int)));
	
	//connect: property changed (property table)
	connect(propertyTable, SIGNAL(cellChanged(int, int)), 
			labHandler, SLOT(saveChangedProperty(int, int)));
	
	//connect: a log event
	connect(labHandler, SIGNAL(logEvent(QString)), this, SLOT(writeLogMessage(QString)));

}

/**
 * [SLOT]
 * Write a log line with time
 */
void MainWindow::writeLogMessage(QString message)
{
	logText->append("[" + QTime::currentTime().toString("HH:mm:ss:zzz") + "] " + message);
}

/**
 * [PRIVATE]
 * Create action groups
 */
void MainWindow::createActionGroups()
{
	/* Group for graphic components */
	QActionGroup *labItemGroup = new QActionGroup(this);
	//add Actions to the group
	labItemGroup->addAction(actionAddVirtualMachine);
	labItemGroup->addAction(actionAddCollisionDomain);
	labItemGroup->setDisabled(true);
	
	/* Group for scene size */
	QActionGroup *sceneSizeGroup = new QActionGroup(this);
	//add Actions to the group
	sceneSizeGroup->addAction(actionSmall);
	actionSmall->setData(QRectF(0, 0, 500, 500));
	sceneSizeGroup->addAction(actionNormal);
	actionNormal->setData(QRectF(0, 0, 1000, 1000));
	sceneSizeGroup->addAction(actionBig);
	actionBig->setData(QRectF(0, 0, 2000, 2000));
	sceneSizeGroup->addAction(actionHuge);
	actionHuge->setData(QRectF(0, 0, 4000, 4000));
	//sceneSizeGroup->setDisabled(true);
	//connect: action for resize scene
	connect(sceneSizeGroup, SIGNAL(triggered(QAction *)), this, SLOT(resizeScene(QAction *)));
}

/**
 * [PRIVATE]
 * Create the scene
 */
void MainWindow::createScene()
{
	LabScene *scene = new LabScene();
	graphicsView->setScene(scene);
}

/**
 * [PRIVATE SLOT]
 * Change the scene size after user interaction
 */
void MainWindow::resizeScene(QAction *action)
{
	if(!action->isChecked())
		return;
	
	QRectF newSize = action->data().toRectF(); 
	graphicsView->scene()->setSceneRect(newSize);
	writeLogMessage(tr("Scene resized") + " (" +
			QByteArray::number(newSize.width()) +
			"x" +
			QByteArray::number(newSize.height()) + " pixel)");
}