/**
 * VisualNetkit is an advanced graphical tool for NetKit <http://www.netkit.org>
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

#include "PropertyExpert.h"
#include <QDomDocument>
#include <QStringList>
#include <QFile>
#include <QDebug>
#include <QRegExp>

/**
 * Constructor
 */
PropertyExpert::PropertyExpert(QString &xmlRes)
{
	xmlPluginRes = xmlRes;
	
	isXmlValid = validateXml();
}

/**
 * Deconstructor
 */
PropertyExpert::~PropertyExpert()
{
}

/**
 * This function build the default plugin's property structure reading
 * infos from xml plugin conf file.
 * Return a list of 'root' plugins and for each of these, build (in depth)
 * all sub-properties that have min-occurrence value >= 1
 */ 
QList<PluginProperty*> PropertyExpert::buildBaseProperties()
{
	QList<PluginProperty*> defaultStructure;
	
	QDomDocument doc = parseDocument();
	
	//properties node exists because the xml is allready validated.
	QDomElement properties = doc.elementsByTagName("properties").at(0).toElement();
	
	QDomNodeList rootProperties = properties.childNodes();
	
	for(int i=0; i<rootProperties.size(); i++)
	{
		// *p is NULL if node "min" attribute is less then one
		PluginProperty *p = buildPropertyDepth(rootProperties.at(i).toElement());
		if(p)
			defaultStructure << p;
	}
	
	return defaultStructure;
}

/**
 * [PRIVATE]
 * Build a property starting by its <property/> node, and recursivelly build
 * all sub properties.
 */
PluginProperty* PropertyExpert::buildPropertyDepth(QDomElement propNode, PluginProperty* parent,
		PluginProperty* current)
{
	PluginProperty* prop = NULL;
	
	//CDATA section must be the first child
	QDomCDATASection propertyDescr = propNode.childNodes().at(0).toCDATASection();
	
	if(propNode.attribute("min").toInt() > 0)
	{
		//Create the new property (node -> property)
		prop = new PluginProperty(
			propNode.attribute("name"), propNode.attribute("default_value"),
			propertyDescr.data(), propNode.attribute("id"),
			propNode.attribute("min").toInt(), propNode.attribute("max").toInt());
	}
	
	if(prop)
	{		
		//append the new property to the parent
		if(current)
		{
			current->appendChild(prop);
			prop->setParent(current);
		}
			
		QDomNodeList propChilds = propNode.childNodes();
		QDomNodeList childProperties;
		
		//Find <childs/> node 
		for(int i=0; i<propChilds.size(); i++)
		{
			if(propChilds.at(i).toElement().tagName() == "childs")
			{
				childProperties = propChilds.at(i).toElement().childNodes();
				break;
			}
		}
		
		//Scan all sub-properties
		for(int j=0; j<childProperties.size(); j++)
		{
			buildPropertyDepth(childProperties.at(j).toElement(), 
					(parent)? parent : prop, prop);
		}
			
	}
	
	return prop;
}

/**
 * Parse the xml file and get a mag key->value that contains info
 * about a property
 * QMap (
 * 		"name"			-> "name",
 * 		"id"			-> "xxx-01",
 * 		"default"		-> "default",
 * 		"min"			-> "1",
 * 		"max"			-> "3",
 * 		"description"	-> "mumble mumble mumble..."
 * 		)
 */
QMap<QString, QString> PropertyExpert::getPropertyInfo(QString propertyId)
{
	QMap<QString, QString> propInfo;
	bool found = false;
	
	QDomDocument doc = parseDocument();
	
	QDomNodeList propList = doc.elementsByTagName("property");
	for(int i=0; i<propList.size(); i++)
	{
		if(propList.at(i).toElement().attribute("id") == propertyId)
		{
			QDomElement p = propList.at(i).toElement();
			
			propInfo.insert("name", p.attribute("name"));
			propInfo.insert("id", p.attribute("id"));
			propInfo.insert("default", p.attribute("default_value"));
			propInfo.insert("min", p.attribute("min"));
			propInfo.insert("max", p.attribute("max"));
			
			//CDATA section must be the first child
			QDomCDATASection propertyDescr = p.childNodes().at(0).toCDATASection();
			propInfo.insert("description", propertyDescr.data());
			found = true;
			break;
		}
	}
	
	if(!found) qWarning() << "PropertyInfo: property with id=" << propertyId << "not found.";
	
	return propInfo;
}

