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
#include "handles/LabHandler.h"
#include "handles/SettingsHandler.h"
#include "handles/VmHandler.h"
#include "GraphicsView.h"
#include <QHeaderView>
#include <QActionGroup>
#include <QDir>

#include <QPrinter>
#include <QPainter>
#include <QSvgGenerator>

/**
 * Constructor
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	/* Get the reference to controllers */
	labHandler = LabHandler::getInstance();
	labHandler->setMainWindow(this);
	vmHandler = VmHandler::getInstance();
	
	/* Connect the UI resource to this QMainWindow */
	setupUi(this);
	
	/* Some settings */
	createActionGroups();
	//propertyTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	
	//by default don't show the dock for logging
	dockLog->setVisible(false);
	
	/* init the file dialog (save mode) */
	saveFileDialog = new QFileDialog(this, tr("Save laboratory as..."), QDir::homePath(), "");
	saveFileDialog->setFileMode(QFileDialog::AnyFile);
	saveFileDialog->setAcceptMode(QFileDialog::AcceptSave);
	saveFileDialog->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
	
	aboutDialog = NULL;
	
	//init actian menu
	actionMenu = new QMenu();
	actionsToolButton->setMenu(actionMenu);
		
	//populte the 'View' menu
	populateViewMenu();
	
	//connections and scene
	createConnections();
	createScene();
	
	/* restore window settings */
	restoreWindow();
	
	/* build the menu' lab history */
	buildLabHistory(SettingsHandler::getInstance()->getLabHistory());
	
	//status bar show a ready state
	statusBar()->showMessage(tr("Ready"), 10000);
	
	clearPropertyDock();
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
	/* Add some shortcut */
	dockLab->toggleViewAction()->setShortcut(QApplication::translate("NetkitMainWindow", "Alt+L", 0, QApplication::UnicodeUTF8));
	dockLab->setWindowTitle(dockLab->windowTitle() + " [" + dockLab->toggleViewAction()->shortcut().toString() + "]");
	
	dockProperty->toggleViewAction()->setShortcut(QApplication::translate("NetkitMainWindow", "Alt+P", 0, QApplication::UnicodeUTF8));
	dockProperty->setWindowTitle(dockProperty->windowTitle() + " [" + dockProperty->toggleViewAction()->shortcut().toString() + "]");

	dockMiniature->toggleViewAction()->setShortcut(QApplication::translate("NetkitMainWindow", "Alt+M", 0, QApplication::UnicodeUTF8));
	dockMiniature->setWindowTitle(dockMiniature->windowTitle() + " [" + dockMiniature->toggleViewAction()->shortcut().toString() + "]");
	
	dockTreeView->toggleViewAction()->setShortcut(QApplication::translate("NetkitMainWindow", "Alt+E", 0, QApplication::UnicodeUTF8));
	dockTreeView->setWindowTitle(dockTreeView->windowTitle() + " [" + dockTreeView->toggleViewAction()->shortcut().toString() + "]");

	dockLog->toggleViewAction()->setShortcut(QApplication::translate("NetkitMainWindow", "Alt+G", 0, QApplication::UnicodeUTF8));
	dockLog->setWindowTitle(dockLog->windowTitle() + " [" + dockLog->toggleViewAction()->shortcut().toString() + "]");

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
	
	//connect: save lab action
	connect(actionSaveAs, SIGNAL(triggered()), this, SLOT(showSaveFileDialog()));
	connect(actionSave, SIGNAL(triggered()), this, SLOT(saveModifiedLab()));
	
	//connect: the save file dialog to the controller handler
	connect(saveFileDialog, SIGNAL(filesSelected(const QStringList &)),
			labHandler, SLOT(saveLabAs(const QStringList &)));
	
	//connect: item tree lab double clicked
	connect(labTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem * , int)),
			labHandler, SLOT(labTreeItemDoubleClicked(QTreeWidgetItem * , int)));

	//connect: a log event
	connect(labHandler, SIGNAL(logEvent(QString)), this, SLOT(writeLogMessage(QString)));
	connect(vmHandler, SIGNAL(logEvent(QString)), this, SLOT(writeLogMessage(QString)));
	
	//connect: undo/redo actions with QUndoStack state
	connect(labHandler->getUndoStack(), SIGNAL(canRedoChanged(bool)),
			actionRedo, SLOT(setEnabled(bool)));

	connect(labHandler->getUndoStack(), SIGNAL(canUndoChanged(bool)),
			actionUndo, SLOT(setEnabled(bool)));
	
	//connect: zoom actions
	connect(zoomSlider, SIGNAL(valueChanged(int)),
			this, SLOT(zoomLabView(int)));
	connect(actionZoomIn, SIGNAL(triggered()),
				this, SLOT(zoomPlus()));
	connect(actionZoomOut, SIGNAL(triggered()),
					this, SLOT(zoomMinus()));
	connect(actionZoomOriginal, SIGNAL(triggered()),
					this, SLOT(zoomNormal()));

	//connect: export to SVG image action
	connect(actionSVGImage, SIGNAL(triggered()), this, SLOT(dumpToSVG()));
	
	//connect: export to PDF image action
	connect(actionPDFImage, SIGNAL(triggered()), this, SLOT(dumpToPDF()));
	
	//connect: export to PNG image action
	connect(actionPNGImage, SIGNAL(triggered()), this, SLOT(dumpToPNG()));
	
	//connect: open lab
	connect(actionOpenLab, SIGNAL(triggered()), this, SLOT(openLab()));
	
	//connect: close lab
	connect(actionCloseLab, SIGNAL(triggered()), labHandler, SLOT(closeLab()));
	
	//connect: action full screen mode
	connect(actionFullScreenMode, SIGNAL(triggered()), this, SLOT(fullscreenMode()));
	
	//connect: build history
	connect(SettingsHandler::getInstance(), SIGNAL(historyChanged()),
				this, SLOT(rebuildHistory()));
	
	//connect: history menu' trigger
	connect(menuHistory, SIGNAL(triggered(QAction*)),
			this, SLOT(handleHistoryAction(QAction*)));
	
	//connect: about action
	connect(actionAboutVisualNetkit, SIGNAL(triggered()),
			this, SLOT(showAbout()));
	
	//connect: action property menu
	connect(actionMenu, SIGNAL(triggered(QAction*)),
			this, SLOT(handlePropertyAction(QAction*)));
}

