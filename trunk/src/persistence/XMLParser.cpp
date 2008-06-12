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
	QDomDocument *doc = XMLExpert::readDocument(labPath);
	QRectF sceneSize;	//the return value
	QDomElement dimension = 
		doc->elementsByTagName("lab").at(0).toElement().
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
	QDomDocument *doc = XMLExpert::readDocument(labPath);
	QPointF vmPos;	//the return value
	QDomNodeList nodeList = doc->elementsByTagName("virtualmachine");
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
				
				vmPos.setX(nodeList.at(i).toElement().attribute("x").toInt());
				vmPos.setY(nodeList.at(i).toElement().attribute("y").toInt());
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
	QDomDocument *doc = XMLExpert::readDocument(labPath);
	QPointF cdPos;	//the return value
	QDomNodeList nodeList = doc->elementsByTagName("collisiondomain");
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
				
				cdPos.setX(nodeList.at(i).toElement().attribute("x").toInt());
				cdPos.setY(nodeList.at(i).toElement().attribute("y").toInt());
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
	QDomDocument *doc = XMLExpert::readDocument(labPath);
	QPointF pos;	//the return value
	QDomNodeList nodeList = doc->elementsByTagName("collisiondomain");
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
					
					pos.setX(labelNode.attribute("x").toInt());
					pos.setY(labelNode.attribute("y").toInt());					
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
	QDomDocument *doc = XMLExpert::readDocument(labPath);
	QPointF pos;	//the return value
	QDomNodeList nodeList = doc->elementsByTagName("collisiondomain");
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
					
					pos.setX(labelNode.attribute("x").toInt());
					pos.setY(labelNode.attribute("y").toInt());					
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
	QDomDocument *doc = XMLExpert::readDocument(labPath);
	QPointF pos;	//the return value
	QDomNodeList nodeList = doc->elementsByTagName("virtualmachine");
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
					
					pos.setX(labelNode.attribute("x").toInt());
					pos.setY(labelNode.attribute("y").toInt());					
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
	QDomDocument *doc = XMLExpert::readDocument(labPath);
	QPointF pos;	//the return value
	QDomNodeList nodeList = doc->elementsByTagName("virtualmachine");
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
					
					pos.setX(labelNode.attribute("x").toInt());
					pos.setY(labelNode.attribute("y").toInt());					
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


