/**
 * The Quagga-core plugin for Visual Netkit 
 * Copyright (C) 2008  Alessio Di Fazio
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

#include "PluginQuaggaCore.h"
#include "HardwareInterface.h"

/** 
 * Constructor 
 */
PluginQuaggaCore::PluginQuaggaCore() : PluginInterface()
{	
	myProxy = NULL;
}

/**
 * Deconstructor
 */
PluginQuaggaCore::~PluginQuaggaCore()
{
	//NOTE: The proxy is destroyed by the plugin framework
	qDeleteAll(properties);
}

/**
 * [PRIVATE]
 * Replace the daemons values inside the passed file template
 */
void PluginQuaggaCore::replaceDaemons(QString *templateFile)
{
	//get daemon properties
	QList<PluginProperty*> dPropertyList =
		myProxy->getPropertyExpert()->searchPropertiesById(properties, "daemons");
	
	if(dPropertyList.size() != 1)
	{
		qWarning() << "PluginQuaggaCore::replaceDaemons() daemons property not found";
		return;
	}
	
	//replace all
	foreach(PluginProperty *daemon, dPropertyList.first()->getChilds())
		templateFile->replace(daemon->getId(), daemon->getValue());
}

/**
 * Returns the plugin template if resource file exists,
 * otherwise returns an empty QString.
 */
QMap<QString, QString> PluginQuaggaCore::getTemplates()
{
	QMap<QString, QString> templates;
	QString templateContent;
	
	//damenos file
	templateContent = file2string(":/quagga-core/daemons");
	replaceDaemons(&templateContent);	
	templates.insert(getTemplateLocation()+"daemons", templateContent);
	
	//zebra.conf file
	templateContent = file2string(":/quagga-core/zebra.conf");
	templates.insert(getTemplateLocation()+"zebra.conf", templateContent);
	
	//start inside machine.startup
	templateContent = file2string(":/quagga-core/quagga_start");
	templates.insert(getTemplateLocation(true), templateContent);
		
	return templates;
}

/**
 * [PRIVATE]
 * Returns the path where save/append the template content
 */
QString PluginQuaggaCore::getTemplateLocation(bool getBase)
{
	VirtualMachine *vm = static_cast<VirtualMachine*>(myProxy->getBaseElement());
	if (vm == NULL)
	{
		qWarning() << "PluginQuaggaCore::getTemplateLocation(): null virtualmachine.";
		return QString();
	}
	
	return QString(vm->getName() + ((getBase)? ".startup" : "/etc/quagga/"));
}

/**
 * If pluginAlertMsg is empty, initializes the passed property propName to propValue. 
 */
bool PluginQuaggaCore::saveProperty(QString propUniqueId, QString propValue, QString *pluginAlertMsg)
{
	//Get the property
	PluginProperty *prop = myProxy->getPropertyExpert()->searchProperty(properties, propUniqueId);
	if(!prop)
	{
		qWarning() << "PluginQuaggaCore::saveProperty: properties doesn't contain property UID = " << propUniqueId ;
		return true;
	}	
	
	/* Skip storage if the value is equals default */
	if (propValue == prop->getDefaultValue())
		return true;
	
	/* Force property save? */
	if (pluginAlertMsg == NULL)
	{
		prop->setValue(propValue);		
		return true;
	} 

	/* Validate the value */
	QRegExp re("yes|no|[0-9]|10");
	QRegExpValidator validator(re, NULL);
	int pos = 0;
	if(validator.validate(propValue, pos) != QValidator::Acceptable)
	{
		if(pluginAlertMsg)
			pluginAlertMsg->append(QObject::tr("Invalid value. Only 'yes', 'no' or 0...10 values are allowed."));
		return false;
	}
	
	prop->setValue(propValue);
	
	return true;
	
}

/**
 * Set proxy
 */
void PluginQuaggaCore::setProxy(PluginProxy* p)
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
 * Init the plugin's properties parsing files
 */
bool PluginQuaggaCore::init(QString laboratoryPath)
{
	
	/* Get my buddy */
	VirtualMachine *vm = static_cast<VirtualMachine*>(myProxy->getBaseElement());
	if (vm == NULL)
	{
		qWarning() << "PluginQuaggaCore::init(): null VM";
		return false;
	}

	//QRegExp lineRegExp(".+\\b" + hi->getName() + "\\b(.+)\\bnetmask\\b(.+)\\bbroadcast\\b(.+\\.[0-9]{1,3})");
//	
//	/* Parse my startup file and get the mac address if any */
//	QString startupPath = laboratoryPath + "/" + hi->getMyVirtualMachine()->getName() + ".startup";
//	startupPath.replace("//", "/");		//normalize path
//	
//	QFile startupFile(startupPath);
//	if(!startupFile.exists())
//	{
//		qWarning() << "PluginIPv4::init(): startup" << startupPath << " file don't exist";
//		return false;
//	}
//	
//	if(!startupFile.open(QFile::ReadOnly))
//	{
//		qWarning() << "PluginIPv4::init(): cannot open" << startupPath << "file in read only mode.";
//		return false;
//	}
//	
//	QStringList startupContent = QString(startupFile.readAll()).split("\n", QString::SkipEmptyParts);
//	startupFile.close();
//	
//	bool parseOk = false;
//	
//	foreach(QString line, startupContent)
//	{
//		ipRegExp.indexIn(line);
//		QStringList capText = ipRegExp.capturedTexts();
//		
//		//It's my rule
//		if(capText[0].contains(hi->getName()))
//		{
//			QString addressVal = capText[1].trimmed();
//			QString netmaskVal = capText[2].trimmed();
//			QString broadcastVal = capText[3].trimmed();
//			
//			myProxy->getPropertyExpert()->searchPropertiesById(properties, "address").at(0)->setValue(addressVal);
//			myProxy->getPropertyExpert()->searchPropertiesById(properties, "netmask").at(0)->setValue(netmaskVal);
//			myProxy->getPropertyExpert()->searchPropertiesById(properties, "broadcast").at(0)->setValue(broadcastVal);
//			parseOk = true;
//			break;
//		}
//	}
//	
//	if(parseOk)
//	{
//		NetworkAddress addr(QHostAddress(
//			myProxy->getPropertyExpert()->searchPropertiesById(properties, "address").at(0)->getValue()),
//				QHostAddress(myProxy->getPropertyExpert()->searchPropertiesById(properties, "netmask").at(0)->getValue()));
//		
//		myProxy->changeGraphicsLabel(addr.toString(PRINT_CIDR_NETMASK));
//		
//		return true;
//	}
//	
	myProxy->changeGraphicsLabel(getDefaultGraphisLabel());

	return true;
}

/**
 * [PRIVATE]
 * Read an entire file in a string
 */
QString PluginQuaggaCore::file2string(const QString &filePath, bool *ok)
{
	QFile data(filePath);
	QString returnVal;
	if(data.open(QFile::ReadOnly)) 
	{
		QTextStream in(&data);
		returnVal = in.readAll();
		data.close();
		
		if(ok != NULL) *ok = true;
	}
	else
	{
		if(ok != NULL) *ok = false;
		qWarning() << "PluginQuaggaCore::file2string" << data.errorString();
	}
	
	return returnVal;
}

/**
 * UNUSED
 */
QPair<PluginProperty*, QString> PluginQuaggaCore::addProperty(QString propertyIdToAdd,
		QString parentPropertyUniqueId)
{
	Q_UNUSED(propertyIdToAdd);
	Q_UNUSED(parentPropertyUniqueId);
	
	return qMakePair(properties.at(0), QObject::tr("You cannot add properties for this plugin"));	//Return never used junk
}
