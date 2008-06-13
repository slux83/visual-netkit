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

#include "XMLParser.h"

/**
 * Constructor
 */
XMLParser::XMLParser()
{
}

/**
 * Deconstructor
 */
XMLParser::~XMLParser()
{
}

/**
 * [STATIC-PUBLIC]
 * Return the size of the scene
 * error string is setted if an error is verified
 */
QRectF XMLParser::getSceneSize(QString labPath, QString *error)
{
	QString errXml;
	QDomDocument doc = XMLExpert::readDocument(labPath, &errXml);
	QRectF sceneSize;	//the return value
	
	//validate dom
	if(!errXml.isEmpty())
	{
		error->append(errXml);
		return sceneSize;
	}
	
	QDomElement dimension = 
		doc.elementsByTagName("lab").at(0).toElement().
			elementsByTagName("scene").at(0).toElement().
				elementsByTagName("dimension").at(0).toElement();
	
	if(!dimension.hasAttribute("width") || !dimension.hasAttribute("height"))
	{
		if(error != NULL)
			error->append("Unvalid lab.xml: dimension node is empty or not correct.");
		
		return sceneSize;
	}
	
	sceneSize.setWidth(dimension.attribute("width").toInt());
	sceneSize.setHeight(dimension.attribute("height").toInt());	
	
	return sceneSize;

}

/**
 * [STATIC-PUBLIC]
 * Get the vm position
 * If someting goes wrong, the error arg is setted
 */
QPointF XMLParser::getVmPosition(QString vmName, QString labPath, QString *error)
{
	QDomDocument doc = XMLExpert::readDocument(labPath);
	QPointF vmPos;	//the return value
	QDomNodeList nodeList = doc.elementsByTagName("virtualmachine");
	
	for(int i=0; i<nodeList.size(); i++)
	{
		if(nodeList.at(i).toElement().hasAttribute("id"))
		{
			if(nodeList.at(i).toElement().attribute("id") == vmName)
			{
				//ok, get x and y coordinates, but before check consistency
				if(!nodeList.at(i).toElement().hasAttribute("x") || !nodeList.at(i).toElement().hasAttribute("y"))
				{
					if(error != NULL)
						error->append("Unvalid lab.xml: node virtualmachine").append(" ID=").append(vmName).append(" ").append("not standard.");
					return vmPos;
				}
				
				vmPos.setX(nodeList.at(i).toElement().attribute("x").toFloat());
				vmPos.setY(nodeList.at(i).toElement().attribute("y").toFloat());
				break;
			}
		}
		else
		{
			if(error != NULL)
				error->append("Unvalid lab.xml: none ID attribute for node virtualmachine").append(" ID=").append(vmName);
		}
	}
	
	return vmPos;
}

/**
 * [STATIC-PUBLIC]
 * Get the cd position
 * If someting goes wrong, the error arg is setted
 */
QPointF XMLParser::getCdPosition(QString cdName, QString labPath, QString *error)
{
	QDomDocument doc = XMLExpert::readDocument(labPath);
	QPointF cdPos;	//the return value
	QDomNodeList nodeList = doc.elementsByTagName("collisiondomain");
	
	for(int i=0; i<nodeList.size(); i++)
	{
		if(nodeList.at(i).toElement().hasAttribute("id"))
		{
			if(nodeList.at(i).toElement().attribute("id") == cdName)
			{
				//ok, get x and y coordinates, but before check consistency
				if(!nodeList.at(i).toElement().hasAttribute("x") || !nodeList.at(i).toElement().hasAttribute("y"))
				{
					if(error != NULL)
						error->append("Unvalid lab.xml: node collisiondomain").append(" ID=").append(cdName).append(" ").append("not standard.");
					return cdPos;
				}
				
				cdPos.setX(nodeList.at(i).toElement().attribute("x").toFloat());
				cdPos.setY(nodeList.at(i).toElement().attribute("y").toFloat());
				break;
			}
		}
		else
		{
			if(error != NULL)
				error->append("Unvalid lab.xml: none ID attribute for node collisiondomain").append(" ID=").append(cdName);
		}
	}
	
	return cdPos;
}

/**
 * [STATIC-PUBLIC]
 * Get the cd label position
 * If someting goes wrong, the error arg is setted
 */
