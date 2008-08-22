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
 * If pluginAlertMsg is empty, initializes the passed property UID to propValue. 
 */
bool PluginTest::saveProperty(QString propUniqueId, QString propValue, QString *pluginAlertMsg)
{
	if(pluginAlertMsg)
	{
		pluginAlertMsg->append("Plugin Test property error (allways showed... just for testing).");
		return false;
	}
		
	PluginProperty* p = myProxy->getPropertyExpert()->searchProperty(properties, propUniqueId);
	if(!p)
	{
		pluginAlertMsg->append("Property not found :( [UID=" + propUniqueId + "]");
		return false;
	}
	
	p->setValue(propValue);
	
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
QString PluginTest::deleteProperty(QString propertyUniqueId)
{
	PluginProperty *propToDelete =
		myProxy->getPropertyExpert()->searchProperty(properties, propertyUniqueId);
	
	if(!propToDelete)
	{
		qWarning()	<< "PluginTest::deleteProperty" << "property not found: uid ="
					<< propertyUniqueId; 
		return QString("Property not found. Maybe it is a program BUG!\nUID+" + propertyUniqueId);
	}
	
	/* Check min occurrence */
	quint16 idCount = 0;
	if(propToDelete->getParent())
	{
		foreach(PluginProperty *pp, propToDelete->getParent()->getChilds())
			if(pp->getId() == propToDelete->getId())	idCount++;
	}
	else
	{
		foreach(PluginProperty *pp, properties)
			if(pp->getId() == propToDelete->getId())	idCount++;
	}
	
	if(propToDelete->getMinOcc() == 1 && idCount == 1)
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
QPair<PluginProperty*, QString> PluginTest::addProperty(QString propertyIdToAdd, QString parentPropertyUniqueId)
{
	QPair<PluginProperty*, QString> retVal;
	retVal.first = NULL;
	
	/* Get the parent property */
	PluginProperty *propParent = NULL;
	
	propParent = myProxy->getPropertyExpert()->searchProperty(properties, parentPropertyUniqueId);
	
	/* Is a root property? Read the xml description*/
	bool isRoot = false;
	
	// pair: <name, id>
	QList< QPair<QString, QString> > allRootProperties = myProxy->getPropertyExpert()->getRootChilds();
	for(int i=0; i<allRootProperties.size(); i++)
		if((isRoot = (allRootProperties.at(i).second == propertyIdToAdd))) break;
	
	if(!propParent && !isRoot)
	{
		qWarning()	<< "PluginTest::addProperty() property parent unknown."
					<< "uid =" << parentPropertyUniqueId;
		
		retVal.second.append("ERROR: Unknown parent property");
		
		return retVal;
	}

	/* Get infos about the new property */
	QMap<QString, QString> childInfo = myProxy->getPropertyExpert()->getPropertyInfo(propertyIdToAdd);
	
	if(childInfo.size() == 0)
	{
		retVal.second.append("ERROR: Cannot find property infos inside XML plugin Test config file");
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
	
	addPropertyChildsDeeply(newProp);
		
	return retVal;
}

/**
 * [PRIVATE]
 * Add child properties reading these on XML conf file through property expert.
 * Recursive.
 */
void PluginTest::addPropertyChildsDeeply(PluginProperty* prop)
{
	if(!prop)
		return;
	
	// pair: <child name, child id>
	QList< QPair<QString, QString> > childsInfo =
		myProxy->getPropertyExpert()->getChildsByParentId(prop->getId());
		
	/* 
	 * Foreach child that have minOcc = 1 i have to create a property child, and
	 * recursively add childs of childs ...
	 */
	for(int i=0; i<childsInfo.size(); i++)
	{
		/* first, get node info from XML
		 * QMap (
		 * 		"name"			-> "name",
		 * 		"id"			-> "xxx-01",
		 * 		"default"		-> "default",
		 * 		"min"			-> "1",
		 * 		"max"			-> "3",
		 * 		"description"	-> "mumble mumble mumble..."
		 * 		)
		 */
		QMap<QString, QString> nodeInfo =
			myProxy->getPropertyExpert()->getPropertyInfo(childsInfo.at(i).second);
		
		if(nodeInfo["min"].toInt() != 1)
			continue;
		
		//make the new child
		PluginProperty *childProp = myProxy->getPropertyExpert()->newProperty(
			nodeInfo["name"], nodeInfo["default"], 
			nodeInfo["description"], nodeInfo["id"], 
			(quint16)nodeInfo["min"].toInt(), (quint16)nodeInfo["max"].toInt());
		
		//attach this child and recursively recall this function on this prop.
		prop->appendChild(childProp);
		childProp->setParent(prop);
		
		addPropertyChildsDeeply(childProp);
	}
}
