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

#include "SuperGroup.h"

/**
 * [SuperGroup]
 * The base class implementation
 */
SuperGroup::SuperGroup()
{
	type = "unknown";
}

/**
 * Deconstructor
 */
SuperGroup::~SuperGroup()
{

}

/**
 * Get the subnets list
 */
QLinkedList<Subnet *> SuperGroup::getSubnets()
{
	return subnets;
}

/**
 * Get the type of this class
 */
QString SuperGroup::getType()
{
	return type;
}


/******************************************************************************/


/**
 * [Lan]
 * The Lan class implementation
 */
Lan::Lan() : SuperGroup()
{
	type = "lan";
}

/**
 * Deconstructor
 */
Lan::~Lan()
{

}

/**
 * [REIMPL]
 * Get the type of this class
 */
QString Lan::getType()
{
	return type;
}