QPointF XMLParser::getCdLabelPosition(QString cdName, QString labPath, QString *error)
{
	QDomDocument doc = XMLExpert::readDocument(labPath);
	QPointF pos;	//the return value
	QDomNodeList nodeList = doc.elementsByTagName("collisiondomain");
	for(int i=0; i<nodeList.size(); i++)
	{
		if(nodeList.at(i).toElement().hasAttribute("id"))
		{
			if(nodeList.at(i).toElement().attribute("id") == cdName)
			{
				QDomElement labelNode = 
					nodeList.at(i).toElement().elementsByTagName("label").at(0).toElement();
				
				if(labelNode.isNull())
				{
					if(error != NULL)
						error->append("Unvalid lab.xml: none label not found inside the collision domain node with").append(" ID=").append(cdName);
				}
				else
				{
					//ok, get x and y coordinates, but before check consistency
					if(!labelNode.hasAttribute("x") || !labelNode.hasAttribute("y"))
					{
						if(error != NULL)
							error->append("Unvalid lab.xml: node label inside collisiondomain").append(" ID=").append(cdName).append(" ").append("not standard.");
						return pos;
					}
					
					pos.setX(labelNode.attribute("x").toFloat());
					pos.setY(labelNode.attribute("y").toFloat());					
				}
			}
		}
		else
		{
			if(error != NULL)
				error->append("Unvalid lab.xml: none ID attribute for node collisiondomain").append(" ID=").append(cdName);
		}
	}
	
	return pos;
}


/**
 * [STATIC-PUBLIC]
 * Get the cd shared area position
 * If someting goes wrong, the error arg is setted
 */
QPointF XMLParser::getCdPluginsAreaPosition(QString cdName, QString labPath, QString *error)
{
	QDomDocument doc = XMLExpert::readDocument(labPath);
	QPointF pos;	//the return value
	QDomNodeList nodeList = doc.elementsByTagName("collisiondomain");
	for(int i=0; i<nodeList.size(); i++)
	{
		if(nodeList.at(i).toElement().hasAttribute("id"))
		{
			if(nodeList.at(i).toElement().attribute("id") == cdName)
			{
				QDomElement labelNode = 
					nodeList.at(i).toElement().elementsByTagName("multilabel").at(0).toElement();
				
				if(labelNode.isNull())
				{
					if(error != NULL)
						error->append("Unvalid lab.xml: none multi-label not found inside the collision domain node with").append(" ID=").append(cdName);
				}
				else
				{
					//ok, get x and y coordinates, but before check consistency
					if(!labelNode.hasAttribute("x") || !labelNode.hasAttribute("y"))
					{
						if(error != NULL)
							error->append("Unvalid lab.xml: node multi-label inside collisiondomain").append(" ID=").append(cdName).append(" ").append("not standard.");
						return pos;
					}
					
					pos.setX(labelNode.attribute("x").toFloat());
					pos.setY(labelNode.attribute("y").toFloat());					
				}
			}
		}
		else
		{
			if(error != NULL)
				error->append("Unvalid lab.xml: none ID attribute for node collisiondomain").append(" ID=").append(cdName);
		}
	}
	
	return pos;
}


/**
 * [STATIC-PUBLIC]
 * Get the vm label position
 * If someting goes wrong, the error arg is setted
 */
QPointF XMLParser::getVmLabelPosition(QString vmName, QString labPath, QString *error)
{
	QDomDocument doc = XMLExpert::readDocument(labPath);
	QPointF pos;	//the return value
	QDomNodeList nodeList = doc.elementsByTagName("virtualmachine");
	for(int i=0; i<nodeList.size(); i++)
	{
		if(nodeList.at(i).toElement().hasAttribute("id"))
		{
			if(nodeList.at(i).toElement().attribute("id") == vmName)
			{
				QDomElement labelNode = 
					nodeList.at(i).toElement().elementsByTagName("label").at(0).toElement();
				
				if(labelNode.isNull())
				{
					if(error != NULL)
						error->append("Unvalid lab.xml: none label not found inside the virtual machine node with").append(" ID=").append(vmName);
				}
				else
				{
					//ok, get x and y coordinates, but before check consistency
					if(!labelNode.hasAttribute("x") || !labelNode.hasAttribute("y"))
					{
						if(error != NULL)
							error->append("Unvalid lab.xml: node label inside virtual machine").append(" ID=").append(vmName).append(" ").append("not standard.");
						return pos;
					}
					
					pos.setX(labelNode.attribute("x").toFloat());
					pos.setY(labelNode.attribute("y").toFloat());					
				}
			}
		}
		else
		{
			if(error != NULL)
				error->append("Unvalid lab.xml: none ID attribute for node virtual machine").append(" ID=").append(vmName);
		}
	}
	
	return pos;
}

/**
 * [STATIC-PUBLIC]
 * Get the vm shared area position
 * If someting goes wrong, the error arg is setted
 */
