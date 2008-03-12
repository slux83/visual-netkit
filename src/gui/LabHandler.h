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
#include "../core/Laboratory.h"
#include "MainWindow.h"

/**
 * This's the controller for the operations on Laboratory
 */
class LabHandler : public QObject
{
	Q_OBJECT

/* Property key for lab */
enum PropertyKey
{
	Name = 0,
	Version,
	Date,
	Description,
	Authors,
	Email,
	Website
};

private:
	static LabHandler *instance;
	MainWindow *mainWindow;
	
	/* Private functions */
	void renderLabProperties(Laboratory *l);
	
public:
	LabHandler();
	virtual ~LabHandler();
	static LabHandler * getInstance();
	void setMainWindow(MainWindow *w);

public slots:
	void newLab();
	void showCreatedLab(Laboratory *newLab);
	void labTreeItemSelected(QTreeWidgetItem * item, int column);
	void clearPropertyEditor();
	void saveChangedProperty(int row, int column);
};

#endif /*NEWLABHANDLER_H_*/
