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

#ifndef VMCONTROLLER_H_
#define VMCONTROLLER_H_

#include <QLinkedList>
#include <QPair>

#include "VirtualMachineItem.h"
#include "../core/VirtualMachine.h"

/**
 * This class is the singleton controller for the virtual machines.
 * It's also map the graphical (view) item with the domain object (model)
 */
class VmController
{
	
private:
	static VmController *instance;
	
	/* the pair: <VIEW, DOMAIN> */
	QLinkedList< QPair<VirtualMachineItem*, VirtualMachine*> > mappings;
	
public:
	VmController();
	virtual ~VmController();
	static VmController* getInstance();
	void addVirtualMachine();
};

#endif /*VMCONTROLLER_H_*/