QPointF XMLParser::getVmPluginsAreaPosition(QString vmName, QString labPath, QString *error)
{
	QDomDocument doc = XMLExpert::readDocument(labPath);
	QPointF pos;	//the return value
	QDomNodeList nodeList = doc.elementsByTagName("virtualmachine");
	for(int i=0; i<nodeList.size(); i++)
	{
		if(nodeList.at(i).toElement().hasAttribute("id"))
		{
			if(nodeList.at(i).toElement().attribute("id") == vmName)
			{
				QDomElement labelNode = 
					nodeList.at(i).toElement().elementsByTagName("multilabel").at(0).toElement();
				
				if(labelNode.isNull())
				{
					if(error != NULL)
						error->append("Unvalid lab.xml: none multi-label not found inside the virtual machine node with").append(" ID=").append(vmName);
				}
				else
				{
					//ok, get x and y coordinates, but before check consistency
					if(!labelNode.hasAttribute("x") || !labelNode.hasAttribute("y"))
					{
						if(error != NULL)
							error->append("Unvalid lab.xml: node multi-label inside virtual machine").append(" ID=").append(vmName).append(" ").append("not standard.");
						return pos;
					}
					
					pos.setX(labelNode.attribute("x").toFloat());
					pos.setY(labelNode.attribute("y").toFloat());					
				}
			}
		}
		else
		{
			if(error != NULL)
				error->append("Unvalid lab.xml: none ID attribute for node virtual machine").append(" ID=").append(vmName);
		}
	}
	
	return pos;
}

/**
 * [STATIC-PUBLIC]
 * Get the list of plugins for a link
 */
QStringList XMLParser::getLinkPlugins(QString vmName, QString ethName,
		QString labPath, QString *error)
{
	QStringList plugins;	//return list
	QDomDocument doc = XMLExpert::readDocument(labPath);
	QDomNodeList nodeList = doc.elementsByTagName("link");
	
	for(int i=0; i<nodeList.size(); i++)
	{
		QDomElement elem = nodeList.at(i).toElement();
		
		//check node consintance
		if(!elem.hasAttribute("virtualmachine") || !elem.hasAttribute("interface"))
		{
			if(error != NULL)
				error->append("Unvalid lab.xml: Node link ").append(ethName).append(", not standard");
			return plugins;
		}
		
		if(elem.attribute("virtualmachine") == vmName && elem.attribute("interface") == ethName)
		{
			//plugin nodes
			QDomNodeList pluginsList = elem.elementsByTagName("plugin");
			
			for(int j=0; j<pluginsList.size(); j++)
			{
				QDomElement pluginNode = pluginsList.at(j).toElement();
				if(!pluginNode.hasAttribute("name"))
				{
					if(error != NULL)
						error->append("Unvalid lab.xml: Node plugin has no 'name' attribute.");
					
					return plugins;
				}
				
				plugins << pluginNode.attribute("name");
			}
		}
	}
	
	return plugins;
}

/**
 * [STATIC-PUBLIC]
 * Get the list of plugins for a virtual machine
 */
QStringList XMLParser::getVmPlugins(QString vmName, QString labPath, QString *error)
{
	QStringList plugins;	//return list
	QDomDocument doc = XMLExpert::readDocument(labPath);
	QDomNodeList nodeList = doc.elementsByTagName("virtualmachine");
	
	for(int i=0; i<nodeList.size(); i++)
	{
		QDomElement elem = nodeList.at(i).toElement();
		
		if(elem.attribute("id") == vmName)
		{
			//plugin nodes
			QDomNodeList pluginsList = elem.elementsByTagName("plugin");
			
			for(int j=0; j<pluginsList.size(); j++)
			{
				QDomElement pluginNode = pluginsList.at(j).toElement();
				if(!pluginNode.hasAttribute("name"))
				{
					if(error != NULL)
						error->append("Unvalid lab.xml: Node plugin has no 'name' attribute.");
					
					return plugins;
				}
				
				plugins << pluginNode.attribute("name");
			}
		}
	}
	
	return plugins;
}

/**
 * [STATIC-PUBLIC]
 * Get the list of plugins for a collision domain
 */
QStringList XMLParser::getCdPlugins(QString cdName, QString labPath, QString *error)
{
	QStringList plugins;	//return list
	QDomDocument doc = XMLExpert::readDocument(labPath);
	QDomNodeList nodeList = doc.elementsByTagName("collisiondomain");
	
	for(int i=0; i<nodeList.size(); i++)
	{
		QDomElement elem = nodeList.at(i).toElement();
		
		if(elem.attribute("id") == cdName)
		{
			//plugin nodes
			QDomNodeList pluginsList = elem.elementsByTagName("plugin");
			
			for(int j=0; j<pluginsList.size(); j++)
			{
				QDomElement pluginNode = pluginsList.at(j).toElement();
				if(!pluginNode.hasAttribute("name"))
				{
					if(error != NULL)
						error->append("Unvalid lab.xml: Node plugin has no 'name' attribute.");
					
					return plugins;
				}
				
				plugins << pluginNode.attribute("name");
			}
		}
	}
	
	return plugins;
}
