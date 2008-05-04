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

#include "Laboratory.h"

#include "../common/CommonConfigs.h"

/**
 * Constructor with some params
 */
Laboratory::Laboratory(QString newName, QString newVersion, QString newAuthors)
{
	name = newName;
	version = newVersion;
	authors = newAuthors;
	isSaved = false;
}

/**
 * Constructor with no params
 */
Laboratory::Laboratory()
{
	name = DEFAULT_LAB_NAME;
	version = "1.0";
	
	/* Set the current date time */
	QDateTime d;
	d.setDate(QDate::currentDate());
	d.setTime(QTime::currentTime());
	
	date = d.toString("dd/MM/yyyy hh:mm");
	
	isSaved = false;
}

/**
 * Deconstructor
 */
Laboratory::~Laboratory()
{

}

/**
 * Add a new machine inside the collection
 */
void Laboratory::addMachine(VirtualMachine *vm)
{
	if(machines.contains(vm->getName()))
	{
		qWarning() << "Warning: Virtual machine" << vm->getName() << "is allready inside the machines map";
		return;
	}
	
	machines.insert(vm->getName(), vm);
}

/**
 * Add a new collision domain inside the collection
 */
void Laboratory::addCollisionDomain(CollisionDomain *cd)
{
	if(collisionDomains.contains(cd->getName()))
	{
		qWarning() << "Warning: Collision domain" << cd->getName() << "is allready inside the collision domain map";
		return;
	}
	
	collisionDomains.insert(cd->getName(), cd);
}
