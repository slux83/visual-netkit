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

#include "DaemonManager.h"

/**
 * Constructor
 */
DaemonManager::DaemonManager()
{
	/* Fill the list of daemons with te enumeration fields
	 * 
	 * TODO: put the daemon list inside an .ini file that the user
	 *       (maybe only developers) can edit easily
	 */
	daemonList.append(new QPair<Daemon, bool>(Zebra, false));
	daemonList.append(new QPair<Daemon, bool>(Ripd, false));
	daemonList.append(new QPair<Daemon, bool>(Bgpd, false));
	daemonList.append(new QPair<Daemon, bool>(Ospfd, false));
	daemonList.append(new QPair<Daemon, bool>(Ospf6d, false));
	daemonList.append(new QPair<Daemon, bool>(Ripngd, false));
	//daemonList.append(new QPair<Daemon, bool>(Bind, false));
}

/**
 * Deconstructor
 */
DaemonManager::~DaemonManager()
{
	/* TODO: delete all qpair pointers */
}

/**
 * Set the new state for a specific daemon
 */ 
void DaemonManager::setDaemonState(Daemon daemon, bool newState)
{
	QLinkedListIterator< QPair<Daemon, bool> * > i(daemonList);
	while (i.hasNext())
	{
		 QPair<Daemon, bool> *d = i.next();
		if(d->first == daemon)
			d->second = newState;
	}
}

/**
 * Get the correct virtual machine type depending on the active daemons
 */
VmType DaemonManager::getVmType()
{
	VmType returnVal = Host;
	bool foundRouter = false;
	QList<Daemon> activeDaemons = daemons2activeList();
	QListIterator<Daemon> it(activeDaemons);
	
	while(it.hasNext())
	{
		Daemon d = it.next();
		if(d >= Zebra && d <= Ripngd)
			foundRouter = true;
	}
	
	if(foundRouter)
		returnVal = Router;
	
	return returnVal;
}

/**
 * [PRIVATE]
 * Map the active daemons inside a list
 */
QList<Daemon> DaemonManager::daemons2activeList()
{
	QList<Daemon> activeDaemons;
	QLinkedListIterator< QPair<Daemon, bool> * > i(daemonList);
	while (i.hasNext())
	{
		QPair<Daemon, bool> *daemon = i.next();
		/* if the daemon is enabled */
		if(daemon->second)
		{
			activeDaemons.append(daemon->first);
		}
	}
	
	return activeDaemons;
}
