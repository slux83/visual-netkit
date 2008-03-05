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

#ifndef LAN_H_
#define LAN_H_

#include "SuperGroup.h"

/**
 * An concrete super group that model a Local Area Network
 */
class Lan : public SuperGroup
{
	
private:
	QList<Subnet *> subnets;
	
public:
	Lan();
	QList<Subnet *> getSubnets();	//the list as copy
	virtual ~Lan();
};

#endif /*LAN_H_*/
