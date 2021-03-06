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

#ifndef VMMAPPER_H_
#define VMMAPPER_H_

#include "../VirtualMachineItem.h"
#include "../../core/VirtualMachine.h"
#include "../AddVmForm.h"
#include "SceneTreeMapper.h"

#include <QObject>
#include <QDebug>
#include <QStringList>

/**
 * This class is the singleton controller for the virtual machines.
 * It's also map the graphical (view) item with the domain object (model)
 */
class VmMapper : public QObject
{
	Q_OBJECT
	
private:
	static VmMapper *instance;
	AddVmForm *addVm;
	
	/* the pair: <VIEW, DOMAIN> */
	QMap<VirtualMachineItem*, VirtualMachine*> mappings;
	
public:
	VmMapper();
	virtual ~VmMapper();
	static VmMapper* getInstance();
	void showAddVmForm(QPointF pos);
	void addNewMapping(VirtualMachineItem* vmItem, VirtualMachine* vm);
	QList<VirtualMachineItem*> getVmItems() { return mappings.keys(); };
	VirtualMachineItem* getVmItem(VirtualMachine *vm) { return mappings.key(vm); };
	VirtualMachine *getVm(VirtualMachineItem* vmItem) { return mappings.value(vmItem); };
	QStringList getMachineInterfaces(VirtualMachineItem* vmItem);
	VirtualMachine * getMachine(VirtualMachineItem *vmItem) { return mappings.value(vmItem); };
	QList<VirtualMachine *> getMachines() { return mappings.values(); };
	void ensureAllItemsAreJoined();
	void destroyMapper(VirtualMachineItem *vmItem) { mappings.remove(vmItem); };

public slots:
	void changeGraphicsLabelVm(VirtualMachine *vm, QString pluginName, QString label);
	void clear();

signals:
	void newMappingCreated(VirtualMachine *machine);
};

#endif /*VMMAPPER_H_*/