/**
 * [SLOT]
 * Write a log line with time
 */
void MainWindow::writeLogMessage(QString message)
{
	logText->append("[" + QTime::currentTime().toString("HH:mm:ss:zzz") + "] " + message);
	
	//set status bar
	statusBar()->showMessage(message , 0);
	
	if(!message.contains("Lab saved"))
		labHandler->setChangedLabState();
}

/**
 * [PRIVATE]
 * Create action groups
 */
void MainWindow::createActionGroups()
{
	/* Group for graphic components */
	labItemGroup = new QActionGroup(this);
	//add Actions to the group
	labItemGroup->addAction(actionAddVirtualMachine);
	labItemGroup->addAction(actionAddCollisionDomain);
	labItemGroup->addAction(actionManageGraph);
	labItemGroup->addAction(actionAddLink);
	labItemGroup->addAction(actionSelectionTool);
	labItemGroup->addAction(actionAddArea);
	labItemGroup->setDisabled(true);
	
	/* Group for scene size */
	sceneSizeGroup = new QActionGroup(this);
	//add Actions to the group
	sceneSizeGroup->addAction(actionSmall);
	actionSmall->setData(QRectF(0, 0, 500, 500));
	sceneSizeGroup->addAction(actionNormal);
	actionNormal->setData(QRectF(0, 0, 1000, 1000));
	sceneSizeGroup->addAction(actionBig);
	actionBig->setData(QRectF(0, 0, 2000, 2000));
	sceneSizeGroup->addAction(actionHuge);
	actionHuge->setData(QRectF(0, 0, 4000, 4000));
	sceneSizeGroup->setDisabled(true);
	//connect: action for resize scene
	connect(sceneSizeGroup, SIGNAL(triggered(QAction *)), this, SLOT(resizeScene(QAction *)));
	
	/* Group for zoom the scene */
	graphZoomGroup = new QActionGroup(this);
	graphZoomGroup->addAction(actionZoomIn);
	graphZoomGroup->addAction(actionZoomOriginal);
	graphZoomGroup->addAction(actionZoomOut);
	graphZoomGroup->setDisabled(true);
}

