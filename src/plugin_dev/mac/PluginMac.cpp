/**
 * A simple test plugin for Visual Netkit 
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

#include "PluginMac.h"
#include "HardwareInterface.h"

/** 
 * Constructor 
 */
PluginMac::PluginMac() : PluginInterface()
{
	mySettings = new QSettings(":/ini_mac", QSettings::NativeFormat);
	
	/* Save the name of this plugin for future uses */
	mySettings->beginGroup("global");
	myName = mySettings->value("name").toString();
	mySettings->endGroup();
	
	myProxy = NULL;
	fetchProperties();
}

/**
 * Deconstructor
 */
PluginMac::~PluginMac()
{
	//NOTE: The proxy is destroyed by the plugin framework
	qDeleteAll(properties);
	delete mySettings;
}

/**
 * Returns the plugin template if resource file exists,
 * otherwise returns an empty QString.
 */
QMap<QString, QString> PluginMac::getTemplates()
{
	QMap<QString, QString> templates;
	QString templateContent;
	
	QFile data(":/ifconfig_tpl");
	if (data.open(QFile::ReadOnly)) 
	{
		QTextStream in(&data);
		templateContent = in.readAll();
		
		HardwareInterface *hi = static_cast<HardwareInterface*>(myProxy->getBaseElement());
		(hi != NULL)? templateContent.replace("<HI>", hi->getName()) : templateContent.replace("<HI>", "");
		
		QString status;
		(hi != NULL && hi->getState())? status = "up" : status = "down";
		templateContent.replace("<HI_STATE>", status);
		
		templateContent.replace("<MAC>", properties.value("mac_address")->getValue());

		/* Append comment */
		QString lineComment;

		if(hi != NULL)
			lineComment = QString("'").append(hi->getMyCollisionDomain()->getName()).append("' collision domain ##");
		else
			lineComment = QString("'unknown' collision domain ##");
		
		templateContent.replace("<COMMENT>", lineComment);
	}
	else
	{
		qWarning() << "PluginMac::getTemplates(): failed reading template file:" << endl << data.errorString();
	}
	
	templates.insert(getTemplateLocation(), templateContent);
	return templates;
}

/**
 * [PRIVATE]
 * Returns the path where save/append the template content
 */
QString PluginMac::getTemplateLocation()
{
	HardwareInterface *hi = static_cast<HardwareInterface*>(myProxy->getBaseElement());
	if (hi == NULL)
	{
		qWarning() << "PluginMac::getTemplateLocation(): null hardware interface name.";
		return QString();
	}
	QString vm = hi->getMyVirtualMachine()->getName();
	return QString(vm+".startup");
}

/**
 * Fetches plugin properties and stores them in the properties map.
 */
bool PluginMac::fetchProperties()
{
	bool allok = true;
	
	mySettings->beginGroup("properties");
	QStringList childgroups = mySettings->childGroups();

	if (!childgroups.empty()) 
	{
		for (int i = 0; i < childgroups.size(); i++)
		{
			QString p_name = childgroups.at(i);
			QString p_default_value = mySettings->value(childgroups.at(i) + "/p_default_value").toString();
			QString p_description = mySettings->value(childgroups.at(i) + "/p_description").toString();
			
			PluginProperty *pp = new PluginProperty(p_name, p_default_value, p_description);
			properties.insert(p_name, pp);
		}
	} else {
		qWarning() << "No properties for plugin:" << myName;
		allok = false;
	}
	mySettings->endGroup();
	
	return allok;
}

/**
 * If pluginAlertMsg is empty, initializes the passed property propName to propValue. 
 */
bool PluginMac::saveProperty(QString propName, QString propValue, QString *pluginAlertMsg)
{
	
	/* Check if property exist */
	if (!properties.contains(propName))
	{
		qWarning() << "PluginMac::initProperty: properties doesn't contain property" + propName;
		return true;
	}	
	
	/* Skip storage if the value is equals default */
	if (propValue == properties.value(propName)->getDefaultValue())
		return true;
	
	/* Force property save? */
	if (pluginAlertMsg == NULL)
	{
		PluginProperty *prop = properties.value(propName);
		prop->setValue(propValue);
		refreshLabel();
		
		return true;
	} 
	else 
	{
		if (propName == "mac_address") 
		{			
			/* Validate the MAC address */
			if(!validateMacAddress(propValue))
			{
				/* set a warning message */
				pluginAlertMsg->append("Invalid MAC address");
			}
			else 
			{
				PluginProperty *prop = properties.value(propName);
				prop->setValue(propValue);
				refreshLabel();
				return true;
			}
		}
		
	}
	return false;
}

/**
 * Init the plugin's properties parsing files
 */
bool PluginMac::init(QString laboratoryPath)
{
	/* Get my buddy */
	HardwareInterface *hi = static_cast<HardwareInterface*>(myProxy->getBaseElement());
	if (hi == NULL)
	{
		qWarning() << "PluginMac::init(): null hardware interface.";
		return false;
	}
	
	QRegExp macRegExp("([0-9A-Fa-f]{2}:){5}[0-9A-Fa-f]{2}");
	
	/* Parse my startup file and get the mac address if any */
	QString startupPath = laboratoryPath + "/" + hi->getMyVirtualMachine()->getName() + ".startup";
	startupPath.replace("//", "/");		//normalize path
	
	QFile startupFile(startupPath);
	if(!startupFile.exists())
	{
		qWarning() << "PluginMac::init(): startup" << startupPath << " file don't exist";
		return false;
	}
	
	if(!startupFile.open(QFile::ReadOnly))
	{
		qWarning() << "PluginMac::init(): cannot open" << startupPath << "file in read only mode.";
		return false;
	}
	
	QStringList startupContent = QString(startupFile.readAll()).split("\n");
	startupFile.close();
	
	/* reand and search my mac address */
	foreach(QString line, startupContent)
	{
		if(line.contains(hi->getName()))
		{
			macRegExp.indexIn(line);
			QString myMacAddress = macRegExp.capturedTexts()[0];
			if(validateMacAddress(myMacAddress))
			{
				//ok, founded my mac address
				properties.value("mac_address")->setValue(myMacAddress);
				myProxy->changeGraphicsLabel(myMacAddress);
						
				return true;
			}
		}
	}
	
	return false;	
}

/**
 * [PRIVATE]
 * Refresh the scene label if mac address is changed
 */
void PluginMac::refreshLabel()
{
	myProxy->changeGraphicsLabel(properties["mac_address"]->getValue());
}

/**
 * [PRIVATE]
 * Validate mac address passed
 */
bool PluginMac::validateMacAddress(QString &mac)
{
	int pos = 0;
	
	/* Mac validator */
	QRegExpValidator macValidator(QRegExp("^([0-9A-Fa-f]{2}:){5}[0-9A-Fa-f]{2}$"), NULL);
	
	/* Validate the MAC address */
	return (macValidator.validate(mac, pos) == QValidator::Acceptable);
}
