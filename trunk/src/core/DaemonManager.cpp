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
#include "../common/CommonConfigs.h"

#include <QDebug>

/**
 * Constructor
 */
DaemonManager::DaemonManager()
{
	/* Fill the daemonList */
	QListIterator<Daemon> i(getDaemons());
	while(i.hasNext())
		daemonList.insert(i.next(), false);
}

/**
 * Deconstructor
 */
DaemonManager::~DaemonManager() { }

/**
 * Set the new state for a specific daemon
 */ 
void DaemonManager::setDaemonState(Daemon daemon, bool newState)
{
	if(!daemonList.contains(daemon))
	{
		qWarning() << "DaemonManager:" << "Unknow daemon" << daemon;
		return;
	}
	
	daemonList[daemon] = newState;
}

/**
 * Get the correct virtual machine type depending on the active daemons
 */
VmType DaemonManager::getVmType()
{
	VmType returnVal = Host;
	bool foundRouter = false;
	QListIterator<Daemon> it(daemons2activeList());
	
	//process active daemons
	while(it.hasNext())
	{
		Daemon d = it.next();
		if(d >= getDaemons().first() && d <= getDaemons().last())
		{
			foundRouter = true;
			break;
		}
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
	QMapIterator<Daemon, bool> i(daemonList);
	while (i.hasNext())
	{
		i.next();
		/* if the daemon is enabled */
		if(i.value())
		{
			activeDaemons.append(i.key());
		}
	}
	
	return activeDaemons;
}
