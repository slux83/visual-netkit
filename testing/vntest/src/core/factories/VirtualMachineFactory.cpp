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

#include "VirtualMachineFactory.h"

/**
 * Init the null instance for the singletone factory
 */
VirtualMachineFactory* VirtualMachineFactory::instance = NULL;

/**
 * Constructor
 */
VirtualMachineFactory::VirtualMachineFactory()
{
}

/**
 * Deconstructor
 */
VirtualMachineFactory::~VirtualMachineFactory()
{
}

/**
 * Get the correct factory
 */
VirtualMachineFactory * VirtualMachineFactory::getInstance()
{	
	return instance;

}

/**
 * Geta a new instance of VirtualMachine giving the name and the
 * list of active daemons
 */
VirtualMachine * VirtualMachineFactory::getNewVirtualMachine(QString name,
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



