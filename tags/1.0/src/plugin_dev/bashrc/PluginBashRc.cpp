/** 
 * A simple Wiki (how-to) plugin for Visual Netkit 
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

#include "PluginBashRc.h"

/** 
 * Constructor 
 */
PluginBashRc::PluginBashRc() : PluginInterface()
{
	//get the ini file from resource
	mySettings = new QSettings(":/ini_bashrc", QSettings::NativeFormat);
	
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
PluginBashRc::~PluginBashRc()
{
	//NOTE: The proxy is destroyed by the plugin framework
	qDeleteAll(properties);
	properties.clear();
	delete mySettings;
}

/**
 * Returns the plugin templates
 */
QMap<QString, QString> PluginBashRc::getTemplates()
{
	QMap<QString, QString> templates;
	QString mainBashContent;
	
	QFile data(":/bash_main");
	QFile aliases(":/bash_aliases");
	QFile ps1(":/bash_ps1");
	
	/* Read bash main */
	if (data.open(QFile::ReadOnly)) 
	{
		QTextStream in(&data);
		mainBashContent = in.readAll();
		data.close();
	}
	else
	{
		qWarning() << "The plugin getTemplate() failed:" << data.errorString();
	}
		
	/* append PS1 and/or asiases according to propertie values */
	if(properties["use aliases"]->getValue() == "true")
	{
		/* Read bash aliases */
		if (aliases.open(QFile::ReadOnly)) 
		{
			QTextStream in(&aliases);
			mainBashContent.append(in.readAll());	//append to bashrc content
			aliases.close();
		}
		else
		{
			qWarning() << "The plugin getTemplate() failed:" << aliases.errorString();
		}
	}
	
	if(properties["use PS1"]->getValue() == "true")
	{
		/* Read bash aliases */
		if (ps1.open(QFile::ReadOnly)) 
		{
			QTextStream in(&ps1);
			mainBashContent.append(in.readAll());	//append to bashrc content
			ps1.close();
		}
		else
		{
			qWarning() << "The plugin getTemplate() failed:" << ps1.errorString();
		}
	}
	
	//we have only one template file
	templates.insert(getTemplateLocation(), mainBashContent);
	
	return templates;
}

/**
 * [PRIVATE]
 * Returns the path where save/append the template content
 */
QString PluginBashRc::getTemplateLocation()
{
	VirtualMachine *vm = static_cast<VirtualMachine*>(myProxy->getBaseElement());
	if (vm == NULL)
	{
		qWarning() << "Plugin::getTemplateLocation(): null VirtualMachine name.";
		return QString();
	}
	
	return QString(vm->getName() + "/root/.bashrc");
}

/**
 * [PRIVATE]
 * Fetches plugin properties and stores them in the properties list.
 */
void PluginBashRc::fetchProperties()
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
		
		PluginProperty *pp = new PluginProperty(p_name, p_default_value, p_description);
		properties.insert(pp->getName(), pp);
	}
	
	mySettings->endGroup();		//Remember endGroup() to restore the settings index
	
}

/**
 * If pluginAlertMsg is empty, initializes the passed property propName to propValue. 
 */
bool PluginBashRc::saveProperty(QString propName, QString propValue, QString *pluginAlertMsg)
{
	/* Check the property name */
	if(!properties.contains(propName))
	{
		if(pluginAlertMsg != NULL) { pluginAlertMsg->append("Property not found."); }
		
		return false;
	}
	
	/* Validate the value of the property: only "true" or "false" */
	if(pluginAlertMsg != NULL && propValue != "true" && propValue != "false")
	{
		pluginAlertMsg->append("You can set only \"true\" or \"false\" values.");
		return false;
	}
	
	/* All is ok, store the property */
	properties.value(propName)->setValue(propValue);
	
	return true;
}
