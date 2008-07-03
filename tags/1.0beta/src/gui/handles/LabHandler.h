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
#include <QStringList>
#include <QTreeWidgetItem>
#include "LabPropertyController.h"
#include "SceneTreeMapper.h"
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
	
	void addPathToNode(QStringList path, QTreeWidgetItem *node, QString fullPath);
	QMap<QString, QTreeWidgetItem*> findItems(QString nodeName, QTreeWidget *node);
	
public:
	LabHandler();
	virtual ~LabHandler();
	static LabHandler * getInstance();
	void setMainWindow(MainWindow *w);
	UndoStack *getUndoStack() { return undoStack; };
	MainWindow* getMainWindow() { return mainWindow; };
	bool getLabState();
	bool getLabChangedState();
	void prepareRenderLabProperties();
	void setSceneSize(QRectF &size);
	void setSaveLabState(bool state);
	void setChangedLabState(bool state = true);
	bool isCurrentLab();
	bool confirmCloseLab(bool *abort = NULL);
	void closeLabForced();
	void setMainWindowTitle();
	void removePathFromTree(QString &path, bool removeVm = false);
	void removeItemFromScene(QGraphicsItem *item);
	
public slots:
	void closeLab();
	void newLab();
	void openLab(QString labPath);
	void saveLabAs(const QStringList &selectedFiles);
	void saveLab();
	void addCreatedLabOnTree(Laboratory *newLab);
	void addCreatedVmOnTree(VirtualMachine *m);
	void labTreeItemDoubleClicked(QTreeWidgetItem * item, int column);
	void saveChangedProperty(int row, int column);
	void addPathToTree(QString path);

signals:
	void logEvent(QString message);
};

#endif /*NEWLABHANDLER_H_*/
