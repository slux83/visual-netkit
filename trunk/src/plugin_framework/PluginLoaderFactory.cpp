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
#include "../gui/handles/VmMapper.h"
#include "../gui/handles/LinkMapper.h"
#include "../gui/handles/CdMapper.h"
#include "PluginRegistry.h"

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
	PluginProxy *pProxy = new PluginProxy(PluginRegistry::getInstance());
	
	p->setProxy(pProxy);
	pProxy->setPluginInterface(p);
	
	/* Connect the proxy signals with some system components */
	connect(p->getProxy(),
			SIGNAL(needLabelChangedVm(VirtualMachine*, QString, QString)),
			VmMapper::getInstance(),
			SLOT(changeGraphicsLabelVm(VirtualMachine*, QString, QString)));
	
	connect(p->getProxy(),
			SIGNAL(needLabelChangedHi(HardwareInterface*, QString, QString)),
			LinkMapper::getInstance(),
			SLOT(changeGraphicsLabelHi(HardwareInterface*, QString, QString)));
	
	connect(p->getProxy(),
			SIGNAL(needLabelChangedCd(CollisionDomain*, QString, QString)),
			CdMapper::getInstance(),
			SLOT(changeGraphicsLabelCd(CollisionDomain*, QString, QString)));
		
	return p->getProxy();
}

/**
 * Destroy the plugin and the associated proxy in the correct sequence
 */
void PluginLoaderFactory::destroyPlugin(PluginProxy *p)
{
	destroyPluginFactory(p->getPlugin());
	delete p;
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
	PluginProxy *tester = createPlugin();

	QSettings* pluginSetting = tester->getPluginSettings();

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
	
	destroyPlugin(tester);
	
	return retVal;
}
