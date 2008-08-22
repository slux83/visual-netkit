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
#include "../../core/handles/VmFacadeController.h"
#include "../LinkItem.h"
#include "../undoCommands/UndoCommands.h"
#include "VmMapper.h"
#include "CdMapper.h"
#include "LinkPropertyController.h"

/**
 * This class is the controller for the link operations (gui/scene side)
 */
class LinkHandler : public QObject
{
	Q_OBJECT
	
private:
	static LinkHandler* instance;
	VmFacadeController* vmFacadeController;
	VmMapper* vmMapper;
	CdMapper* cdMapper;
	LabHandler *labHandler;
	LinkPropertyController *propertyController;
	InitPluginsPropertiesDialog *pluginPropDialog;
	
public:
	LinkHandler();
	virtual ~LinkHandler();
	static LinkHandler* getInstance();
	bool hiNameExist(HardwareInterface *hi, QString toCheck);
	void updateLinkName(HardwareInterface *hi, QString newName);
	void renderLinkProperties(LinkItem *linkItem);
	void deleteLink(LinkItem *link);

public slots:
	void createLink(VirtualMachineItem *vmItem, CollisionDomainItem *cdItem,
			QStringList selectedPlugins, bool manuallyInit, QString ethName, bool state);

};

#endif /*LINKHANDLER_H_*/