/**
 * Parse the xml file and return a map of key->value that contains info
 * of <global />
 * 
 * QMap (
 * 		"plugin name"	-> "name",
 * 		"type"			-> "vm",
 * 		"version"		-> "1.0",
 * 		"author"		-> "some authors",
 * 		"dep"			-> "plugin1",			(Multi-Map)
 * 		"dep"			-> "plugin2",
 * 		...
 * 		"description"	-> "mumble mumble mumble"
 * 		)
 */ 
QMap<QString, QString> PropertyExpert::parseXmlGlobalInfo()
{
	QMap<QString, QString> globalInfo;

	QDomDocument doc = parseDocument();
	
	/* Get plugin name */
	globalInfo.insert("plugin name",
			doc.elementsByTagName("plugin").at(0).toElement().attribute("name"));
	
	/* Get global infos */
	QDomElement global = doc.elementsByTagName("global").at(0).toElement();
	
	globalInfo.insert("type", global.attribute("type"));
	globalInfo.insert("version", global.attribute("version"));
	globalInfo.insert("author", global.attribute("author"));
	
	//plit dependencies
	QStringList deps = global.attribute("dependencies").split(",", QString::SkipEmptyParts);
	for(int i=0; i<deps.size(); i++)	//trim all
	{
		QString trimmed = deps.takeAt(i).trimmed();
		globalInfo.insertMulti("dep", trimmed);
	}
	
	globalInfo.insert("description", global.text());
	
	return globalInfo;
	
}

/**
 * [PRIVATE]
 * XML document creator
 */
QDomDocument PropertyExpert::parseDocument()
{
	QDomDocument doc;
	QString errorMsg;
	int errorLine, errorColumn;
	QFile xmlFile(xmlPluginRes);
	
	if(!xmlFile.open(QFile::ReadOnly))
	{
		qDebug() << "Cannot open XML resource file:" << xmlPluginRes << xmlFile.errorString();
		return doc;
	}

	if (!doc.setContent(&xmlFile, &errorMsg, &errorLine, &errorColumn))
	{	
		qWarning()	<< "Plugin Xml file invalid."
					<< "Error:"		<< errorMsg
					<< "Line:"		<< errorLine
					<< "Column:"	<< errorColumn;
	}
	
	xmlFile.close();
	
	return doc;
}

/**
 * [PRIVATE]
 * Validate the xml plugin config file
 */
bool PropertyExpert::validateXml()
{
	QDomDocument doc;
	QString errorMsg;
	int errorLine, errorColumn;
	QFile xmlFile(xmlPluginRes);
	
	if(!xmlFile.open(QFile::ReadOnly))
	{
		qDebug() << "Cannot open XML resource file:" << xmlPluginRes << xmlFile.errorString();
		return false;
	}

	if (!doc.setContent(&xmlFile, &errorMsg, &errorLine, &errorColumn))
	{	
		qWarning()	<< "Plugin Xml file invalid."
					<< "Error:"		<< errorMsg
					<< "Line:"		<< errorLine
					<< "Column:"	<< errorColumn;
		
		return false;
	}
	
	xmlFile.close();
	
	//validate root element
	QDomElement root = doc.childNodes().at(0).toElement();
	if(root.tagName() != "plugin")
	{
		qWarning() << "No <plugin/> root node.";
		return false;
	}
	
	//root attribute
	if(!root.hasAttribute("name"))
	{
		qWarning() << "<plugin/> root node not valid.";
		return false;
	}
	
	//validate global infos
	QDomElement global = root.elementsByTagName("global").at(0).toElement();
	if(global.tagName() != "global")
	{
		qWarning() << "No <global/> node.";
		return false;
	}
	
	//validate global attributes
	if(!(global.hasAttribute("type") && global.hasAttribute("version") && global.hasAttribute("author") && 
			global.hasAttribute("dependencies")))
	{
		qWarning() << "<global/> node not valid.";
		return false;
	}
	
	//validate type; must be 'vm' or 'cd' or 'link'
	QString type = global.attribute("type");
	QRegExp typeValidator("vm|cd|link");
	
	if(!typeValidator.exactMatch(type))
	{
		qWarning() << "<plugin/> type must is 'vm', 'cd' or 'link'";
		return false;
	}
	
	//validate properties
	QDomElement properties = root.elementsByTagName("properties").at(0).toElement();
	if(properties.tagName() != "properties")
	{
		qWarning() << "No <properties/> node.";
		return false;
	}
	
	//validate each property
	QDomNodeList pList = properties.elementsByTagName("property");
	QStringList ids;
	for(int i=0; i<pList.size(); i++)
	{
		QDomElement property = pList.at(i).toElement();
		
		//validate attributes
		if(!(property.hasAttribute("id") && property.hasAttribute("name") &&
				property.hasAttribute("default_value") && property.hasAttribute("min") &&
				property.hasAttribute("max")))
		{
			qWarning() << "<property/> node not valid";
			return false;
		}
		
		//validate min value
		if(property.attribute("min") != "0" && property.attribute("min") != "1")
		{
			qWarning() << "<property/> min value must be equals to '0' or '1'";
			return false;
		}
		
		//validate unique id
		if(ids.contains(property.toElement().attribute("id")))
		{
			qWarning() << "<property/> node doesn't have an unique ID:" << property.toElement().attribute("id");
			return false;
		}
		
		ids << property.toElement().attribute("id");	//save unes id
	}
	
	return true;	//validate
}