/**
 * [PRIVATE]
 * Create the scene
 */
void MainWindow::createScene()
{
	scene = new LabScene();
	graphicsView->setScene(scene);
	graphicsView->setVisible(false);
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
	
	//update the miniature scale factor
	updateMinuatureDock((int)newSize.width());
	
	labHandler->setChangedLabState();
	
	//Log action
	writeLogMessage(tr("Scene resized") + " (" +
			QByteArray::number(newSize.width()) +
			"x" +
			QByteArray::number(newSize.height()) + " pixel)");
}

/**
 * Set the scene size (used by open lab UC)
 */
void MainWindow::setSceneSize(QRectF &size)
{
	graphicsView->scene()->setSceneRect(size);
	
	//Select the correct action
	if(actionSmall->data().toRectF() == size)
			actionSmall->setChecked(true);
		
	if(actionSmall->data().toRectF() == size)
			actionSmall->setChecked(true);
		
	if(actionNormal->data().toRectF() == size)
			actionNormal->setChecked(true);
		
	if(actionBig->data().toRectF() == size)
			actionBig->setChecked(true);
		
	if(actionHuge->data().toRectF() == size)
			actionHuge->setChecked(true);
		
		
	//update the miniature scale factor
	updateMinuatureDock((int)size.width());
	
}

/**
 * Unlock the scene and correlated actions
 */
void MainWindow::unlockSceneAndActions()
{
	graphicsView->setVisible(true);
	graphicsView->setDisabled(false);
	graphicsView->ensureVisible(0, 0, 100, 100);
	sceneSizeGroup->setDisabled(false);
	labItemGroup->setDisabled(false);
	menuGraph->setDisabled(false);
	graphZoomGroup->setDisabled(false);
	
	/* save and save as... */
	actionSave->setDisabled(false);
	actionSaveAs->setDisabled(false);
	
	//select the default action
	forceManageGraphAction();
	
	initMiniatureDock();
}

/**
 * Lock the scene and correlated actions
 */
void MainWindow::lockSceneAndActions()
{
	graphicsView->setVisible(false);
	graphicsView->setDisabled(true);
	sceneSizeGroup->setDisabled(true);
	labItemGroup->setDisabled(true);
	menuGraph->setDisabled(true);
	graphZoomGroup->setDisabled(true);
	
	/* save and save as... */
	actionSave->setDisabled(true);
	actionSaveAs->setDisabled(true);
	
	labMiniature->setDisabled(true);
	labMiniature->setScene(NULL);
	
}


/**
 * [PRIVATE]
 * init the miniature dock
 */
void MainWindow::initMiniatureDock()
{
	QTransform transform;
	transform.scale(.2, .2);
	labMiniature->setScene(graphicsView->scene());
	labMiniature->setTransform(transform);
}

/**
 * [PRIVATE]
 * update the miniature when the scene rect is changed
 */
void MainWindow::updateMinuatureDock(int size)
{
	QTransform transform;
	
	//choose the correct scale factor
	switch (size)
	{
		case 500:
			transform.scale(.4, .4);
			break;

		case 1000:
			transform.scale(.2, .2);
			break;
			
		case 2000:
			transform.scale(.1, .1);
			break;
		
		case 4000:
			transform.scale(.05, .05);
			break;
	}
	
	labMiniature->setTransform(transform);
}

/**
 * [PRIVATE-SLOT]
 * Change the zoom
 */
void MainWindow::zoomLabView(int value)
{
	QTransform transform;
	transform.scale(value / 100.0, value / 100.0);
	graphicsView->setTransform(transform);
	zoomLabel->setText(QString::number(value) + "%");
}

/**
 * [PRIVATE-SLOT]
 * Zoom plus the scene
 * (step 20)
 */
void MainWindow::zoomPlus()
{
	zoomSlider->setValue(zoomSlider->value() + 20);	
}

/**
 * [PRIVATE-SLOT]
 * Zoom minus the scene
 * (step 20)
 */
void MainWindow::zoomMinus()
{
	zoomSlider->setValue(zoomSlider->value() - 20);	
}

/**
 * [PRIVATE-SLOT]
 * Zoom 100% the scene
 */
