/**
 * The IPv4 plugin for Visual Netkit 
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

#include "PluginIPv4.h"
#include "HardwareInterface.h"

/** 
 * Constructor 
 */
PluginIPv4::PluginIPv4() : PluginInterface()
{	
	myProxy = NULL;
}

/**
 * Deconstructor
 */
PluginIPv4::~PluginIPv4()
{
	//NOTE: The proxy is destroyed by the plugin framework
	qDeleteAll(properties);
}

/**
 * Returns the plugin template if resource file exists,
 * otherwise returns an empty QString.
 */
QMap<QString, QString> PluginIPv4::getTemplates()
{
	QMap<QString, QString> templates;
	QString templateContent;
	
	QFile data(":/ipv4/basic_tpl");
	if (data.open(QFile::ReadOnly)) 
	{
		QTextStream in(&data);
		templateContent = in.readAll();
		
		HardwareInterface *hi = static_cast<HardwareInterface*>(myProxy->getBaseElement());
		(hi != NULL)? templateContent.replace("<HI>", hi->getName()) : templateContent.replace("<HI>", "ethUNKNOWN");
		
		QList<PluginProperty*> sProp =
			myProxy->getPropertyExpert()->searchPropertiesById(properties, "address");
		
		if(sProp.isEmpty())
		{
			qWarning() << "PluginIPv4::getTemplates() Address property not found.";
			data.close();
			return templates;
		}
		
		PluginProperty *pp = sProp.at(0);
		
		templateContent.replace("<IP>", pp->getValue());
		
		sProp =	myProxy->getPropertyExpert()->searchPropertiesById(properties, "netmask");
		
		if(sProp.isEmpty())
		{
			qWarning() << "PluginIPv4::getTemplates() Netmask property not found.";
			data.close();
			return templates;
		}
		
		pp = sProp.at(0);
		
		/* Check if netmask is in CIDR notation */
		QString netmask = pp->getValue();
		bool isCidr;
		netmask.toInt(&isCidr);

		if(isCidr)
			templateContent.replace("<NETMASK>", NetworkAddress::cidr2netmask(netmask.toInt()).toString());
		else
			templateContent.replace("<NETMASK>", netmask);
		
		sProp =	myProxy->getPropertyExpert()->searchPropertiesById(properties, "broadcast");
				
		if(sProp.isEmpty())
		{
			qWarning() << "PluginIPv4::getTemplates() Broadcast property not found.";
			data.close();
			return templates;
		}
		
		pp = sProp.at(0);
		
		templateContent.replace("<BROADCAST>", pp->getValue());
		
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
		qWarning() << "PluginIPv4::getTemplates(): failed reading template file:" << endl << data.errorString();
	}
	
	templates.insert(getTemplateLocation(), templateContent);
	return templates;
}

/**
 * [PRIVATE]
 * Returns the path where save/append the template content
 */
QString PluginIPv4::getTemplateLocation()
{
	HardwareInterface *hi = static_cast<HardwareInterface*>(myProxy->getBaseElement());
	if (hi == NULL)
	{
		qWarning() << "PluginIPv4::getTemplateLocation(): null hardware interface name.";
		return QString();
	}
	QString vm = hi->getMyVirtualMachine()->getName();
	return QString(vm + ".startup");
}

/**
 * If pluginAlertMsg is empty, initializes the passed property propName to propValue. 
 */
bool PluginIPv4::saveProperty(QString propUniqueId, QString propValue, QString *pluginAlertMsg)
{
	//Get the property
	PluginProperty *prop = myProxy->getPropertyExpert()->searchProperty(properties, propUniqueId);
	if(!prop)
	{
		qWarning() << "PluginIPv4::initProperty: properties doesn't contain property UID = " << propUniqueId ;
		return true;
	}	
	
	/* Skip storage if the value is equals default */
	if (propValue == prop->getDefaultValue())
		return true;
	
	/* Force property save? */
	if (pluginAlertMsg == NULL)
	{
		prop->setValue(propValue);
		refreshLabel();
		
		return true;
	} 
	else 
	{
		/* Select property */
		if(prop->getId() == "address") 
		{
			/* Check the network */
			if(!NetworkAddress::validateIp(propValue))
			{
				/* set a warning message */
				pluginAlertMsg->append("Invalid IP address");
			}
			else 
			{
				prop->setValue(propValue);
				refreshLabel();
				return true;
			}
		}
		
		if(prop->getId() == "netmask") 
		{
			quint8 cidrNetmask = 0;
			
			/* CIDR notation for the netmask? */
			bool isInt;
			cidrNetmask = propValue.toInt(&isInt);
			
			/* Maybe the netmask is in normal notation? 255.255.0.0 */
			if(!isInt)
			{
				QHostAddress netmask(propValue);

				if(!NetworkAddress::validateNetmask(netmask))
				{
					/* Show a warning message */
					pluginAlertMsg->append("Invalid netmask");
				}
				else //Netmask ok
				{
					prop->setValue(propValue);
					refreshLabel();
					return true;
				}
			}
			else
			{
				/* Validate cidr netmask */
				if(cidrNetmask > 32 || cidrNetmask < 1)			
				{
					/* Show a warning message */
					pluginAlertMsg->append("Invalid netmask.\nIn the CIDR notation the netmask has to be included between 1 and 32");
				}
				else	//save the netmask
				{
					prop->setValue(propValue);
					refreshLabel();
					return true;
				}	
			}
		} 
		
		if(prop->getId() == "broadcast") 
		{
			QHostAddress bcast;
			
			/* check if netmask is in cidr notation */
			bool isCidr;
			PluginProperty *netmaskProp = myProxy->getPropertyExpert()->searchPropertiesById(properties, "netmask").at(0);
			PluginProperty *addressProp = myProxy->getPropertyExpert()->searchPropertiesById(properties, "address").at(0);
			
			netmaskProp->getValue().toInt(&isCidr);
			if(isCidr)
			{
				bcast = NetworkAddress::generateBroadcast(
						QHostAddress(addressProp->getValue()), 
						QHostAddress(NetworkAddress::cidr2netmask(netmaskProp->getValue().toInt())));
			}
			else
			{
				bcast = NetworkAddress::generateBroadcast(
						QHostAddress(addressProp->getValue()), 
						QHostAddress(netmaskProp->getValue()));
			}
			
			if (bcast != QHostAddress(propValue))
			{
				pluginAlertMsg->append("generated broadcast (" + bcast.toString() + ") address differs from passed value");
			}
			else 
			{
				prop->setValue(propValue);
				return true;
			}
		}
		return false;
	}
}