/**
 * Return a list of pairs <name, id> of all childs of a parent identified by its ID
 */
QList< QPair<QString, QString> > PropertyExpert::getChildsByParentId(QString id)
{
	QList < QPair<QString, QString> > childDescr;
	
	QDomDocument doc = parseDocument();
	
	QDomElement parent;
	QDomNodeList properties = doc.elementsByTagName("property");
	
	//search the property with id = `id`
	for(int l=0; l<properties.size(); l++)
	{
		QDomElement node = properties.at(l).toElement();
		if(node.attribute("id") == id)
		{
			parent = node;
			break;
		}
	}
	
	if(!parent.isNull())
	{
		QDomNodeList childs = parent.childNodes();
		for(int i=0; i<childs.size(); i++)
		{
			if(childs.at(i).toElement().tagName() == "childs")
			{
				QDomNodeList propChilds = childs.at(i).toElement().childNodes();
				for(int j=0; j<propChilds.size(); j++)
				{
					QPair<QString, QString> descr(
							propChilds.at(j).toElement().attribute("name"),
							propChilds.at(j).toElement().attribute("id"));
					childDescr << descr;
				}
				break;
			}
		}
	}
	
	
	return childDescr;
}

/**
 * Convenient function
 */
QList< QPair<QString, QString> > PropertyExpert::getRootChilds()
{
	QList < QPair<QString, QString> > childDescr;
	
	QDomDocument doc = parseDocument();
	
	QDomElement root = doc.elementsByTagName("properties").at(0).toElement();
	
	if(!root.isNull())
	{
		QDomNodeList childs = root.childNodes();
		for(int i=0; i<childs.size(); i++)
		{
			
			QPair<QString, QString> descr(
				childs.at(i).toElement().attribute("name"),
				childs.at(i).toElement().attribute("id"));
			childDescr << descr;
		}
	}
	
	
	return childDescr;

}

/**
 * Usefull function to search fast a property by its id and copy number
 * Can return NULL if the property doesn't exists.
 */
PluginProperty* PropertyExpert::searchProperty(
		QList<PluginProperty*> properties, QString pId, quint16 pCopy)
{
	PluginProperty *prop = NULL;
	
	foreach(PluginProperty* p, properties)
	{
		PluginProperty *ret = searchPropertyPrivate(p, pId, pCopy);
		if(ret)
		{
			prop = ret;
			break;
		}
	}
	
	return prop;
}

/**
 * [PRIVATE]
 * Recursive search
 */
PluginProperty * PropertyExpert::searchPropertyPrivate(PluginProperty *parent, QString pId, quint16 pCopy)
{
	if(parent->getId() == pId && parent->getCopy() == pCopy)
		return parent;
	
	PluginProperty *ret = NULL;
	foreach(PluginProperty* p, parent->getChilds())
	{
		ret = searchPropertyPrivate(p, pId, pCopy);
		if(ret)
			break;
	}
	
	return ret;
}

/**
 * Property factory
 */
PluginProperty* PropertyExpert::newProperty(QString propName, QString defValue, QString desc,
			QString idVal, quint16 minOcc, quint16 maxOcc)
{
	return new PluginProperty(propName, defValue, desc, idVal, minOcc, maxOcc);
}