void MainWindow::zoomNormal()
{
	zoomSlider->setValue(100);	
}

/**
 * Clear the content inside the property dock, and reset to 0 the rows count
 */
void MainWindow::clearPropertyDock(TreeModel *newModel)
{	
	//Destroy tree model
	QAbstractItemModel *model = propertiesTreeView->model();
	if(!newModel)
	{
		QStringList header;
		header << tr("Property") << tr("Value");
		TreeModel *model = new TreeModel(header);
		propertiesTreeView->setModel(model);
	}
	else
		propertiesTreeView->setModel(newModel);
	
	//Expand and resize columns
	propertiesTreeView->expandAll();
	for(int column=0; column < propertiesTreeView->model()->columnCount(); ++column)
		propertiesTreeView->resizeColumnToContents(column);
	
	//selection model has changed.. reconnect signal
	connect(propertiesTreeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
			this, SLOT(handleSelection(const QModelIndex&, const QModelIndex&)));
		
	//clear description property label
	descriptionLabel->setText("");
	
	//Destroy old model
	if(model)
		delete model;
}

/**
 * [PRIVATE-SLOT]
 * Save a modified lab (allready saved as)
 */
void MainWindow::saveModifiedLab()
{
	if(labHandler->isCurrentLab() && !labHandler->getLabState())
	{
		//Save as...
		showSaveFileDialog();
		return;
	}
	
	if(!labHandler->getLabChangedState())
	{
		qDebug() << "Lab null or not need to be saved";
		return;
	}
	else
	{
		labHandler->saveLab();
	}
}

/**
 * Change the name of a node inside the tree with the new name
 * Match criteria: MatchExactly
 */
void MainWindow::changeTreeNodeName(QString oldName, QString newName)
{
	QTreeWidgetItem * nodeVm;

	//Find the node as child of root element
	QTreeWidgetItem *root = labTree->topLevelItem(0);		
	if(root == NULL)
	{
		qWarning() << "MainWindow::changeTreeNodeName lab tree top element not found!";
		return;
	}
	
	//get the hostnode
	for(int i=0; i<root->childCount(); i++)
	{
		if(root->child(i)->data(0, Qt::DisplayRole).toString() == oldName)
			nodeVm = root->child(i);
	}
	
	nodeVm->setData(0, Qt::DisplayRole, newName);
	
	updatePathDeph(oldName, newName, nodeVm);	//start a recursive update path of all childs	
}

/**
 * [PRIVATE]
 * Recursive visit of a node to change/update the path of config files
 */
void MainWindow::updatePathDeph(QString oldName, QString newName, QTreeWidgetItem *startNode)
{
	//this is a leaf (update path and exit)
	if(startNode->data(0, Qt::UserRole).toString() == "config_file")
	{
		//get the old path, and change it
		QString tmp = startNode->data(0, Qt::UserRole + 1).toString();
		tmp.replace(oldName, newName);
		
		//save the updated path
		startNode->setData(0, Qt::UserRole + 1, tmp);
		
		return;
	}
	
	// visit all children for the passed node
	for(int i=0; i<startNode->childCount(); i++) 
	{		
		updatePathDeph(oldName, newName, startNode->child(i));	//recursive call
	}
}

/**
 * [PRIVATE-SLOT]
 * Dump the scene inside an PDF (A4 format) file
 */
void MainWindow::dumpToPDF() 
{	
	QPrinter *printer = new QPrinter(QPrinter::HighResolution);
	printer->setOutputFormat(QPrinter::PdfFormat);
	QString fileName = QFileDialog::getSaveFileName(this, tr("Export to PDF"), "~/untitled.pdf", tr("File (*.pdf, *.ps)"));
	QApplication::setOverrideCursor(Qt::WaitCursor);
	printer->setOutputFileName(fileName);
	QPainter *pdfPainter = new QPainter(printer);
	graphicsView->scene()->render(pdfPainter);
	pdfPainter->end();
	
	delete pdfPainter;
	delete printer;
	
	QApplication::restoreOverrideCursor();
}

/**
 * [PRIVATE-SLOT]
 * Dump the scene inside an SVG file
 */
