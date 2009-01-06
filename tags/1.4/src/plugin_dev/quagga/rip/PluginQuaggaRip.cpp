/**
 * The Quagga-rip plugin for Visual Netkit
 * Copyright (C) 2009  Alessio Di Fazio
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

#include "PluginQuaggaRip.h"

/**
 * Constructor
 */
PluginQuaggaRip::PluginQuaggaRip() : PluginInterface()
{
	myProxy = NULL;
}

/**
 * Deconstructor
 */
PluginQuaggaRip::~PluginQuaggaRip()
{
	//NOTE: The proxy is destroyed by the plugin framework
	qDeleteAll(properties);
}

/**
 * Return all templates ready to be saved
 */
QMap<QString, QString> PluginQuaggaRip::getTemplates()
{
	QMap<QString, QString> templates;
	QString templateContent;
	PropertyExpert *pExpert = myProxy->getPropertyExpert();

	//bgpd.conf file
	templateContent = file2string(":/quagga-rip/ripd.conf");

	//replace properties
	templateContent.replace("<HOSTNAME>", pExpert->searchPropertiesById(properties, "hostname").first()->getValue());
	templateContent.replace("<PASSWORD>", pExpert->searchPropertiesById(properties, "password").first()->getValue());

	templates.insert(getTemplateLocation() + "ripd.conf", templateContent);

	return templates;
}

/**
 * [PRIVATE]
 * Returns the path where save/append the template content
 */
QString PluginQuaggaRip::getTemplateLocation(bool getBase)
{
	VirtualMachine *vm = static_cast<VirtualMachine*>(myProxy->getBaseElement());
	if (vm == NULL)
	{
		qWarning() << "PluginQuaggaRip::getTemplateLocation(): null virtualmachine.";
		return QString();
	}

	return QString(vm->getName() + ((getBase)? ".startup" : "/etc/quagga/"));
}

/**
 * If pluginAlertMsg is empty, initializes the passed property propName to propValue.
 */
bool PluginQuaggaRip::saveProperty(QString propUniqueId, QString propValue, QString *pluginAlertMsg)
{
	//Get the property
	PluginProperty *prop = myProxy->getPropertyExpert()->searchProperty(properties, propUniqueId);
	if(!prop)
	{
		qWarning() << "PluginQuaggaRip::saveProperty: properties doesn't contain property UID = " << propUniqueId ;
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

	if(prop->getName() == "hostname")
	{
		if(propValue.trimmed().isEmpty() && pluginAlertMsg)
		{
			pluginAlertMsg->append("The hostname value must be not empty.");
			return false;
		}
		else
		{
			prop->setValue(propValue);
			return true;
		}
	}

	if(prop->getName() == "password")
	{
		prop->setValue(propValue);
		return true;
	}

	return true;
}

/**
 * Set proxy
 */
void PluginQuaggaRip::setProxy(PluginProxy* p)
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
bool PluginQuaggaRip::init(QString laboratoryPath)
{
	PropertyExpert *pExpert = myProxy->getPropertyExpert();
	QString emptyPwd;

	/* Get my buddy */
	VirtualMachine *vm = static_cast<VirtualMachine*>(myProxy->getBaseElement());
	if (vm == NULL)
	{
		qWarning() << "PluginQuaggaRip::init(): null VM";
		return false;
	}

	QString bgpconfFilePath = laboratoryPath + "/" + vm->getName() + "/etc/quagga/ripd.conf";

	/* Parse bgpd.conf file and get the properties */
	QString bgpdFileContent = file2string(bgpconfFilePath);

	if(bgpconfFilePath.isNull())
	{
		qWarning() << "PluginQuaggaBgp::init()" << "bgpd.conf file reading error";
		return false;
	}

	foreach(QString line, bgpdFileContent.split("\n", QString::SkipEmptyParts))
	{
		QStringList words = line.split(" ", QString::SkipEmptyParts);

		//get the hostname
		if(words.size() == 2 && words[0] == "hostname")
			pExpert->searchPropertiesById(properties, "hostname").first()->setValue(words[1]);

		//get the password
		if(words.size() >=1  && words[0] == "password")
			pExpert->searchPropertiesById(properties, "password").first()->setValue(
					(words.size() == 2)? words[1] : emptyPwd );	//pwd may be empty
	}

	//set the graphics label
	myProxy->changeGraphicsLabel(getDefaultGraphisLabel());

	return true;
}

/**
 * [PRIVATE]
 * Read an entire file in a string
 */
QString PluginQuaggaRip::file2string(const QString &filePath, bool *ok)
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
		qWarning() << "PluginQuaggaRip::file2string" << data.errorString();
	}

	return returnVal;
}

/**
 * UNUSED
 */
QPair<PluginProperty*, QString> PluginQuaggaRip::addProperty(QString propertyIdToAdd,
		QString parentPropertyUniqueId)
{
	Q_UNUSED(propertyIdToAdd);
	Q_UNUSED(parentPropertyUniqueId);

	return qMakePair(properties.at(0), QObject::tr("You cannot add properties for this plugin"));	//Return never used junk
}
