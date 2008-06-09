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

#ifndef LABOPENER_H_
#define LABOPENER_H_

#include <QObject>
#include <QThread>
#include <QRegExp>
#include <QFile>
#include <QDir>
#include <QStringList>
#include <QDebug>

#include "../core/handles/VmFacadeController.h"

class LabHandler;

/**
 * This's the expert that allow to open a lab created with visual netkit
 * This is not an IMPORTER
 */
class LabOpener : public QObject
{
	Q_OBJECT
	
private:
	QString labPath;
	QString errorString;
	bool validateLab();			//step 0
	bool createLab();			//step 1
	bool fetchMachines();		//step 2
	bool fetchCds();			//step 3
	
signals:
	void loadStepDone(int step, bool result);
	
public:
	LabOpener(QString &labPathToOpen);
	LabOpener() {};
	virtual ~LabOpener();
	void open();
	void setPath(QString &newPath) { labPath = newPath; };
	QString getPath() { return labPath; };
	QString getErrorString() { return errorString; };
};

#endif /*LABOPENER_H_*/
