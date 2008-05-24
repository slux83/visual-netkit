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

#include "VmMapper.h"
#include "LabHandler.h"
#include <QListIterator>

/**
 * Init the null instance for the singletone controller
 */
VmMapper* VmMapper::instance = NULL;

/**
 * Constructor
 */
VmMapper::VmMapper() : QObject()
{
	addVm = NULL;
	
	/* Connect signals */
	connect(this, SIGNAL(newMappingCreated(VirtualMachine *)),
			LabHandler::getInstance(), SLOT(addCreatedVmOnTree(VirtualMachine *)));
}

/**
 * Deconstructor
 */
VmMapper::~VmMapper()
{
}

/**
 * Singletone get instance
 */
VmMapper * VmMapper::getInstance()
{
	if (instance == NULL)
	{
		instance = new VmMapper();
	}

	return instance;
}

/**
 * Show the form
 */
void VmMapper::showAddVmForm(QPointF pos)
{
	//lazy init
	if(addVm == NULL)
		addVm = new AddVmForm();
	
	addVm->setMachinePos(pos);	//update the pos for the new machine
	
	addVm->show();
}

/**
 * Add a new mapping (view, domain) and pass the view item to the scene
 */
void VmMapper::addNewMapping(VirtualMachineItem* vmItem, VirtualMachine* vm)
{
	//add mapping
	mappings.insert(vmItem, vm);
	LabHandler *labHandler = LabHandler::getInstance();
	labHandler->getMainWindow()->graphicsView->scene()->addItem(vmItem);
	labHandler->getMainWindow()->graphicsView->ensureVisible(vmItem);
	
	/* add vm inside the scene tree */
	SceneTreeMapper::getInstance()->addVmElement(vmItem);
	
	//emit signal
	emit newMappingCreated(vm);
}

/**
 * Get the interfaces for the machine passed
 * name -> ip/netmask
 */
QStringList VmMapper::getMachineInterfaces(VirtualMachineItem* vmItem)
{
	QStringList interfaces;
	VirtualMachine* vm = mappings.value(vmItem);
	QMapIterator<QString, HardwareInterface *> i(vm->getInterfaces());
	
	while(i.hasNext())
	{
		i.next();
		interfaces.append(i.key());
	}
	
	return interfaces;
}

/**
 * Ensure that all graphics items are in a joined state (label is joined inside
 * the group)
 */
void VmMapper::ensureAllItemsAreJoined()
{
	QListIterator<VirtualMachineItem*> itemIterator(mappings.keys());
	while(itemIterator.hasNext())
		itemIterator.next()->restoreGroupActionCalled();
}

/**
 * [SLOT]
 * Changes the label associated to the pluginName for the VirtualMachineItem associated with vm.
 */
void VmMapper::changeGraphicsLabel(VirtualMachine *vm, QString pluginName, QString label)
{
	qDebug() << vm->getName() << pluginName << label;
/*	
	VirtualMachineItem *vmi = mappings.key(vm);
	if (vmi == NULL)
	{
		qWarning() << "VmMapper::changeGraphicsLabel: null pointer to VirtualMachineItem.";
		return;
	}
	
	vmi->setPluginLine(pluginName, label);
*/	
}

