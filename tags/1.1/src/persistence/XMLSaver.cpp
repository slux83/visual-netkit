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

#include "XMLSaver.h"
#include "../gui/handles/VmMapper.h"
#include "../gui/handles/CdMapper.h"
#include "../gui/handles/LinkMapper.h"
#include "../gui/handles/AreaController.h"
#include "../common/Types.h"
#include "../common/CommonConfigs.h"

/**
 * Constructor
 * 
 * Path is the lab root folder.
 */
XMLSaver::XMLSaver(const QString &path)
{
	curPath = path;
}

/**
 * Deconstructor
 */
XMLSaver::~XMLSaver()
{
}

/**
 * Laboratory save function.
 * 
 * Path is the lab root folder.
 */
bool XMLSaver::saveLab()
{
	return XMLExpert::dumpDocument(prepareDomDocument(), curPath);
}

/**
 * Returns a QDomDocument representing the scene's items infos.
 */
QDomDocument* XMLSaver::prepareDomDocument()
{
	int i = 0;
	
	//prepare DOM document
	QDomDocument *doc = new QDomDocument();
	
	//root element
	QDomElement root = doc->createElement("lab");
	doc->appendChild(root);
	
	//adds lab scene infos
	QDomElement scene = doc->createElement("scene");
	root.appendChild(scene);
	
	//adds scene dimension
	QDomElement sceneDim = doc->createElement("dimension");
	scene.appendChild(sceneDim);
	
	//X position value
	sceneDim.setAttribute("width", QString(QByteArray::number(LabHandler::getInstance()->
							getMainWindow()->getGraphicsView()->scene()->width())));
	
	//Y position value
	sceneDim.setAttribute("height", QString(QByteArray::number(LabHandler::getInstance()->
							getMainWindow()->getGraphicsView()->scene()->height())));

	
	/* adds lab scene items if any. (Note: at least the green border line is in the list) */
	QList<QGraphicsItem *> itemsList = LabHandler::getInstance()->getMainWindow()->getGraphicsView()->scene()->items();
	if (!itemsList.isEmpty() && itemsList.size() > 1) 
	{
		QDomElement items = doc->createElement("items");
		scene.appendChild(items);
	
		/* adds virtualmachine items */
		QList<VirtualMachineItem *> vmlist = VmMapper::getInstance()->getVmItems();
		if (vmlist.size() > 0) 
		{	
			QDomElement vms;
			QDomElement rs;
			
			for (i=0; i<vmlist.size(); i++)
			{
				QDomElement item;
				
				//adds to XML all virtualmachine type items
				if (vms.isNull()) 
				{
					 vms = doc->createElement("virtualmachines");
					 items.appendChild(vms);
				}
				
				//creates a new virtualmachine item node
				item = doc->createElement("virtualmachine");
				vms.appendChild(item);
				
				//adds item's id
				item.setAttribute("id", vmlist.at(i)->getLabel());
				
				//adds item's scene position
				item.setAttribute("x", QString(QByteArray::number(vmlist.at(i)->scenePos().x())));
				item.setAttribute("y", QString(QByteArray::number(vmlist.at(i)->scenePos().y())));
				
				//adds item's label
				QDomElement label = doc->createElement("label");
				item.appendChild(label);
				
				//adds item's label relative position
				label.setAttribute("x", QString(QByteArray::number(vmlist.at(i)->getLabelItemPrivate()->pos().x())));
				label.setAttribute("y", QString(QByteArray::number(vmlist.at(i)->getLabelItemPrivate()->pos().y())));
				
				
				//adds item's multiline label
				QDomElement multilabel = doc->createElement("multilabel");
				item.appendChild(multilabel);
				
				//adds item's multiline label relative position
				multilabel.setAttribute("x", QString(QByteArray::number(vmlist.at(i)->getPluginsSharedArea()->pos().x())));
				multilabel.setAttribute("y", QString(QByteArray::number(vmlist.at(i)->getPluginsSharedArea()->pos().y())));
				
				//adds this VM plugin names
				QDomElement plugins = doc->createElement("plugins");
				item.appendChild(plugins);
				
				QListIterator<PluginProxy*> pIter(
						PluginRegistry::getInstance()->getVmProxies(
								VmMapper::getInstance()->getVm(vmlist.at(i))));
	
				//save plugin`s name 
				while(pIter.hasNext())
				{
					QDomElement plugin = doc->createElement("plugin");
					plugin.setAttribute("name", pIter.next()->getPlugin()->getName());
					plugins.appendChild(plugin);
				}
			}
		}
		
		/* adds collision domain items */
		QList<CollisionDomainItem *> cdsList = CdMapper::getInstance()->getCdItems();
		
		if (cdsList.size() > 0) 
		{
			//adding to XML all collisiondomain type items
			QDomElement cds = doc->createElement("collisiondomains");
			items.appendChild(cds);
			
			for (i=0; i < cdsList.size(); i++) 
			{
				// adds collisiondomain node
				QDomElement cd = doc->createElement("collisiondomain");
				cds.appendChild(cd);
				
				//adds collisiondomain's id
				QStringList strlist;
				QString str(cdsList.at(i)->getLabel());
				strlist = str.split(QRegExp("\\s+"));
				
				cd.setAttribute("id", strlist.at(0));
				
				//adds collisiondomain's scene position
				cd.setAttribute("x", QString(QByteArray::number(cdsList.at(i)->scenePos().x())));
				cd.setAttribute("y", QString(QByteArray::number(cdsList.at(i)->scenePos().y())));
				
				//adds collisiondomain's label
				QDomElement label = doc->createElement("label");
				cd.appendChild(label);
				
				
				//adds collisiondomain's label relative position
				label.setAttribute("x", QString(QByteArray::number(cdsList.at(i)->getLabelItemPrivate()->pos().x())));
				label.setAttribute("y", QString(QByteArray::number(cdsList.at(i)->getLabelItemPrivate()->pos().y())));
				
				//adds collisiondomain's multiline label
				QDomElement multilabel = doc->createElement("multilabel");
				cd.appendChild(multilabel);
				
				//adds collisiondomain's multiline label relative position
				multilabel.setAttribute("x", QString(QByteArray::number(cdsList.at(i)->getPluginsSharedArea()->pos().x())));
				multilabel.setAttribute("y", QString(QByteArray::number(cdsList.at(i)->getPluginsSharedArea()->pos().y())));
				
				//adds this CD plugin names
				QDomElement plugins = doc->createElement("plugins");
				cd.appendChild(plugins);
				
				QListIterator<PluginProxy*> pIter(
						PluginRegistry::getInstance()->getCdProxies(
								CdMapper::getInstance()->getCD(cdsList.at(i))));
	
				//save plugin`s name 
				while(pIter.hasNext())
				{
					QDomElement plugin = doc->createElement("plugin");
					plugin.setAttribute("name", pIter.next()->getPlugin()->getName());
					plugins.appendChild(plugin);
				}
	
			}
		}

		/* adds link items */
		QList<LinkItem *> linksList = LinkMapper::getInstance()->getLinkItems();
		
		if (linksList.size() > 0) 
		{
			//adding to XML all link type items
			QDomElement links = doc->createElement("links");
			items.appendChild(links);
			
			for (i=0; i < linksList.size(); i++) 
			{
				QDomElement link = doc->createElement("link");
				links.appendChild(link);
				
				//adds link's virtualmachine attribute
				link.setAttribute("virtualmachine", linksList.at(i)->getVirtualMachineItem()->getLabel());
				
				//adds link's collisiondomain attribute
				CollisionDomainItem *cd = linksList.at(i)->getCollisionDomainItem();
				
				//we want to store only first part of the label (the collision domain name)
				QStringList strlist;
				QString str(cd->getLabel());
				strlist = str.split(QRegExp("\\s+"));
				link.setAttribute("collisiondomain", strlist.at(0));
				
				QString hi(linksList.at(i)->getLabelItemPrivate()->text());
				strlist = hi.split(QRegExp("\\s+"));
				link.setAttribute("interface", strlist.at(0));				
				
				//adds link's label
				QDomElement label = doc->createElement("label");
				link.appendChild(label);
				
				//adds this link plugin names
				QDomElement plugins = doc->createElement("plugins");
				link.appendChild(plugins);
				
				QListIterator<PluginProxy*> pIter(
						PluginRegistry::getInstance()->getHiProxies(
								LinkMapper::getInstance()->getHardwareIterface(linksList.at(i))));
	
				//save plugin`s name 
				while(pIter.hasNext())
				{
					QDomElement plugin = doc->createElement("plugin");
					plugin.setAttribute("name", pIter.next()->getPlugin()->getName());
					plugins.appendChild(plugin);
				}
			}
		}
		
		/* Add area items */
		QList<AreaItem *> areas = AreaController::getInstance()->getAreas();
		if (areas.size() > 0) 
		{
			//adding to XML all area type items
			QDomElement areasNode = doc->createElement("areas");
			items.appendChild(areasNode);
			
			foreach(AreaItem* area, areas) 
			{
				QDomElement areaNode = doc->createElement("area");
				areasNode.appendChild(areaNode);
				
				//adds area position & size
				areaNode.setAttribute("x", QString::number(area->scenePos().x()));
				areaNode.setAttribute("y", QString::number(area->scenePos().y()));
				areaNode.setAttribute("width", QString::number(area->rect().width()));
				areaNode.setAttribute("height", QString::number(area->rect().height()));
				areaNode.setAttribute("z", QString::number(area->zValue()));
				
				//color in R,G,B syntax
				areaNode.setAttribute("color",	QString::number(area->getCurrentColor().red()) + "," +
												QString::number(area->getCurrentColor().green()) + "," +
												QString::number(area->getCurrentColor().blue()));
				
				QByteArray areaTextB64 = area->getLabel().toUtf8().toBase64();
				QDomCDATASection areaText = doc->createCDATASection(QString(areaTextB64));			
				areaNode.appendChild(areaText);
			}
		}
		
	}
	
	/* Save excluded paths */
	QStringList excludedPaths = LabHandler::getInstance()->getExcludePaths();
	QDomElement excludedNode = doc->createElement("excluded");
	root.appendChild(excludedNode);
	
	//save paths as excluded's childs
	foreach(QString path, excludedPaths)
	{
		QDomElement pathNode = doc->createElement("path");
		QDomCDATASection pathString = doc->createCDATASection(path);
		pathNode.appendChild(pathString);
		excludedNode.appendChild(pathNode);
	}
	
	return doc;
}