void MainWindow::dumpToSVG()
{	
	QSvgGenerator *gen = new QSvgGenerator();
	QString fileName = QFileDialog::getSaveFileName(this, tr("Export to SVG"), "~/untitled.svg", tr("File (*.svg)"));
	QApplication::setOverrideCursor(Qt::WaitCursor);
	gen->setFileName(fileName);
	gen->setResolution(10);
	QPainter *svgPainter = new QPainter(gen);
	svgPainter->setRenderHints(QPainter::Antialiasing);
	graphicsView->scene()->render(svgPainter);
	svgPainter->end();
	
	delete svgPainter;
	delete gen;
	
	QApplication::restoreOverrideCursor();
}


/**
 * [PRIVATE-SLOT]
 * Dump the scene inside a PNG file
 */
void MainWindow::dumpToPNG()
{	
	QString fileName = QFileDialog::getSaveFileName(this, tr("Export to PNG"), "~/untitled.png", tr("File (*.png)"));
	QApplication::setOverrideCursor(Qt::WaitCursor);

	QPixmap *pngImage =
		new QPixmap(graphicsView->scene()->sceneRect().width(), graphicsView->scene()->sceneRect().height());
	
	pngImage->fill(Qt::white);
	
	QPainter *painter = new QPainter(pngImage);
	painter->setRenderHints(QPainter::Antialiasing);
	graphicsView->scene()->render(painter);
	painter->end();
	
	pngImage->save(fileName, "PNG", PNG_COPRESSION);
	
	delete painter;
	delete pngImage;
	
	QApplication::restoreOverrideCursor();
}

/**
 * [PRIVATE-SLOT]
 * Show the dialog open lab
 */
void MainWindow::openLab()
{
	if(labHandler->isCurrentLab())
	{
		QMessageBox::warning(this, "Visual Netkit - warning",
				tr("You have to close the active lab before opening a new one!"),
				QMessageBox::Yes);
		
		return;
	}
	
	bool abort = false;
	labHandler->confirmCloseLab(&abort);
	
	// abort operation because the user have choosed "cancel"
	// (or "no" when asked him "do you want close etc...) 
	if(abort)
		return;
	
	labHandler->closeLabForced();
	
	olf.resetGui();
	olf.show();
}

/**
 * [PRIVATE-SLOT]
 * Show the dialog open lab (called by history action)
 */
void MainWindow::openLabFromHistory(QString path)
{
	if(labHandler->isCurrentLab())
	{
		QMessageBox::warning(this, "Visual Netkit - warning",
				tr("You have to close the active lab before opening a new one!"),
				QMessageBox::Yes);
		
		return;
	}
	
	bool abort = false;
	labHandler->confirmCloseLab(&abort);
	
	// abort operation because the user have choosed "cancel"
	// (or "no" when asked him "do you want close etc...) 
	if(abort)
		return;
	
	labHandler->closeLabForced();
	
	olf.labPathLineEdit->setText(path);
	olf.show();
}

/**
 * [PRIVATE-SLOT]
 * Set/Unset the window fullscreen mode
 */
void MainWindow::fullscreenMode()
{
	if(actionFullScreenMode->isChecked())
		showFullScreen();
	else
		showNormal();
}

/**
 * [PROTECTED]
 * Close application event
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
	qDebug() << "Closing VisualNetkit...";
	
	/* save the window settings */
	SettingsHandler::getInstance()->saveWindowSettings(saveState(), saveGeometry());
	
	if(!labHandler->isCurrentLab())
	{
		event->accept();	//close the application
		QApplication::exit(0);
	}
	
	bool abort = false;
	labHandler->confirmCloseLab(&abort);
	
	if(abort)
		event->ignore();
	else
		labHandler->closeLabForced();
	
	QApplication::exit(0);
}

/**
 * [PRIVATE]
 * Restore the window state and geometry
 */
void MainWindow::restoreWindow()
{
	/* restore the window geometry */
	QByteArray geometry = SettingsHandler::getInstance()->restoreWindowGeometry();
	if(!geometry.isEmpty())
	{
		restoreGeometry(geometry);
	}

	/* restore the window state */
	QByteArray state = SettingsHandler::getInstance()->restoreWindowState();	
	if(!state.isEmpty())
	{
		restoreState(state);
	}
}

/**
 * [SLOT]
 * Build the lab history menu
 */
