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

#include "VirtualMachine.h"

/**
 * [VirtualMachine]
 * Base contructor
 */
VirtualMachine::VirtualMachine(QString newName)
{
	name = newName;
	dm = new DaemonManager();
}

/**
 * Deconstructor
 */
VirtualMachine::~VirtualMachine()
{
	
}

/**
 * Get the machine name 
 */
QString VirtualMachine::getName()
{
	return name;
}

/**
 * Get the list of interfaces owned by this virtual machine
 */
QLinkedList<HardwareInterface *> VirtualMachine::getInterfaces()
{
	return interfaces;
}

/**
 * Get my vm type
 */
VmType VirtualMachine::getMyType()
{
	return dm->getVmType();
}
