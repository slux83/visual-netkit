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

#include <QNetworkAddressEntry>
#include <QString>

#include "VirtualMachine.h"
#include "CollisionDomain.h"

/**
 * [ABSTRACT CLASS]
 * HardwareInteface (like ethernet, ect...)
 */
class HardwareInterface
{
protected:
	QNetworkAddressEntry address;
	QString name;
	VirtualMachine *myMachine;
	CollisionDomain *domain;
	static const QString type("unknown");
	
public:
	HardwareInterface(VirtualMachine *m);
	virtual ~HardwareInterface() {};
	virtual QString getType();
	QString getName();
	QNetworkAddressEntry getAddress();
	VirtualMachine* getMyVirtualMachine();
	CollisionDomain* getMyCollisionDomain();
	void setMyCollisionDomain(CollisionDomain *cs);
	//TODO: etc...

};


/******************************************************************************/


class IpInterface : public HardwareInterface
{
private:
	const QString type("ip_interface");
	
public:
	IpInterface();
	virtual ~IpInterface();
	QString getType();
};

#endif /*HARDWAREINTERFACE_H_*/