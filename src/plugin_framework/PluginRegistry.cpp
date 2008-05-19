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

#include <QMap>

#include "PluginRegistry.h"
#include "PluginLoaderFactory.h"


/* Init che instance field to NULL */
PluginRegistry* PluginRegistry::instance = NULL;

/**
 * Constructor
 */
PluginRegistry::PluginRegistry()
{
}

/**
 * Deconstructor
 */
PluginRegistry::~PluginRegistry()
{
}

/**
 * [STATIC]
 * Singleton
 */
PluginRegistry* PluginRegistry::getInstance()
{
	if (instance == NULL) 
	{
		instance = new PluginRegistry();
	}
	return instance;
}

/**
 * Registers a plugin proxy in the register and returns it.
 */
PluginProxy* PluginRegistry::registerPlugin(QString pluginName, QObject* baseElement)
{	
	PluginProxy* proxy = factories.value(pluginName)->createPlugin(baseElement);
	
	VirtualMachine *vm = dynamic_cast<VirtualMachine*>(baseElement);
	if ( vm != NULL ) {
		vmAssociations.insert(vm, proxy);
	}
	
	CollisionDomain *cd = dynamic_cast<CollisionDomain*>(baseElement);
	if ( cd != NULL ) {
		cdAssociations.insert(cd, proxy);
	}
	
	HardwareInterface *hi = dynamic_cast<HardwareInterface*>(baseElement);
	if ( hi != NULL ) {
		hiAssociations.insert(hi, proxy);
	}
	
	return proxy;
}

bool PluginRegistry::fetchPlugins()
{
	bool allok = true;
	//factories.insert(pluginName, new PluginLoaderFactory(getPluginPath()));
	return allok;
}

/**
 * [PROTECTED]
 * Returns passed plugin file path.
 */
//TODO
QString getPluginPath(QString pluginName)
{
	return "";
}

