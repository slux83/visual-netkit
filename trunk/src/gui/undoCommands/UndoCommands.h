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
#include "../CollisionDomainItem.h"
#include "../../core/CollisionDomain.h"
#include "../../core/HardwareInterface.h"
#include "../LinkItem.h"
#include "../handles/VmMapper.h"
#include "../handles/CdMapper.h"
#include "../handles/LinkMapper.h"

/**
 * [COMMAND]
 * Add a virtual machine command.
 */
class AddVmCommand : public QObject, public QUndoCommand
{
	
private:
	VirtualMachineItem *vmItem;
	VirtualMachine *vm;
	QList<PluginProxy*> plugins;	//the plugins used with the vm
	
public:
	AddVmCommand(VirtualMachineItem *newVmItem, VirtualMachine *newVm,
			QList<PluginProxy *> pList,	QUndoCommand *parent = 0);
	virtual ~AddVmCommand();
	
	//TODO
	void undo();
	void redo();
};

/******************************************************************************/

/**
 * [COMMAND]
 * Add a collision domain command.
 */
class AddCdCommand : public QObject, public QUndoCommand
{
	
private:
	CollisionDomainItem *cdItem;
	CollisionDomain *cd;
	QList<PluginProxy*> plugins;	//the plugins used with the cd
	
public:
	AddCdCommand(CollisionDomainItem *newCdItem, CollisionDomain *newCd,
			QList<PluginProxy *> pList, QUndoCommand *parent = 0);
	virtual ~AddCdCommand();
	
	//TODO
	void undo();
	void redo();
};

/******************************************************************************/

/**
 * [COMMAND]
 * Add a link command.
 */
class AddLinkCommand : public QObject, public QUndoCommand
{
	
private:
	LinkItem *linkItem;
	HardwareInterface *hi;
	QList<PluginProxy*> plugins;	//the plugins used with the hi
	
public:
	AddLinkCommand(LinkItem *newLinkItem, HardwareInterface *newHi, QList<PluginProxy *> pList,
			QUndoCommand *parent = 0);
	virtual ~AddLinkCommand();
	
	//TODO
	void undo();
	void redo();
};

/******************************************************************************/

/**
 * [COMMAND]
 * Delete plugins for a virtual machine.
 * This command stores delete plugins for a specific vm
 */
class DeleteVmPluginsCommand : public QObject, public QUndoCommand
{
	
private:
	VirtualMachine *vm;
	QList<PluginProxy*> plugins;	//the deleted
	QSet<QString> pathsToDelete;
	
public:
	DeleteVmPluginsCommand(VirtualMachine *vmP, QList<PluginProxy *> pList,
			QSet<QString> paths, QUndoCommand *parent = 0);
	virtual ~DeleteVmPluginsCommand();
	
	//TODO
	void undo();
	void redo();
};

/******************************************************************************/

/**
 * [COMMAND]
 * Delete a link/hardware interface
 */
class DeleteLinkCommand : public QObject, public QUndoCommand
{
	
private:
	HardwareInterface *hi;
	LinkItem *linkItem;
	QList<PluginProxy*> plugins;
	
public:
	DeleteLinkCommand(HardwareInterface *hInterface, QList<PluginProxy *> pList,
			LinkItem *lItem, QUndoCommand *parent = 0);
	virtual ~DeleteLinkCommand();
	
	//TODO
	void undo();
	void redo();
};

/**
 * [COMMAND]
 * Delete a Collision domain interface
 */
class DeleteCdCommand : public QObject, public QUndoCommand
{
	
private:
	CollisionDomain *cd;
	CollisionDomainItem *cdItem;
	QList<PluginProxy*> cdPlugins;
	QMap<LinkItem*, HardwareInterface*> links;
	
	//a multi-map
	QMap<HardwareInterface*, PluginProxy*> hiProxies;
	
public:
	DeleteCdCommand(CollisionDomain *cdD, CollisionDomainItem *cdItemD,
			QList<PluginProxy*> pList, QMap<LinkItem*, HardwareInterface*> linksMap,
			QMap<HardwareInterface*, PluginProxy*> hiProxiesMap,
			QUndoCommand *parent = 0);
	virtual ~DeleteCdCommand();
	
	//TODO
	void undo();
	void redo();
};

#endif /*UNDOCOMMANDS_H_*/