/**
 * Set proxy
 */
void PluginIPv4::setProxy(PluginProxy* p)
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
bool PluginIPv4::init(QString laboratoryPath)
{
	/* Get my buddy */
	HardwareInterface *hi = static_cast<HardwareInterface*>(myProxy->getBaseElement());
	if (hi == NULL)
	{
		qWarning() << "PluginIPv4::init(): null hardware interface.";
		return false;
	}
	
	QRegExp ipRegExp(".+\\b" + hi->getName() + "\\b(.+)\\bnetmask\\b(.+)\\bbroadcast\\b(.+\\.[0-9]{1,3})");
	
	/* Parse my startup file and get the mac address if any */
	QString startupPath = laboratoryPath + "/" + hi->getMyVirtualMachine()->getName() + ".startup";
	startupPath.replace("//", "/");		//normalize path
	
	QFile startupFile(startupPath);
	if(!startupFile.exists())
	{
		qWarning() << "PluginIPv4::init(): startup" << startupPath << " file don't exist";
		return false;
	}
	
	if(!startupFile.open(QFile::ReadOnly))
	{
		qWarning() << "PluginIPv4::init(): cannot open" << startupPath << "file in read only mode.";
		return false;
	}
	
	QStringList startupContent = QString(startupFile.readAll()).split("\n", QString::SkipEmptyParts);
	startupFile.close();
	
	bool parseOk = false;
	
	foreach(QString line, startupContent)
	{
		ipRegExp.indexIn(line);
		QStringList capText = ipRegExp.capturedTexts();
		
		//It's my rule
		if(capText[0].contains(hi->getName()))
		{
			QString addressVal = capText[1].trimmed();
			QString netmaskVal = capText[2].trimmed();
			QString broadcastVal = capText[3].trimmed();
			
			myProxy->getPropertyExpert()->searchPropertiesById(properties, "address").at(0)->setValue(addressVal);
			myProxy->getPropertyExpert()->searchPropertiesById(properties, "netmask").at(0)->setValue(netmaskVal);
			myProxy->getPropertyExpert()->searchPropertiesById(properties, "broadcast").at(0)->setValue(broadcastVal);
			parseOk = true;
			break;
		}
	}
	
	if(parseOk)
	{
		NetworkAddress addr(QHostAddress(
			myProxy->getPropertyExpert()->searchPropertiesById(properties, "address").at(0)->getValue()),
				QHostAddress(myProxy->getPropertyExpert()->searchPropertiesById(properties, "netmask").at(0)->getValue()));
		
		myProxy->changeGraphicsLabel(addr.toString(PRINT_CIDR_NETMASK));
		
		return true;
	}
	
	return false;
}

/**
 * [PRIVATE]
 * Refresh the scene label if netmask or ip are changed
 */
void PluginIPv4::refreshLabel()
{
	NetworkAddress addr(QHostAddress(myProxy->getPropertyExpert()->searchPropertiesById(properties, "address").at(0)->getValue()),
				QHostAddress(myProxy->getPropertyExpert()->searchPropertiesById(properties, "netmask").at(0)->getValue()));
	
	QString netmask = myProxy->getPropertyExpert()->searchPropertiesById(properties, "netmask").at(0)->getValue();
	
	bool isInt;
	int cidrNetmask = netmask.toInt(&isInt);
	
	if(isInt)
	{
		addr.setCidrNetmask((quint8)cidrNetmask);
	}
	
	myProxy->changeGraphicsLabel(addr.toString(PRINT_CIDR_NETMASK));
}

/**
 * UNUSED
 */
QPair<PluginProperty*, QString> PluginIPv4::addProperty(QString propertyIdToAdd,
		QString parentPropertyUniqueId)
{
	Q_UNUSED(propertyIdToAdd);
	Q_UNUSED(parentPropertyUniqueId);
	
	return qMakePair(properties.at(0), QObject::tr("You cannot add properties for this plugin"));	//Return never used junk
}
