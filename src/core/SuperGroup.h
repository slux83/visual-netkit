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

#ifndef SUPERGROUP_H_
#define SUPERGROUP_H_

#include <QMap>
#include <QString>
#include <QSet>
#include "../common/NetworkAddress.h"
#include "../common/Types.h"
#include "CollisionDomain.h"

/**
 * [ABSTRACT CLASS]
 * Super Group interface (implementations: AS, Area, etc...)
 */
class SuperGroup
{
protected:
	/**
	 * The super group subnets (if any)
	 * This value is calculated trought the collision domains owned by this 
	 * super group or by a matching between config files and internal collision
	 * domains
	 */
	QSet<NetworkAddress *> subnets;
	QMap<QString, CollisionDomain *> domains;
	
	/**
	 * The base class is a generic group (maybe it contains some uncategorized
	 * collision domains like peering collision between two or more AS
	 */
	SuperGroupType type;
	
public:
	SuperGroup(SuperGroupType groupType = Uncategorized);
	virtual ~SuperGroup();
	
	/**
	 * Getter functions
	 */
	QSet<NetworkAddress *> getSubnets() { return subnets; };
	SuperGroupType getType() { return type; };
	
	/**
	 * Setter functions
	 */
	void setType(SuperGroupType newType) { type = newType; };
	
};


/******************************************************************************/


/**
 * [AUTONOMOUS SYSTEM CLASS]
 * An extension of super group that model an AS
 */
class AutonomouSystem : public SuperGroup
{

public:
	AutonomouSystem(SuperGroupType groupType = AutonomousSystemType);
	virtual ~AutonomouSystem();
};


#endif /*SUPERGROUP_H_*/
