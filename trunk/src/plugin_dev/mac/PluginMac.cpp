/**
 * A Media Access Control (MAC) plugin for Visual Netkit
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
	myProxy = NULL;
}

/**
 * Deconstructor
 */
PluginMac::~PluginMac()
{
	//NOTE: The proxy is destroyed by the plugin framework
	qDeleteAll(properties);
}

/**
 * Returns the plugin template if resource file exists,
 * otherwise returns an empty QString.
 */
QMap<QString, QString> PluginMac::getTemplates()
{
	QMap<QString, QString> templates;
	QString templateContent;
	PluginProperty *macProperty = properties.at(0);	//we have only one property
	
	QFile data(":/mac/ifconfig_tpl");
	if (data.open(QFile::ReadOnly)) 
	{
		QTextStream in(&data);
		templateContent = in.readAll();
		
		HardwareInterface *hi = static_cast<HardwareInterface*>(myProxy->getBaseElement());
		(hi != NULL)? templateContent.replace("<HI>", hi->getName()) : templateContent.replace("<HI>", "");
		
		templateContent.replace("<MAC>", macProperty->getValue());

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
 * If pluginAlertMsg is empty, initializes the passed property propName to propValue. 
 */
bool PluginMac::saveProperty(QString propertyUniqueId, QString propValue, QString *pluginAlertMsg)
{
	PluginProperty *macProperty = properties.at(0);
	
	/* Check if property exist */
	if (macProperty->getUniqueId() != propertyUniqueId)
	{
		qWarning() << "PluginMac::initProperty: properties doesn't contain property" + propertyUniqueId;
		return true;
	}	
	
	/* Skip storage if the value is equals default */
	if (propValue == macProperty->getDefaultValue())
		return true;
	
	/* Force property save? */
	if (pluginAlertMsg == NULL)
	{
		macProperty->setValue(propValue);
		refreshLabel();
		
		return true;
	} 
	else 
	{
		/* Validate the MAC address */
		if(!validateMacAddress(propValue))
		{
			/* set a warning message */
			pluginAlertMsg->append("Invalid MAC address");
		}
		else 
		{
			macProperty->setValue(propValue);
			refreshLabel();
			return true;
		}
		
	}
	
	return false;
}

/**
 * Init the plugin's properties parsing files
 */
bool PluginMac::init(QString laboratoryPath)
{
	PluginProperty *macProperty = properties.at(0);

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
				macProperty->setValue(myMacAddress);
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
	myProxy->changeGraphicsLabel(properties.at(0)->getValue());
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

/**
 * Set 'my' proxy and get my info/properties
 */
void PluginMac::setProxy(PluginProxy* p)
{
	myProxy = p;
	
	//Now we can get plugin infos and base properties from property expert
	if(p->getPropertyExpert()->isXmlConfValid())
	{
		globalInfo = p->getPropertyExpert()->parseXmlGlobalInfo();
		properties = p->getPropertyExpert()->buildBaseProperties();
	}
}

/**
 * UNUSED
 */
QPair<PluginProperty*, QString> PluginMac::addProperty(QString propertyIdToAdd,
		QString parentPropertyUniqueId)
{
	Q_UNUSED(propertyIdToAdd);
	Q_UNUSED(parentPropertyUniqueId);
	
	return qMakePair(properties.at(0), QObject::tr("You cannot add properties for this plugin"));	//Return never used junk
}

