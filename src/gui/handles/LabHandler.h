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

#ifndef NEWLABHANDLER_H_
#define NEWLABHANDLER_H_

#include <QObject>
#include <QDebug>
#include "LabPropertyController.h"
#include "../../core/handles/LabFacadeController.h"
#include "../../core/Laboratory.h"
#include "../MainWindow.h"
#include "../UndoStack.h"

/**
 * [PRIMARY CONTROLLER]
 * This's the controller for the operations on Laboratory 
 */
class LabHandler : public QObject
{
	Q_OBJECT

private:
	static LabHandler *instance;
	MainWindow *mainWindow;
	UndoStack *undoStack;
	LabPropertyController *propertyController;
	
public:
	LabHandler();
	virtual ~LabHandler();
	static LabHandler * getInstance();
	void setMainWindow(MainWindow *w);
	UndoStack *getUndoStack() { return undoStack; };
	MainWindow* getMainWindow() { return mainWindow; };

public slots:
	void newLab();
	void openLab();
	void saveLab();
	void addCreatedLabOnTree(Laboratory *newLab);
	void addCreatedVmOnTree(VirtualMachine *m);
	void labTreeItemSelected(QTreeWidgetItem * item, int column);
	void labTreeItemDoubleClicked(QTreeWidgetItem * item, int column);
	void saveChangedProperty(int row, int column);

signals:
	void logEvent(QString message);
};

#endif /*NEWLABHANDLER_H_*/
