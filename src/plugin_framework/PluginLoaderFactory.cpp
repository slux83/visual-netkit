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
	if(!p)
		qDebug() << "err";
	PluginProxy *pProxy = new PluginProxy(PluginRegistry::getInstance());
	
	pProxy->setPluginInterface(p);
	p->setProxy(pProxy);
	
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
	
	//init the instance, and see if all is valid
	PluginProxy *tester = createPlugin();
	
	if(!tester->getPropertyExpert()->isXmlConfValid())
	{
		qWarning() << "Plugin" << fileName() << "have unvalid XML config file";
		retVal = false;
	}
	
	QMap<QString, QString> infos = tester->getPropertyExpert()->parseXmlGlobalInfo();
	
	/* save global infos */
	name = infos["plugin name"];
	type = infos["type"];
	description = infos["description"];
	version = infos["version"];
	author = infos["author"];
	deps << infos.values("dep");
	
	/* copy all properties for fast info access */
	properties = tester->getPluginProperties();
	
	destroyPlugin(tester);
	
	return retVal;
}
