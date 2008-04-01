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

#ifndef NETWORKADDRESS_H_
#define NETWORKADDRESS_H_

#include <QNetworkAddressEntry>

/**
 * This class extend QNetworkAddressEntry, and offer some useful functions.
 * For instance the convertion between [IP/CIDR-NETMASK] and [IP/CLASSIC-NETMASK]
 * EXAMPLE: 10.0.0.0/8 = 10.0.0.0/255.0.0.0
 */
class NetworkAddress : public QNetworkAddressEntry
{
private:
	//this map is a mapping between ip-netmask and cidr netmask notation
	QMap<quint8, QHostAddress> netmaskMapping;
	QHostAddress cidr2netmask(quint8 cidrMask);
	void fillNetmaskMap();
	
public:
	NetworkAddress();
	NetworkAddress(QHostAddress ip, QHostAddress netmask);
	NetworkAddress(QHostAddress ip, quint8 netmaskCidr);
	virtual ~NetworkAddress();
	QString toString(bool cidr = true);

};

#endif /*NETWORKADDRESS_H_*/
