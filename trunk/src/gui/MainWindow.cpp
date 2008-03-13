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
#include <QHeaderView>
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
	
	propertyTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	
	//by default don't show the dock for logging
	dockLog->setVisible(false);
	
	//populte the 'View' menu
	populateViewMenu();
	
	createConnections();
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
}