void MainWindow::buildLabHistory(const QStringList &history)
{
	/* Restore menu */
	menuHistory->clear();
	
	//rebuild menu
	foreach(QString path, history)
		menuHistory->addAction(QIcon(":/small/folder_vm"), path)->setData(path);
	
	/* add separator and clear menu' action */
	menuHistory->addSeparator();
	menuHistory->addAction(QIcon(":/menu/trash"), tr("Clear Menu"))->setData("clear");
}

/**
 * [PRIVATE-SLOT]
 * Rebuild the lab history
 */
void MainWindow::rebuildHistory()
{
	buildLabHistory(SettingsHandler::getInstance()->getLabHistory());
}

/**
 * [PRIVATE-SLOT]
 * handle the history trigger signal
 */
void MainWindow::handleHistoryAction(QAction* action)
{
	/* Clear menu */
	if(action->data().toString() == "clear")
	{
		SettingsHandler::getInstance()->clearLabHistory();
		qDebug() << "Cleared history.";
	}
	else
	{
		openLabFromHistory(action->data().toString());
	}
}

/**
 * [PRIVATE-SLOT]
 * Show the About dialog
 */
void MainWindow::showAbout()
{
	if(aboutDialog == NULL)
		aboutDialog = new AboutDialog();
	
	aboutDialog->show();
}

/**
 * [PRIVATE-SLOT]
 * Build/active/deactive the menu action based on node selected and its childs
 */
void MainWindow::handleSelection(const QModelIndex &current, const QModelIndex &previous)
{
	Q_UNUSED(previous);
	
	TreeItem *item = static_cast<TreeItem*>(current.internalPointer());

	//invalid item
	if(!item)
	{
		actionsToolButton->setEnabled(false);
		return;
	}
	
	if(item->isElementProperty())
	{
		actionsToolButton->setEnabled(false);
		//Change node description
		descriptionLabel->setText(item->getDescrioption());
		return;
	}
	
	//Change node description
	descriptionLabel->setText(item->getDescrioption());
	
	//Enable if item is a property (plugin property)
	actionsToolButton->setEnabled(item->isProperty());
	
	actionMenu->clear();
	
	if(!item->isPluginHeaderNode())
	{
		QAction *delAction = new QAction(QIcon(":/menu/list_remove"), tr("Remove selected property"), actionMenu);
		QStringList actionData; 
		actionData << "delete" << item->getId();
		delAction->setData(actionData);
		actionMenu->addAction(delAction);
		actionMenu->addSeparator();
	}
	
	foreach(ChildDescription cd, item->getChildDescription())
	{
		QAction *action = new QAction(QIcon(":/menu/list_add"), tr("Add") + " property: " + cd.name, actionMenu);
		QStringList actionData;
		actionData << "add" << item->getId() << cd.id; 
		
		action->setData(actionData);
		actionMenu->addAction(action);
	}
}

/**
 * [PRIVATE-SLOT]
 * Handle a property action
 */
