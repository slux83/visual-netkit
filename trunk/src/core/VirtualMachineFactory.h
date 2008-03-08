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

#ifndef VIRTUALMACHINEFACTORY_H_
#define VIRTUALMACHINEFACTORY_H_

#include "VirtualMachine.h"
#include <QString>

/**
 * Abstract Factory for VirtualMachine
 * [possible extensions are: RouterFactory, etc...]
 */
class VirtualMachineFactory
{
	
private:
	static VirtualMachineFactory *instance;
	
public:
	VirtualMachineFactory();
	virtual ~VirtualMachineFactory();
	static VirtualMachineFactory * getInstance(VirtualMachineType type);
	virtual VirtualMachine * getNewVirtualMachine(QString name);
};

#endif /*VIRTUALMACHINEFACTORY_H_*/