///**
// * Reads and store the QDomDocument representing laboratory graph.
// * This function has to be called before loadXML() function.
// */
//bool XMLParser::parseXML()
//{
//	bool allok = true;
//	QString not_found("not found");
//	
//	qDebug() << "Parsing XML: ";
//	
//	// parsing XML to create QMap<QString, QString> *labInfos
//	qDebug() << "looking for lab infos... ";
//	QDomElement lab = labDom->elementsByTagName("lab").at(0).toElement();
//	labInfos = new QMap<QString, QString>();
//	labInfos->insert("name", lab.attribute("name", not_found));
//	labInfos->insert("date", lab.attribute("date", not_found));
//	qDebug() << "done!" << endl;
//	
//	// parsing XML to create QMap<QString, QString> *sceneInfos
//	qDebug() << "looking for scene infos... ";
//	lab = labDom->elementsByTagName("scene").at(0).toElement();
//	sceneInfos = new QMap<QString, QString>();
//	sceneInfos->insert("width", lab.attribute("width", not_found));
//	sceneInfos->insert("height",lab.attribute("height", not_found));
//	qDebug() << "done!" << endl;
//	
//	//looking for any item in the scene
//	qDebug() << "looking for any item in the scene..." << endl;
//	QDomElement itemsNode = labDom->elementsByTagName("items").at(0).toElement(); 
//	if (!itemsNode.isNull())
//	{
//		int i=0;
//		
//		//looking for any virtualmachine in the scene
//		QDomNodeList vmsNodeList = itemsNode.elementsByTagName("virtualmachine");
//		if (!vmsNodeList.isEmpty())
//		{
//			// parsing XML to create QMap< QString, QMap<QString, QString> > *vmsInfos
//			qDebug() << "looking for virtual machines infos...";
//			vmsInfos = new QMap< QString, QMap<QString, QString> >();
//			for (i=0; i < vmsNodeList.size(); i++)
//			{
//				QDomElement vmNode = vmsNodeList.at(i).toElement();
//				QMap<QString, QString> vm;
//				
//				// adds vm id
//				vm.insert("id", vmNode.attribute("id", not_found));
//
//				// adds vm type
//				vm.insert("type", vmNode.attribute("type", not_found));
//				
//				// adds vm position
//				vm.insert("posx", vmNode.attribute("x", not_found));
//				vm.insert("posy", vmNode.attribute("y", not_found));
//				
//				// adds vm label position
//				QDomElement label = vmNode.elementsByTagName("label").at(0).toElement();
//				vm.insert("labelposx", label.attribute("rposx", not_found));
//				vm.insert("labelposy", label.attribute("rposy", not_found));
//				
//				//default value in case of error is the iterator position i
//				vmsInfos->insert(vmNode.attribute("id", QString::number(i)), vm);
//			}
//			qDebug() << "done!";
//		}
//		
//		
//		//looking for any collision domain in the scene
//		QDomNodeList cdsNodeList = itemsNode.elementsByTagName("collisiondomain");
//		if (!cdsNodeList.isEmpty())
//		{
//			// parsing XML to create QMap<QString, QMap<QString, QString> > *cdsInfos
//			qDebug() << "looking for collision domain infos...";
//			cdsInfos = new QMap<QString, QMap<QString, QString> >();
//			for (i=0; i < cdsNodeList.size(); i++)
//			{
//				QDomElement cdNode = cdsNodeList.at(i).toElement();
//				QMap<QString, QString> cd;
//				
//				// adds cd id
//				cd.insert("id", cdNode.attribute("id", not_found));
//				
//				// adds cd position
//				cd.insert("posx", cdNode.attribute("x", not_found));
//				cd.insert("posy", cdNode.attribute("y", not_found));
//				
//				// adds cd label position
//				QDomElement label = cdNode.elementsByTagName("label").at(0).toElement();
//				cd.insert("labelposx", label.attribute("rposx", not_found));
//				cd.insert("labelposy", label.attribute("rposy", not_found));
//				
//				//default value in case of error is the iterator position i
//				cdsInfos->insert(cdNode.attribute("id", QString::number(i)), cd);
//			}
//			qDebug() << "done!";
//		}
//		
//		
//		//looking for any link in the scene
//		QDomNodeList linksNodeList = itemsNode.elementsByTagName("link");
//		if (!linksNodeList.isEmpty())
//		{
//			// parsing XML to create QMap<QString, QString> *linksInfos	
//			qDebug() << "looking for link infos...";
//			linksInfos = new QList< QMap<QString, QString> >();
//			for (i=0; i < linksNodeList.size(); i++)
//			{
//				QDomElement linkNode = linksNodeList.at(i).toElement();
//				QMap<QString, QString> link;
//				
//				// adds link's virtualmachine
//				link.insert("virtualmachine", linkNode.attribute("virtualmachine", not_found));
//				
//				// adds link's collisiondomain
//				link.insert("collisiondomain", linkNode.attribute("collisiondomain", not_found));
//				
//				// adds vm label position
//				QDomElement label = linkNode.elementsByTagName("label").at(0).toElement();
//				link.insert("labelposx", label.attribute("rposx", not_found));
//				link.insert("labelposy", label.attribute("rposy", not_found));
//				
//				linksInfos->append(link);
//			}
//			qDebug() << "done!";
//		}
//		
//	}
//	return allok;
//}
//
///**
// * Reads and store the QDomDocument representing laboratory graph.
// * This function has to be called in order to populate and use the QDomDocument.
// */
//bool XMLParser::loadXML()
//{
//	qDebug() << "loadXML(): reading labDom...";
//	labDom = ex->readDocument();
//	qDebug() << "done!" << endl;
//	return labDom->isNull();
//}
//
///**
// * Returns the QDomDocument representing laboratory graph.
// */
//QDomDocument* XMLParser::getLabDom()
//{
//	return labDom;
//}
//
//
///**
// * Returns a fake laboratory with name and date attributes.
// */
//Laboratory* XMLParser::getLaboratory()
//{
//	Laboratory *lab = new Laboratory();
//	if (!labDom->isNull())
//	{
//		lab->setName(labInfos->value("name"));
//		lab->setDate(labInfos->value("date"));
//	} else {
//		qWarning() << "Function XMLParser::getLaboratory(): labDom is null!";
//	}
//	return lab;
//}
//
