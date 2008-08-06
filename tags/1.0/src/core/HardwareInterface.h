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

#ifndef HARDWAREINTERFACE_H_
#define HARDWAREINTERFACE_H_

#include <QObject>
#include <QString>
#include <QDebug>
#include "CollisionDomain.h"

class VirtualMachine;

/**
 * HardwareInteface (like ethernet, wireless, ect...)
 */
class HardwareInterface : public QObject
{

private:
	QString name;
	VirtualMachine *myMachine;
	CollisionDomain *domain;
	bool state;
	
public:
	HardwareInterface(VirtualMachine *m, CollisionDomain *cd,
			QString ethName, bool interfaceState);
	virtual ~HardwareInterface();
	QString getName() { return name; };
	void setName(QString newName) { name = newName; };
	void setState(bool newState) { state = newState; };	
	VirtualMachine* getMyVirtualMachine() { return myMachine; };
	CollisionDomain* getMyCollisionDomain() { return domain; };
	bool getState() { return state; };
	void setMyCollisionDomain(CollisionDomain *cd);

};

#endif /*HARDWAREINTERFACE_H_*/
