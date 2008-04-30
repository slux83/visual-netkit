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

XMLParser::XMLParser()
{
	ex = new XMLExpert();
}

XMLParser::~XMLParser()
{
}

/**
 * Reads and store the QDomDocument representing laboratory graph.
 * This function has to be called before loadXML() function.
 */
bool XMLParser::parseXML()
{
	bool allok = true;
	QString not_found("not found");
	
	// parsing XML to create QMap<QString, QString> *labInfos
	QDomElement lab = labDom->elementsByTagName("lab").at(0).toElement();
	labInfos->insert("name", lab.attribute("name", not_found));
	labInfos->insert("date", lab.attribute("date", not_found));
	
	// parsing XML to create QMap<QString, QString> *sceneInfos
	lab = labDom->elementsByTagName("scene").at(0).toElement();
	sceneInfos->insert("width", lab.attribute("width", not_found));
	sceneInfos->insert("height",lab.attribute("height", not_found));
	
	//looking for any item in the scene
	QDomElement itemsNode = labDom->elementsByTagName("items").at(0).toElement(); 
	if (!itemsNode.isNull())
	{
		int i=0;
		
		//looking for any virtualmachine in the scene
		QDomNodeList vmsNodeList = itemsNode.elementsByTagName("virtualmachine");
		if (!vmsNodeList.isEmpty())
		{
			// parsing XML to create QMap<QString, QString> *vmsInfos
			for (i=0; i < vmsNodeList.size(); i++)
			{
				// adds vm position
				QDomElement vmNode = vmsNodeList.at(i).toElement();
				QDomElement pos = vmNode.elementsByTagName("position").at(0).toElement();
				vmsInfos->insert("posx", pos.attribute("x", not_found));
				vmsInfos->insert("posy", pos.attribute("y", not_found));
				
				// adds vm label
				QDomElement label = vmNode.elementsByTagName("label").at(0).toElement();
				QDomElement labelt = label.elementsByTagName("text").at(0).toElement();
				vmsInfos->insert("label", labelt.attribute("text", not_found));
				
				// adds vm label position
				QDomElement rpos = label.elementsByTagName("position").at(0).toElement();
				vmsInfos->insert("labelposx", rpos.attribute("rposx", not_found));
				vmsInfos->insert("labelposy", rpos.attribute("rposy", not_found));
				
			}
			
		}
		
		// parsing XML to create QMap<QString, QString> *cdsInfos
		// parsing XML to create QMap<QString, QString> *linksInfos	
	}
	return allok;
}

/**
 * Reads and store the QDomDocument representing laboratory graph.
 * This function has to be called in order to populate and use the QDomDocument.
 */
bool XMLParser::loadXML(QString *filename)
{
	labDom = ex->readDocument(filename);
	return labDom->isNull();
}

/**
 * Returns the QDomDocument representing laboratory graph.
 */
QDomDocument* XMLParser::getLabDom()
{
	return labDom;
}


/**
 * Returns a fake laboratory with name and date attributes.
 */
Laboratory* XMLParser::getLaboratory()
{
	Laboratory *lab = new Laboratory();
	lab->setName("");
	lab->setDate("");
	return lab;
}

