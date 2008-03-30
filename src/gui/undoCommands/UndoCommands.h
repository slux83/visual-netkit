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

#ifndef UNDOCOMMANDS_H_
#define UNDOCOMMANDS_H_

#include <QUndoCommand>
#include <QObject>
#include "../VirtualMachineItem.h"
#include "../../core/VirtualMachine.h"
#include "../handles/VmMapper.h"

/**
 * [COMMAND]
 * Add a virtual machine command.
 */
class AddVmCommand : public QObject, public QUndoCommand
{
	
private:
	VirtualMachineItem *vmItem;
	VirtualMachine *vm;
	
public:
	AddVmCommand(VirtualMachineItem *newVmItem, VirtualMachine *newVm,
			QUndoCommand *parent = 0);
	virtual ~AddVmCommand();
	
	//TODO
	void undo();
	void redo();
};

#endif /*UNDOCOMMANDS_H_*/
