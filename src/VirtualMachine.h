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

#ifndef VIRTUALMACHINE_H_
#define VIRTUALMACHINE_H_

/**
 * [PURE ABSTRACT CLASS]
 * The general Virtual machine that inside NetKit it is modelled with
 * User Mode Linux
 */
class VirtualMachine
{
	
public:
	virtual ~VirtualMachine() {};
	virtual QString getName() = 0;
	virtual QString getType() = 0;
	
};

#endif /*VIRTUALMACHINE_H_*/
