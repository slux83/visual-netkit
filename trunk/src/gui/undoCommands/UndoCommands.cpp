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
	//NOTE: do not destroy elemes here
	plugins.clear();
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
	{
		PluginProxy *p = i.next();
		p->showDefaultGraphicsLabel();
		QList<QString> tplPaths = p->getTemplates().keys();
		
		// for each plugin and for each key in the templates map
		for (int j=0; j<tplPaths.size(); j++)
		{
			// adds path to tree
			LabHandler::getInstance()->addPathToTree(tplPaths.at(j));
		}
	}
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
	plugins.clear();
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
	{
		PluginProxy *p = i.next();
		p->showDefaultGraphicsLabel();
		QList<QString> tplPaths = p->getTemplates().keys();
		
		// for each plugin and for each key in the templates map
		for (int j=0; j<tplPaths.size(); j++)
		{
			// adds path to tree
			LabHandler::getInstance()->addPathToTree(tplPaths.at(j));
		}
	}
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
	plugins.clear();
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
	{
		PluginProxy *p = i.next();
		p->showDefaultGraphicsLabel();
		QList<QString> tplPaths = p->getTemplates().keys();
		// for each plugin and for each key in the templates map
		for (int j=0; j<tplPaths.size(); j++)
		{
			// adds path to tree
			LabHandler::getInstance()->addPathToTree(tplPaths.at(j));
		}
	}
}

/**
 * Undo action
 */
void AddLinkCommand::undo()
{
	//TODO
}

/******************************************************************************/

/**
 * Contructor
 */
DeleteVmPluginsCommand::DeleteVmPluginsCommand(VirtualMachine *vmP, QList<PluginProxy *> pList,
		QSet<QString> paths, QUndoCommand *parent) : QUndoCommand(parent)
{
	pathsToDelete = paths;
	plugins = pList;
	vm = vmP;
	
	setText(tr("Deleted plugins for vm: ") + vm->getName());
}

/**
 * Deconstructor
 */ 
DeleteVmPluginsCommand::~DeleteVmPluginsCommand()
{
	//NOTE: do not destroy plugin here (see addCommands)
	plugins.clear();
}

/**
 * Redo action
 */
void DeleteVmPluginsCommand::redo()
{
	/* Clear the tree-lab view deleting unused paths */
	QRegExpValidator startupValidator(QRegExp("^[a-zA-Z0-9]+\\.startup$"), this);
	QSet<QString> allPaths = PluginRegistry::getInstance()->getAllUsedPaths();
	
	foreach(QString path, pathsToDelete)
	{
		int pos = 0;
		//exclude "lab.conf" and startup files
		if(path != "lab.conf" &&
				startupValidator.validate(path, pos) == QValidator::Invalid &&
				!allPaths.contains(path))
		{
			//Delete path from tree
			LabHandler::getInstance()->removePathFromTree(path);
		}
	}
}

/**
 * Undo action
 */
void DeleteVmPluginsCommand::undo()
{
	//TODO
}

/******************************************************************************/

/**
 * Contructor
 */
DeleteLinkCommand::DeleteLinkCommand(HardwareInterface *hInterface, QList<PluginProxy *> pList,
		LinkItem *lItem, QUndoCommand *parent) : QUndoCommand(parent)
{
	hi = hInterface;
	plugins = pList;
	linkItem = lItem;
	
	// host12[eth3]
	setText(tr("Deleted link ") +
			hi->getMyVirtualMachine()->getName().append("[").append(hi->getName()).append("]"));
}

/**
 * Deconstructor
 */ 
DeleteLinkCommand::~DeleteLinkCommand()
{
	plugins.clear();
}

/**
 * Redo action
 */
void DeleteLinkCommand::redo()
{
	qDebug() << "link deleted";
	LabHandler::getInstance()->getMainWindow()->writeLogMessage(text());
}

/**
 * Undo action
 */
void DeleteLinkCommand::undo()
{
	//TODO
}

/******************************************************************************/

/**
 * Contructor
 */
DeleteCdCommand::DeleteCdCommand(CollisionDomain *cdD, CollisionDomainItem *cdItemD,
		QList<PluginProxy*> pList, QMap<LinkItem*, HardwareInterface*> linksMap,
		QMap<HardwareInterface*, PluginProxy*> hiProxiesMap, QUndoCommand *parent) : QUndoCommand(parent)
{
	cd = cdD;
	cdItem = cdItemD;
	cdPlugins = pList;
	links = linksMap;
	hiProxies = hiProxiesMap;

	setText(tr("Deleted collision domain ") + "\"" + cd->getName() + "\"");
}

/**
 * Deconstructor
 */ 
DeleteCdCommand::~DeleteCdCommand()
{
	cdPlugins.clear();
	hiProxies.clear();
	links.clear();
}

/**
 * Redo action
 */
void DeleteCdCommand::redo()
{
	qDebug() << "Cd deleted";
	LabHandler::getInstance()->getMainWindow()->writeLogMessage(text());
}

/**
 * Undo action
 */
void DeleteCdCommand::undo()
{
	//TODO
}

/******************************************************************************/

/**
 * Contructor
 */
DeleteVmCommand::DeleteVmCommand(VirtualMachine *vmD, VirtualMachineItem *vmItemD,
		QList<PluginProxy*> pList, QMap<LinkItem*, HardwareInterface*> linksMap,
		QMap<HardwareInterface*, PluginProxy*> hiProxiesMap,
		QUndoCommand *parent) : QUndoCommand(parent)
{
	vm = vmD;
	vmItem = vmItemD;
	vmPlugins = pList;
	links = linksMap;
	hiProxies = hiProxiesMap;
	
	// host12
	setText(tr("Deleted virtual machine ") + "\"" + vm->getName() + "\"");
}

/**
 * Deconstructor
 */ 
DeleteVmCommand::~DeleteVmCommand()
{
	vmPlugins.clear();
	hiProxies.clear();
	links.clear();
}

/**
 * Redo action
 */
void DeleteVmCommand::redo()
{
	qDebug() << "Vm deleted";
	LabHandler::getInstance()->getMainWindow()->writeLogMessage(text());
}

/**
 * Undo action
 */
void DeleteVmCommand::undo()
{
	//TODO
}
