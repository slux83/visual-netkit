/** 
 * Zebra plugin for Visual Netkit 
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

#include <QDebug>
#include <QResource>
#include <QFile>

#include "PluginZebra.h"

/** 
 * Constructor 
 */
PluginZebra::PluginZebra() : PluginInterface()
{
	//get the ini file from resource
	mySettings = new QSettings(":/zebra/ini", QSettings::NativeFormat);
	
	/* Save the name of this plugin for future uses */
	mySettings->beginGroup("global");
	myName = mySettings->value("name").toString();
	mySettings->endGroup();
	
	myProxy = NULL;
	fetchProperties();	//read properties from ini file
}

/**
 * Deconstructor
 */
PluginZebra::~PluginZebra()
{
	//NOTE: The proxy is destroyed by the plugin framework
	qDeleteAll(properties);
	properties.clear();
	delete mySettings;
}

/**
 * Returns the plugin templates
 */
QMap<QString, QString> PluginZebra::getTemplates()
{
	QMap<QString, QString> templates;
	
	QFile zebraConf(":/zebra/zebra.conf");
	QFile daemons(":/zebra/daemons");
	
	/* Daemons template */
	if(daemons.open(QFile::ReadOnly))
	{
		QTextStream in(&daemons);
		QString zebraConfContent(in.readAll());
		
		/* change tags */
		zebraConfContent.replace("<USE_ZEBRA>",	properties["use zebra daemon"]->getValue());
		zebraConfContent.replace("<USE_BGP>",	properties["use bgp daemon"]->getValue());
		zebraConfContent.replace("<USE_RIP>",	properties["use rip daemon"]->getValue());
		
		templates.insert(getTemplateLocation().append("daemons"), zebraConfContent);
		daemons.close();
	}
	else
	{
		qWarning() << "The plugin getTemplate() failed:" << daemons.errorString();
	}
	
	/* Zebra template (static) */
	if (zebraConf.open(QFile::ReadOnly)) 
	{
		QTextStream in(&zebraConf);
		QString fileContent = in.readAll();
		
		/* Change tags */
		VirtualMachine *vm = static_cast<VirtualMachine*>(myProxy->getBaseElement());
		QString hostname = (vm)? vm->getName() : "zebra";

		fileContent.replace("<ZEBRA_HOSTNAME>",		hostname);
		fileContent.replace("<ZEBRA_PASSWORD>",		properties["zebra password"]->getValue());
		fileContent.replace("<ZEBRA_LOG_PATH>",		properties["zebra log path"]->getValue());
	
		templates.insert(getTemplateLocation().append("zebra.conf"), fileContent);
		zebraConf.close();
	}
	else
	{
		qWarning() << "The plugin getTemplate() failed:" << daemons.errorString();
	}
	
	/* Startup file to init zebra */
	templates.insert(getTemplateLocation(true), "\n/etc/init.d/zebra start\n");
	
	return templates;
}

/**
 * [PRIVATE]
 * Returns the path where save/append the template content
 */
QString PluginZebra::getTemplateLocation(bool startup)
{
	VirtualMachine *vm = static_cast<VirtualMachine*>(myProxy->getBaseElement());
	if (vm == NULL)
	{
		qWarning() << "Plugin::getTemplateLocation(): null VirtualMachine name.";
		return QString();
	}
	
	if(startup)
		return QString(vm->getName() + ".startup");
	else
		return QString(vm->getName() + "/etc/zebra/");
}

/**
 * [PRIVATE]
 * Fetches plugin properties and stores them in the properties list.
 */
void PluginZebra::fetchProperties()
{
	
	mySettings->beginGroup("properties");
	QStringList childgroups = mySettings->childGroups();
	
	if (childgroups.empty()) 
	{
		qWarning() << "No properties for plugin:" << myName;
	}
		
	/* Read the properties */
	for (int i = 0; i < childgroups.size(); i++)
	{
		QString p_name = childgroups.at(i);
		QString p_default_value = mySettings->value(childgroups.at(i) + "/p_default_value").toString();
		QString p_description = mySettings->value(childgroups.at(i) + "/p_description").toString();
		QString p_order = mySettings->value(childgroups.at(i) + "/p_init_order").toString();
		
		PluginProperty *pp = new PluginProperty(p_name, p_default_value, p_description);
		pp->setOrder(p_order.toInt());
		
		properties.insert(pp->getName(), pp);
	}
	
	mySettings->endGroup();		//Remember endGroup() to restore the settings index
	
}

/**
 * If pluginAlertMsg is empty, initializes the passed property propName to propValue. 
 */
bool PluginZebra::saveProperty(QString propName, QString propValue, QString *pluginAlertMsg)
{
	/* Check the property name */
	if(!properties.contains(propName))
	{
		if(pluginAlertMsg != NULL) { pluginAlertMsg->append("Property not found."); }
		
		return false;
	}
	
	/* Validate the value for daemons list: only "no" or "yes" */
	if(pluginAlertMsg != NULL && propValue != "no" && propValue != "yes" && propName.startsWith("use "))
	{
		pluginAlertMsg->append("You can set only \"yes\" or \"no\" values for daemons properties.");
		return false;
	}
	
	/* All is ok, store the property */
	properties.value(propName)->setValue(propValue);	
	
	return true;
}

/**
 * Init the plugin on load
 */
bool PluginZebra::init(QString laboratoryPath)
{
	Q_UNUSED(laboratoryPath)
	
	/* Set the graphic label */
	myProxy->changeGraphicsLabel("zebra");
	
	return false;
}
