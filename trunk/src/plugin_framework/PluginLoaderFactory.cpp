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

#include "PluginLoaderFactory.h"

/**
 * Constructor
 */
PluginLoaderFactory::PluginLoaderFactory(const QString &fileName, QObject *parent)
	: QLibrary(fileName, parent)
{
	
}

/**
 * Deconstructor
 */
PluginLoaderFactory::~PluginLoaderFactory()
{
	qDeleteAll(properties);
}

/**
 * Factory function that return a new instance of the plugin proxy
 * NOTE: the sistem registry is the caller
 */
PluginProxy * PluginLoaderFactory::createPlugin()
{
	PluginInterface *p = createPluginFactory();
	
	return p->getProxy();
}

/**
 * Init and check the plugin consistance
 */
bool PluginLoaderFactory::initPluginLibrary()
{
	bool retVal = true;
	
	createPluginFactory = (createPlugin_t *)resolve("createPlugin");
	destroyPluginFactory = (destroyPlugin_t *)resolve("destroyPlugin");
	
	if(!createPluginFactory || !destroyPluginFactory)
	{
		qWarning()	<< "ERROR: cannot resolve factories for plugin:"
					<< fileName() << "\n" << errorString();
		return false;
	}
	
	//init the instance, and get the QSetting
	PluginInterface *tester = createPluginFactory();
	QSettings* pluginSetting = tester->getMySettings();
	
	/* Validate settings */
	pluginSetting->beginGroup("global");
	QStringList generalKeys = pluginSetting->childKeys();
	
	//validate keys (General)
	if(retVal && (!generalKeys.contains("name") ||
		!generalKeys.contains("type") ||
		!generalKeys.contains("description") ||
		!generalKeys.contains("version") ||
		!generalKeys.contains("author") ||
		!generalKeys.contains("dependencies")))
	{
		qWarning() << "Plugin" << fileName() << "have unvalid config file (global section)";
		retVal = false;
	}
	
	/* save global infos */
	name = pluginSetting->value("name").toString();
	type = pluginSetting->value("type").toString();
	description = pluginSetting->value("description").toString();
	version = pluginSetting->value("version").toString();
	author = pluginSetting->value("author").toString();
	deps = pluginSetting->value("dependencies").toString();
	
	//type must be 'vm' or 'cd' or 'link'
	QRegExp typeValidator("vm|cd|link");
	if(retVal && !typeValidator.exactMatch(pluginSetting->value("type").toString()))
	{
		qWarning() << "Plugin" << fileName() << "have unvalid type (global section)";
		retVal = false;
	}
	
	//TODO: plugin name must be unique
	
	pluginSetting->endGroup();
	
	/* copy all properties for fast info access */
	QListIterator<PluginProperty *>propIterator(tester->getPluginProperties().values());
	while(propIterator.hasNext())
	{
		//Clone property
		PluginProperty *original = propIterator.next();
		PluginProperty *clone =
			new PluginProperty(name, original->getDefaultValue(), original->getDescription());
		properties.append(clone);
	}
	
	destroyPluginFactory(tester);
	
	return retVal;
}
