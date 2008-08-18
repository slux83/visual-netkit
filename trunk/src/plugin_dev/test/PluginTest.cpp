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

#include "PluginTest.h"

/** 
 * Constructor 
 */
PluginTest::PluginTest() : PluginInterface()
{	
	myProxy = NULL;
}

/**
 * Deconstructor
 */
PluginTest::~PluginTest()
{
	//NOTE: The proxy is destroyed by the plugin framework
	
	qDeleteAll(properties);
	properties.clear();
}

/**
 * Returns the plugin template if resource file exists,
 * otherwise returns an empty QString.
 */
QMap<QString, QString> PluginTest::getTemplates()
{
	QMap<QString, QString> templates;
	QString templateContent;
	
	QFile data(":/test/basic-template");
	if (data.open(QFile::ReadOnly)) 
	{
		QTextStream in(&data);
		templateContent = in.readAll();
	}
	else
	{
		qWarning() << "The plugin getTemplate() failed:" << data.errorString();
	}
	
	templates.insert(getTemplateLocation(), templateContent);
	
	return templates;
}

/**
 * [PRIVATE]
 * Returns the path where save/append the template content
 */
QString PluginTest::getTemplateLocation()
{
	VirtualMachine *vm = static_cast<VirtualMachine*>(myProxy->getBaseElement());
	if (vm == NULL)
	{
		qWarning() << "Plugin::getTemplateLocation(): null VirtualMachine name.";
		return QString();
	}
	
	return QString(vm->getName() + "/etc/basic.conf");
}

/**
 * If pluginAlertMsg is empty, initializes the passed property propName to propValue. 
 */
bool PluginTest::saveProperty(QString propId, QString propValue, QString *pluginAlertMsg)
{
	Q_UNUSED(propId)
	Q_UNUSED(pluginAlertMsg)
	myProxy->changeGraphicsLabel(propValue);
	
	qDebug() << "PluginTest: saving" << propId << propValue;
	
	return true;
}

/**
 * Set 'my' proxy and get my info/properties
 */
void PluginTest::setProxy(PluginProxy* p)
{
	myProxy = p;
	
	//Now we can get plugin infos and base properties from property expert
	if(p->getPropertyExpert()->isXmlConfValid())
	{
		globalInfo = p->getPropertyExpert()->parseXmlGlobalInfo();
		properties = p->getPropertyExpert()->buildBaseProperties();
		//printProperties();
	}
}

/**
 * [PRIVATE]
 * A private function just for testing properties Tree structure
 */
void PluginTest::printProperties(PluginProperty* current)
{
	if(!current)
	{
		foreach(PluginProperty* prop, properties)
		{
			printProperties(prop);
		}
	}
	else
	{
		qDebug() << current->getName() << "id =" << current->getId() << "copy =" << current->getCopy() << current << "childs:" << current->getChilds();
		foreach(PluginProperty* prop, current->getChilds())
		{
			printProperties(prop);
		}
	}
}

/**
 * Delete a proprety
 */
QString PluginTest::deleteProperty(QString propertyId, quint16 propertyCopy)
{
	PluginProperty *propToDelete =
		myProxy->getPropertyExpert()->searchProperty(properties, propertyId, propertyCopy);
	
	if(!propToDelete)
	{
		qWarning()	<< "PluginTest::deleteProperty" << "property not found: id ="
					<< propertyId << "copy =" << propertyCopy; 
		return QString("Property not found. Maybe it is a program BUG!\nID+" + propertyId + " COPY=" + propertyCopy);
	}
	
	if(propToDelete->getMaxOcc() == 1)
		return QString("Cannot deleted properties with MIN occurrence equals to 1.");
	
	if(!propToDelete->getParent())
	{
		//i can delete the property directly from root properties
		properties.removeAll(propToDelete);
		delete propToDelete;
	}
	else
	{
		//i have to delete the property from its parent
		if(!propToDelete->getParent()->destroyChild(propToDelete))
			return QString("ERROR: There was a property destroy error. Maybe it is a program BUG!");
	}
	
	return QString();
}

/**
 * Add a property
 */
QPair<PluginProperty*, QString> PluginTest::addProperty(QString propertyIdToAdd, QString parentPropertyId,
			quint16 parentPropertyCopy)
{
	QPair<PluginProperty*, QString> retVal;
	retVal.first = NULL;
	
	/* Get the parent property */
	PluginProperty *propParent = NULL;
	
	propParent = myProxy->getPropertyExpert()->searchProperty(properties, parentPropertyId, parentPropertyCopy);
	
	/* Is a root property? Read the xml description*/
	bool isRoot = false;
	
	// pair: <name, id>
	QList< QPair<QString, QString> > allRootProperties = myProxy->getPropertyExpert()->getRootChilds();
	for(int i=0; i<allRootProperties.size(); i++)
		if((isRoot = (allRootProperties.at(i).second == propertyIdToAdd))) break;
	
	if(!propParent && !isRoot)
	{
		qWarning()	<< "PluginTest::addProperty() property parent unknown."
					<< "id =" << parentPropertyId
					<< "copy =" << parentPropertyCopy;
		
		retVal.second.append("ERROR: Unknown parent property");
		
		return retVal;
	}

	/* Get infos about the new property */
	QMap<QString, QString> childInfo = myProxy->getPropertyExpert()->getPropertyInfo(propertyIdToAdd);
	
	if(childInfo.size() == 0)
	{
		retVal.second.append("ERROR: Cannot find property infos");
		return retVal;
	}
	
	quint16 maxOcc = (quint16)childInfo["max"].toInt();
	quint16 childIdCount = 0;
	
	if(propParent)
		/* Count propParent childs with id = propertyIdToAdd, and control max occurrence */
		foreach(PluginProperty *p, propParent->getChilds())
		{
			if(p->getId() == propertyIdToAdd)
				childIdCount++;
		}
	else
		foreach(PluginProperty *p, properties)
		{
			if(p->getId() == propertyIdToAdd)
				childIdCount++;
		}
	
	if(childIdCount == maxOcc)
	{
		qWarning() << "PluginTest::addProperty() reached max occurrence of property with id =" << propertyIdToAdd;
		retVal.second.append("You can't add more properties of this type; max " + QString::number(maxOcc) + " copies");
		return retVal;
	}
	
	PluginProperty *newProp = myProxy->getPropertyExpert()->newProperty(
			childInfo["name"], childInfo["default"], 
			childInfo["description"], childInfo["id"], 
			(quint16)childInfo["min"].toInt(), (quint16)childInfo["max"].toInt());
	
	newProp->setCopy(childIdCount);
	
	if(propParent)
	{
		propParent->appendChild(newProp);
		newProp->setParent(propParent);
	}
	else
		properties << newProp;
	
	retVal.first = newProp;
	
	return retVal;
}


