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

#ifndef TYPES_H_
#define TYPES_H_

/**
 * Enumeration for daemons
 */
enum Daemon
{
	Zebra = 0,	//Zebra must be the first!
	Ripd = 1,
	Bgpd = 2
};

/**
 * Enumeration for virtual machines
 */
enum VmType
{
	Host = 0,
	Router = 1
};

/**
 * Enumeration for Super Groups
 */
enum SuperGroupType
{
	AutonomousSystemType = 0,
	Uncategorized = 1
};

/**
 * Enumeration for graphics item groups
 */
enum ItemType
{
	VmItem = 1,
	CdItem = 2
};

#endif /*TYPES_H_*/
