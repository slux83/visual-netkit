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

#ifndef COLLISIONDOMAIN_H_
#define COLLISIONDOMAIN_H_

#include <QNetworkAddressEntry>
#include <QMap>
#include <QString>

#include "../common/NetworkAddress.h"

class HardwareInterface;

class CollisionDomain
{

private:
	QString name;
	NetworkAddress *address;
	QMap<QString, HardwareInterface *> peers;

public:
	CollisionDomain(QString newName, NetworkAddress *subnet);
	virtual ~CollisionDomain();
	
	/* Setter and getter functions */
	QString getName() { return name; };
	NetworkAddress* getSubnet() { return address; };
	void addPeer(HardwareInterface* hi);
};

#endif /*COLLISIONDOMAIN_H_*/