void MainWindow::handlePropertyAction(QAction * action)
{
	if(!action)
		return;
	
	/* Get infos from action */
	QStringList actionData = action->data().toStringList();
	
	//PLUGIN_NAME{SEPARATOR}PROPERTY_UID
	QStringList splittedId = actionData[1].split(SEPARATOR);

	QModelIndex current = propertiesTreeView->currentIndex();
	QAbstractItemModel *model = propertiesTreeView->model();
	TreeItem *currentItem = static_cast<TreeItem*>(current.internalPointer());
	AbstractPropertyHandler *handler = currentItem->getCurrentPropertyHandler();
	
	if(!currentItem || !handler)
		return;
	
	//ACTION: delete property
	if(actionData[0] == "delete")
	{
		QString response = handler->removePluginProperty(splittedId[0], splittedId[1]);
		if(response.isEmpty())
		{
			//remove the tree item from tree view
			bool esite = model->removeRows(current.row(), 1, current.parent());
			actionsToolButton->setDisabled(esite);
			
			if(!esite)
				qWarning() << "WARNING: property view unconsistence; deleted the plugin property, but not the view item";
		}
		else
		{
			//Show a warning
			QMessageBox::warning(this, "Warning - VisualNetkit", response, QMessageBox::Ok);
		}
	}
	
	//ACTION: add property
	if(actionData[0] == "add")
	{
		QPair<PluginProperty*, QString> response = 
			handler->addPluginProperty(splittedId[0], actionData[2], splittedId[1]);
		
		if(response.second.isEmpty())
		{
			//add a new tree item
			QModelIndex index = propertiesTreeView->selectionModel()->currentIndex();
			TreeModel *model = static_cast<TreeModel*>(propertiesTreeView->model());

			if(model->insertRows(0, 1, index))
			{
				PluginProperty *p = response.first;
				
				//Set columns value
				QModelIndex child0 = model->index(0, 0, index);
				QModelIndex child1 = model->index(0, 1, index);
				
				//This is an hack to prevent a stupid Qt? bug.
				model->setData(child0, QVariant(p->getName()), Qt::EditRole);
				model->setData(child1, QVariant(p->getDefaultValue()), Qt::EditRole);
				
				if(model->data(child0, Qt::EditRole).toString().isNull())
				{
					qDebug() << "OMFG OBSCURE BUG!!!";
				}
				
				//Expand and resize view
				propertiesTreeView->expandAll();
				for(int column=0; column < propertiesTreeView->model()->columnCount(); ++column)
					propertiesTreeView->resizeColumnToContents(column);
				
				//Init the low level model (tree item of child0/1)
				TreeItem *ti = model->getItem(child0);
				ti->setProperty(true);
				
				//PLUGIN_NAME{SEPARATOR}PROPERTY_ID{SEPARATOR}PROPERTY_COPY
				ti->setId(splittedId[0] + SEPARATOR + p->getUniqueId());
				
				PluginProxy *proxy = handler->getPluginFromCurrentElement(splittedId[0]);
				if(!proxy)
				{
					qWarning() << "WARNING: [NULL PROXY] property view unconsistence; added the plugin property, but not the view item";
					return;
				}
				
				ti->appendChildsDescription(proxy->getPropertyExpert()->getChildsByParentId(p->getId()));
				
				ti->setPropertyHandler(handler);
				
				ti->setDescription(p->getDescription());
				
				buildViewChildsDeeply(child0, p, proxy, handler); //Recursive build
			}
			else
				qWarning() << "Fail creating view row.";
		}
		else
		{
			//Show a warning
			QMessageBox::warning(this, "Warning - VisualNetkit", response.second, QMessageBox::Ok);
		}
	}
}

/**
 * [PRIVATE]
 * Build view deeply starting from property childs
 */
void MainWindow::buildViewChildsDeeply(
		QModelIndex &child, PluginProperty *prop, PluginProxy *proxy, AbstractPropertyHandler *handler)
{
	TreeModel *model = static_cast<TreeModel*>(propertiesTreeView->model());
	
	foreach(PluginProperty *p, prop->getChilds())
	{
		/* Add a sub-child */
		if(model->insertRows(0, 1, child))
		{		
			//Set columns value
			QModelIndex child0 = model->index(0, 0, child);
			QModelIndex child1 = model->index(0, 1, child);
			
			//This is an hack to prevent a stupid Qt? bug.
			model->setData(child0, QVariant(p->getName()), Qt::EditRole);
			model->setData(child1, QVariant(p->getDefaultValue()), Qt::EditRole);
			
			if(model->data(child0, Qt::EditRole).toString().isNull())
				qDebug() << "--> OMFG OBSCURE BUG!!!";
			
			//Expand and resize view
			propertiesTreeView->expandAll();
			for(int column=0; column < propertiesTreeView->model()->columnCount(); ++column)
				propertiesTreeView->resizeColumnToContents(column);
		
			
			//Init the low level model (tree item of child0/1)
			TreeItem *ti = model->getItem(child0);
			ti->setProperty(true);
			
			//PLUGIN_NAME{SEPARATOR}PROPERTY_UID
			ti->setId(proxy->getPlugin()->getName() + SEPARATOR + p->getUniqueId());
			
			ti->appendChildsDescription(proxy->getPropertyExpert()->getChildsByParentId(p->getId()));
			
			ti->setPropertyHandler(handler);
			
			ti->setDescription(p->getDescription());
			
			buildViewChildsDeeply(child0, p, proxy, handler);
		}
		else
			qWarning() << "Fail creating view row.";
	}
}


