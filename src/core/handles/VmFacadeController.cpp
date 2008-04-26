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

#include "VmFacadeController.h"

/**
 * Init the null instance for the singletone controller
 */
VmFacadeController* VmFacadeController::instance = NULL;

/**
 * Constructor
 */
VmFacadeController::VmFacadeController()
{
	labFacadeController = LabFacadeController::getInstance();
}

/**
 * Deconstructor
 */
VmFacadeController::~VmFacadeController()
{
}


/**
 * Singletone get instance
 */
VmFacadeController* VmFacadeController::getInstance()
{
	if (instance == NULL)
	{
		instance = new VmFacadeController();
	}

	return instance;
}

/**
 * Create a new instance of VirtualMachine [by-creator]
 */
VirtualMachine* VmFacadeController::createNewVirtualMachine(QString name,
		QList<Daemon> activeDaemons)
{
	VirtualMachine *m = new VirtualMachine(name);
	QListIterator<Daemon> i(activeDaemons);
	
	//Active daemons in list
	while(i.hasNext())
	{
		/* Set the daemon as active */
		m->getDm()->setDaemonState(i.next(), true);
	}
	
	return m;
}

/**
 * Create and return a new hardware interface connected to `cd` collision domain
 */
HardwareInterface* VmFacadeController::createNewHardwareIterface(VirtualMachine *vm, QString ethName,
			bool state, NetworkAddress address, CollisionDomain *cd)
{

}
