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

#ifndef LINKHANDLER_H_
#define LINKHANDLER_H_

#include <QObject>

#include "../VirtualMachineItem.h"
#include "../CollisionDomainItem.h"
#include "../../common/NetworkAddress.h"
#include "../../core/handles/VmFacadeController.h"
#include "VmMapper.h"
#include "CdMapper.h"
#include "../LinkItem.h"
#include "../undoCommands/UndoCommands.h"

class LinkHandler : public QObject
{
	Q_OBJECT
	
private:
	static LinkHandler* instance;
	VmFacadeController* vmFacadeController;
	VmMapper* vmMapper;
	CdMapper* cdMapper;
	LabHandler *labHandler;
	
public:
	LinkHandler();
	virtual ~LinkHandler();
	static LinkHandler* getInstance();
	
public slots:
	void createLink(VirtualMachineItem *vmItem, CollisionDomainItem *cdItem,
			QString ethName, bool state, NetworkAddress address);

};

#endif /*LINKHANDLER_H_*/
