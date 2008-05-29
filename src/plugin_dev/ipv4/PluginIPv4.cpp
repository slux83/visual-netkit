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

#include "PluginIPv4.h"
#include "HardwareInterface.h"

/** 
 * Constructor 
 */
PluginIPv4::PluginIPv4() : PluginInterface()
{
	mySettings = new QSettings(":/iniipv4", QSettings::NativeFormat);
	
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
PluginIPv4::~PluginIPv4()
{
	//NOTE: The proxy is destroyed by the plugin framework
	qDeleteAll(properties);
	delete mySettings;
}

/**
 * Returns the plugin template if resource file exists,
 * otherwise returns an empty QString.
 */
QString PluginIPv4::getTemplate()
{
	QString templateContent;
	
	QFile data(":/basic_tpl");
	if (data.open(QFile::ReadOnly)) 
	{
		QTextStream in(&data);
		templateContent = in.readAll();
		
		HardwareInterface *hi = dynamic_cast<HardwareInterface*>(myProxy->getBaseElement());
		(hi != NULL)? templateContent.replace("HI", hi->getName()) : templateContent.replace("HI", "null");
		
		QString status;
		(hi->getState())? status = "up" : status = "down";
		templateContent.replace("HI_STATE", status);
		
		PluginProperty *pp = properties.value("address");
		templateContent.replace("IP", pp->getValue());
		
		pp = properties.value("netmask");
		templateContent.replace("NETMASK", pp->getValue());
		
		pp = properties.value("broadcast");
		templateContent.replace("BROADCAST", pp->getValue());
	}
	else
	{
		qWarning() << "The plugin getTemplate() failed:" << data.errorString();
	}
	
	qDebug() << endl << "====== IPv4 TEMPLATE ==============" << endl << templateContent << endl;
		
	return templateContent;
}

/**
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
	return QString(vm+".startup");
}

/**
 * Fetches plugin properties and stores them in the properties map.
 */
bool PluginIPv4::fetchProperties()
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
bool PluginIPv4::saveProperty(QString propName, QString propValue, QString *pluginAlertMsg)
{
	// se non esiste una property di nome propName
	if (!properties.contains(propName))
	{
		qWarning() << "PluginIPv4::initProperty: properties doesn't contain property" + propName;
		return true;
	}	
	
	// se il valore della property propName è uguale a quello di default
	if (propValue == properties.value(propName)->getDefaultValue())
		return true;
	
	// se voglio forzare il salvataggio della property
	if (pluginAlertMsg == NULL)
	{
		PluginProperty *prop = properties.value(propName);
		prop->setValue(propValue);
		refreshLabel();
		
		return true;
	} 
	// altrimenti, se voglio effettuare i controlli sui valori inseriti
	else 
	{
		// controllo il valore relativo alla property address
		if (propName == "address") 
		{
			/* Check the network */
			if(!NetworkAddress::validateIp(propValue))
			{
				/* set a warning message */
				pluginAlertMsg->append("Invalid IP address");
			}
			else 
			{
				PluginProperty *prop = properties.value(propName);
				prop->setValue(propValue);
				refreshLabel();
				return true;
			}
		}
		// controllo il valore relativo alla property netmask
		else if (propName == "netmask") 
		{
			quint8 cidrNetmask = 0;
			
			/* CIDR notation for the netmask? */
			bool isInt;
			cidrNetmask = propValue.toInt(&isInt);
			
			/* Maybe the netmask is in normal notation? 255.255.0.0 */
			if(!isInt)
			{
				QHostAddress netmask(propValue);
				qDebug() << netmask << propValue;
				if(!NetworkAddress::validateNetmask(netmask))
				{
					/* Show a warning message */
					pluginAlertMsg->append("Invalid netmask");
				}
				else //Netmask ok
				{
					PluginProperty *prop = properties.value(propName);
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
					PluginProperty *prop = properties.value(propName);
					prop->setValue(propValue);
					refreshLabel();
					return true;
				}	
			}
		} 
		// controllo il valore relativo alla property broadcast
		else if (propName == "broadcast") 
		{
			QHostAddress bcast;
			
			/* check if netmask is in cidr notation */
			bool isCidr;
			properties.value("netmask")->getValue().toInt(&isCidr);
			if(isCidr)
			{
				bcast = NetworkAddress::generateBroadcast(
						QHostAddress(properties.value("address")->getValue()), 
						QHostAddress(NetworkAddress::cidr2netmask(properties.value("netmask")->getValue().toInt())));
			}
			else
			{
				bcast = NetworkAddress::generateBroadcast(
						QHostAddress(properties.value("address")->getValue()), 
						QHostAddress(properties.value("netmask")->getValue()));
			}
			
			if (bcast != QHostAddress(propValue))
			{
				pluginAlertMsg->append("generated broadcast (" + bcast.toString() + ") address differs from passed value");
			}
			else 
			{
				PluginProperty *prop = properties.value(propName);
				prop->setValue(propValue);
				return true;
			}
		}
		return false;
	}
}

/**
 * [PRIVATE]
 * Refresh the scene label if netmask or ip are changed
 */
void PluginIPv4::refreshLabel()
{
	myProxy->changeGraphicsLabel(
			properties["address"]->getValue() + "/" + properties["netmask"]->getValue());
}

