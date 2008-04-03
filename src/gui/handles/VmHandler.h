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

#ifndef VMHANDLER_H_
#define VMHANDLER_H_

#include <QObject>

#include "../../core/handles/LabFacadeController.h"
#include "../../core/handles/VmFacadeController.h"
#include "../../core/VirtualMachine.h"
#include "LabHandler.h"

/**
 * This class is the controller for the vm operations
 */
class VmHandler : public QObject
{
	Q_OBJECT
	
private:
	static VmHandler* instance;
	LabFacadeController *labFacadeController;
	VmFacadeController *vmFacadeController;
	LabHandler *labHandler;
	
public:
	VmHandler();
	virtual ~VmHandler();
	static VmHandler* getInstance();
	bool vmNameExist(QString vmNameToCheck);

signals:
	void logEvent(QString message);

public slots:
	void createVm(QString vmNewName, QList<Daemon> activeDaemons);
};

#endif /*VMHANDLER_H_*/
