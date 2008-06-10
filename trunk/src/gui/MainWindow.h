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

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include "ui_main.h"
#include "OpenLabForm.h"

class VmHandler;
class LabHandler;

#include <QWidget>
#include <QTreeWidget>
#include <QFileDialog>

/**
 * The main window class
 */
class MainWindow : public QMainWindow, public Ui::NetkitMainWindow
{
	Q_OBJECT
	
public:
	MainWindow(QWidget *parent = 0);
	virtual ~MainWindow();
	void unlockSceneAndActions();
	void lockSceneAndActions();
	QGraphicsView* getGraphicsView() { return graphicsView; };
	void clearPropertyDock();
	void changeTreeNodeName(QString oldName, QString newName, bool rootElement = false);
	
private:
	/* Controllers */
	LabHandler *labHandler;
	VmHandler *vmHandler;
	
	/* Open lab form */
	OpenLabForm olf;
	
	/* Action groups */
	QActionGroup *labItemGroup, *sceneSizeGroup, *graphZoomGroup;
	
	/* The file chooser to select where save a lab */
	QFileDialog *saveFileDialog;
	
	void populateViewMenu();
	void createConnections();
	void createActionGroups();
	void createScene();
	void initMiniatureDock();
	void updateMinuatureDock(int size);
	
private slots:
	void zoomLabView(int value);
	void zoomPlus();
	void zoomMinus();
	void zoomNormal();
	void showSaveFileDialog() { saveFileDialog->show(); };
	void saveModifiedLab();
	void dumpToPDF();
	void dumpToSVG();
	void dumpToPNG();
	void openLab();

public slots:
	void writeLogMessage(QString message);
	void resizeScene(QAction *action);
	void forceManageGraphAction() { actionManageGraph->setChecked(true); };

};

#endif /*MAINWINDOW_H_*/
