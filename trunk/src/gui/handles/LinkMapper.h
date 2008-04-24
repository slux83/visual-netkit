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

#ifndef LINKMAPPER_H_
#define LINKMAPPER_H_

#include <QObject>
#include <QGraphicsItemGroup>
#include "../../core/HardwareInterface.h"
#include "../VirtualMachineItem.h"
#include "../CollisionDomainItem.h"
#include "../../core/VirtualMachine.h"
#include "../AddLinkForm.h"

class LinkMapper : QObject
{
	Q_OBJECT
	
private:
	static LinkMapper* instance;
	AddLinkForm *addLinkForm;
	
	/* the pair: <VIEW, DOMAIN> */
	QMap<QGraphicsItemGroup*, HardwareInterface*> mappings;
	
public:
	LinkMapper();
	virtual ~LinkMapper();
	static LinkMapper* getInstance();

public slots:
	void showAddLinkForm(VirtualMachineItem *vm, CollisionDomainItem* cd);
};

#endif /*LINKMAPPER_H_*/
