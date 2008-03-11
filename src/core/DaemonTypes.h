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

#ifndef DAEMONTYPES_H_
#define DAEMONTYPES_H_

/**
 * Enumeration for daemons
 */
enum Daemon
{
	Zebra = 0,
	Ripd = 1,
	Bgpd = 2,
	Ospfd = 3,
	Ospf6d = 4,
	Ripngd = 5,
	Bind = 6
};

#endif /*DAEMONTYPES_H_*/
