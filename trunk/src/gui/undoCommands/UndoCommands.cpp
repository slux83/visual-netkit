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

#include "UndoCommands.h"

/**
 * Contructor
 */
AddVmCommand::AddVmCommand(VirtualMachineItem *newVmItem, VirtualMachine *newVm,
		QList<PluginProxy *> pList, QUndoCommand *parent) : QUndoCommand(parent)
{
	vmItem = newVmItem;
	vm = newVm;
	plugins = pList;
	setText(tr("Added a new virtual machine: ") + vm->getName());
}

/**
 * Deconstructor
 */ 
AddVmCommand::~AddVmCommand()
{
}

/**
 * Redo action
 */
void AddVmCommand::redo()
{
	//show and connect view & domain machines
	VmMapper::getInstance()->addNewMapping(vmItem, vm);
	
	//call default label for plugins
	QListIterator<PluginProxy *> i(plugins);
	while(i.hasNext())
		i.next()->showDefaultGrophicsLabel();
}

/**
 * Undo action
 */
void AddVmCommand::undo()
{
	//TODO
}

/******************************************************************************/

/**
 * Contructor
 */
AddCdCommand::AddCdCommand(CollisionDomainItem *newCdItem, CollisionDomain *newCd,
		QList<PluginProxy *> pList, QUndoCommand *parent) : QUndoCommand(parent)
{
	cdItem = newCdItem;
	cd = newCd;
	plugins = pList;
	setText(tr("Added a new collision domain: ") + cd->getName());
}

/**
 * Deconstructor
 */ 
AddCdCommand::~AddCdCommand()
{
}

/**
 * Redo action
 */
void AddCdCommand::redo()
{
	//show and connect view & domain objects
	CdMapper::getInstance()->addNewMapping(cdItem, cd);
	
	//call default label for plugins
	QListIterator<PluginProxy *> i(plugins);
	while(i.hasNext())
		i.next()->showDefaultGrophicsLabel();
}

/**
 * Undo action
 */
void AddCdCommand::undo()
{
	//TODO
}

/******************************************************************************/

/**
 * Contructor
 */
AddLinkCommand::AddLinkCommand(LinkItem *newLinkItem, HardwareInterface *newHi,
		QList<PluginProxy *> pList, QUndoCommand *parent) : QUndoCommand(parent)
{
	linkItem = newLinkItem;
	hi = newHi;
	plugins = pList;
	
	/**
	 * example:
	 * router[eth1] ---> A
	 */
	setText(tr("Added a new link: ") + 
			hi->getMyVirtualMachine()->getName() + 
			"[" + hi->getName() + "] ---> " + hi->getMyCollisionDomain()->getName());
}

/**
 * Deconstructor
 */ 
AddLinkCommand::~AddLinkCommand()
{
}

/**
 * Redo action
 */
void AddLinkCommand::redo()
{
	//show and connect view & domain objects
	LinkMapper::getInstance()->addNewMapping(linkItem, hi);
	
	//call default label for plugins
	QListIterator<PluginProxy *> i(plugins);
	while(i.hasNext())
		i.next()->showDefaultGrophicsLabel();
}

/**
 * Undo action
 */
void AddLinkCommand::undo()
{
	//TODO
}
