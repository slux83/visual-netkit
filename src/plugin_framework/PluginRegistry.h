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

#ifndef PLUGINREGISTRY_H_
#define PLUGINREGISTRY_H_

#include <QMap>
#include <QObject>
#include "PluginProxy.h"
#include "PluginLoaderFactory.h"
#include "../core/VirtualMachine.h"
#include "../core/CollisionDomain.h"
#include "../core/HardwareInterface.h"

class PluginRegistry
{
private:
	static PluginRegistry *instance;
	
	QMap<QString, PluginLoaderFactory*> factories;
	QMap<VirtualMachine*, PluginProxy*> vmAssociations;
	QMap<CollisionDomain*, PluginProxy*> cdAssociations;
	QMap<HardwareInterface*, PluginProxy*> hiAssociations;
	
public:
	PluginRegistry();
	virtual ~PluginRegistry();
	static PluginRegistry* getInstance();
	
	bool fetchPlugins();
	PluginProxy* registerPlugin(QString pluginName, QObject* baseElement);
	
	PluginProxy* getVmProxy(VirtualMachine* vm) { return vmAssociations.value(vm); };
	PluginProxy* getCdProxy(CollisionDomain* cd) { return cdAssociations.value(cd); };
	PluginProxy* getHiProxy(HardwareInterface* hi) { return hiAssociations.value(hi); };
};

#endif /*PLUGINREGISTRY_H_*/
