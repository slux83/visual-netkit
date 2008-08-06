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

#include <QDir>
#include <QList>
#include <QMap>
#include <QObject>

#include "../core/VirtualMachine.h"
#include "../core/CollisionDomain.h"
#include "../core/HardwareInterface.h"

/**
 * IMPORTANT:
 * DO NOT INCLUDE OTHER HEADERS HERE, BROKE PLUGINS COMPILATION
 * Use "class XXX" instead and "#include XXX" header inside PluginRegistry.cpp
 */
class PluginLoaderFactory;
class PluginProxy;

/**
 * The sistem registry that offer a directory service
 */
class PluginRegistry
{
private:
	static PluginRegistry *instance;
	
	QMap<QString, PluginLoaderFactory*> factories;
	
	QList<QDir> pluginPaths;
	
	//Multi maps: the value for each key can have multi-values
	QMap<VirtualMachine*, PluginProxy*> vmAssociations;
	QMap<CollisionDomain*, PluginProxy*> cdAssociations;
	QMap<HardwareInterface*, PluginProxy*> hiAssociations;
	
public:
	PluginRegistry();
	virtual ~PluginRegistry();
	static PluginRegistry* getInstance();
	void clean();
	
	void fetchPlugins();
	PluginProxy* registerPlugin(QString pluginName, QObject* baseElement);
	
	QList<PluginProxy*> getVmProxies(VirtualMachine* vm)	{ return vmAssociations.values(vm); };
	QList<PluginProxy*> getCdProxies(CollisionDomain* cd)	{ return cdAssociations.values(cd); };
	QList<PluginProxy*> getHiProxies(HardwareInterface* hi)	{ return hiAssociations.values(hi); };
	QList<PluginProxy*> takeHiProxies(HardwareInterface* hi);
	QList<PluginProxy*> takeCdProxies(CollisionDomain* cd);
	QList<PluginProxy*> takeVmProxies(VirtualMachine *vm);
	
	QObject* getBaseElement(PluginProxy* proxy);
	QList<PluginProxy*> unregisterVmPlugins(VirtualMachine *vm, QStringList toDelete, QStringList *paths);
	const QList<PluginLoaderFactory*> getAllPluginFactories() { return factories.values(); };
	
	QList<PluginProxy*> getAllProxies();
	QSet<QString> getAllUsedPaths();
	
	void addPluginPath(QString pluginPath) { pluginPaths << QDir(pluginPath); };
	void destroyPlugin(PluginProxy *proxy);
};

#endif /*PLUGINREGISTRY_H_*/
