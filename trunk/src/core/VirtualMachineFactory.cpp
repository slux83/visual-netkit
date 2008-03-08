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

#include "VirtualMachineFactory.h"

/**
 * Init the null instance for the singletone abstract factory
 */
VirtualMachineFactory* VirtualMachineFactory::instance = NULL;

/**
 * Constructor
 */
VirtualMachineFactory::VirtualMachineFactory()
{
}

/**
 * Deconstructor
 */
VirtualMachineFactory::~VirtualMachineFactory()
{
}

/**
 * Get the correct factory
 */
VirtualMachineFactory * VirtualMachineFactory::getInstance(VirtualMachineType type)
{
	Q_UNUSED(type)
	
	//TODO: use a stantment like this when you have some extension of VirtualMachine
/*	switch (type) 
	{
		case VirtualMachine::Router :
			return RouterFactory::getInstance();
			break;
		case VirtualMachine::Host :
			return instance;
			break;
	}
*/	
	return instance;

}

VirtualMachine * VirtualMachineFactory::getNewVirtualMachine(QString name)
{
	return new VirtualMachine(name);
}



