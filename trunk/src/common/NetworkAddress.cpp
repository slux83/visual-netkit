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

#include "NetworkAddress.h"

/**
 * Empty constructor
 */
NetworkAddress::NetworkAddress() : QNetworkAddressEntry()
{
	fillNetmaskMap();
}

/**
 * Constructor with ip + netmask
 */
NetworkAddress::NetworkAddress(QHostAddress ip, QHostAddress netmask) 
	: QNetworkAddressEntry()
{
	setIp(ip);
	setNetmask(netmask);
	fillNetmaskMap();
}

/**
 * Constructor with ip + class
 */
NetworkAddress::NetworkAddress(QHostAddress ip, quint8 netmaskCidr)
	: QNetworkAddressEntry()
{
	setIp(ip);
	fillNetmaskMap();
	
	setNetmask(netmaskMapping.value(netmaskCidr));
}

/**
 * Deconstructor
 */
NetworkAddress::~NetworkAddress()
{
}

/**
 * [PRIVATE]
 * Convert che cidr notation in common ip netmask.
 * 		=> EXAMPLE: 10.0.0.0/8 = 10.0.0.0/255.0.0.0
 */
QHostAddress NetworkAddress::cidr2netmask(quint8 cidrMask)
{
	QHostAddress base(QHostAddress::Broadcast);
	QHostAddress conv((base.toIPv4Address() << (32 - cidrMask)));
	
	return conv;
}

/**
 * [PRIVATE]
 * fill the netmask table
 */
void NetworkAddress::fillNetmaskMap()
{
	/* fill the table */
	for(int i=1; i<33; i++)
		netmaskMapping.insert(i, cidr2netmask(i));
}

/**
 * Get the string that represent this network address
 * Default:		"10.0.0.1/8" 
 * False arg:	"10.0.0.1/255.0.0.0"
 */
QString NetworkAddress::toString(bool cidr)
{
	if(cidr)	//10.0.0.1/8
		return QString(ip().toString() + "/" + QString::number(netmaskMapping.key(netmask())));
	else		//10.0.0.1/255.0.0.0
		return QString(ip().toString() + "/" + netmask().toString());
}

/**
 * [STATIC-PUBLIC]
 * Get the network gived an ip and the netmask
 * (ip & netmask)
 */
QHostAddress NetworkAddress::toGeneralNetwork(QHostAddress ip, QHostAddress netmask)
{
	return QHostAddress(ip.toIPv4Address() & netmask.toIPv4Address());
}

/**
 * [STATIC-PUBIC]
 * Validate a netmask
 */
bool NetworkAddress::validateNetmask(QHostAddress netmask)
{
	bool valid = false;
	for(int i=1; i<33; i++)
	{
		if(QHostAddress((QHostAddress(QHostAddress::Broadcast).toIPv4Address() << (32 - i))) == netmask)
		{
			valid = true;
			break;
		}
	}
	
	return valid;
}

/**
 * [STATIC-PUBLIC]
 * Validate an ip address (ipv4)
 */
bool NetworkAddress::validateIp(QString ip)
{
	//Regular expression
	QRegExp ipRegExp("(?:1\\d?\\d?|2(?:[0-4]\\d?|[6789]|5[0-5]?)?|[3-9]\\d?|0)(?:\\.(?:1\\d?\\d?|2(?:[0-4]\\d?|[6789]|5[0-5]?)?|[3-9]\\d?|0)){3}", 
					Qt::CaseSensitive, QRegExp::RegExp2);
	
	return (ipRegExp.exactMatch(ip));
}

/**
 * [PUBLIC]
 * Check a netmask if is valid
 */
bool NetworkAddress::isValidNetmask()
{
	return (netmaskMapping.key(netmask()) != 0);
}

/**
 * [PUBLIC]
 * Set the netmask gived the CIDR notation
 * (ussuming a correct cidr value)
 */
void NetworkAddress::setCidrNetmask(quint8 cidrNetmask)
{
	setNetmask(cidr2netmask(cidrNetmask));
}

/**
 * [STATIC-PUBLIC]
 * generate a valid broadcast address by ip and netmask
 * 		note: assuming that ip and netmask are valid!
 */
QHostAddress NetworkAddress::generateBroadcast(QHostAddress ip, QHostAddress netmask)
{
	QHostAddress full("255.255.255.255");
	
	//get network
	QHostAddress network(ip.toIPv4Address() & netmask.toIPv4Address());
	
	QHostAddress broadcast(network.toIPv4Address() | (~netmask.toIPv4Address() & full.toIPv4Address()));
	
	return broadcast;
}

