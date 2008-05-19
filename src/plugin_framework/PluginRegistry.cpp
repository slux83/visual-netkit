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
#include <QDir>

#include "PluginRegistry.h"
#include "PluginLoaderFactory.h"
#include "../common/CommonConfigs.h"


/* Init the instance field to NULL */
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

/**
 * Fetches all plugins in the plugin directory.
 */
bool PluginRegistry::fetchPlugins()
{
	bool allok = true;
	
	// checks if directory exists
	QDir pluginDir(DEFAULT_PLUGIN_DIR);
	if (!pluginDir.exists())
	{
		qWarning("Cannot find the plugin directory");
		allok = false;
	}	
	else
	{
		// gets plugins list in the dir
		QStringList list = pluginDir.entryList();
		QRegExp rx("*.so");
		rx.setPatternSyntax(QRegExp::Wildcard);
		QStringList filteredList = list.filter(rx);
		if (!filteredList.empty()) 
		{
			for (int i = 0; i < filteredList.size(); i++)
			{
				//loads the i-th plugin
				qDebug() << "Loading" << filteredList.at(i);
				qDebug() << pluginDir.filePath(filteredList.at(i));
				
				//inserts plugin and factory in factories map
				PluginLoaderFactory* factory = new PluginLoaderFactory(pluginDir.filePath(filteredList.at(i)));
				factory->initPluginLibrary();
				factories.insert(filteredList.at(i), factory);
			}
		} else {
			qWarning() << "Non ci sono plugin nella cartella" << DEFAULT_PLUGIN_DIR;
			allok = false;
		}
	}
	
	return allok;
}

