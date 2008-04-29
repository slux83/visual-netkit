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
#include "../common/Types.h"
#include "../common/CommonConfigs.h"
#include "../gui/LinkItem.h"

/**
 * Constructor
 */
XMLSaver::XMLSaver()
{
	ex = new XMLExpert();
}

/**
 * Deconstructor
 */
XMLSaver::~XMLSaver()
{
}

/**
 * Laboratory save function.
 */
bool XMLSaver::saveLab()
{
	return ex->dumpDocument(prepareDomDocument(), new QString("lab.xml"));
}

/**
 * Returns a QDomDocument representing the scene's items infos.
 */
QDomDocument* XMLSaver::prepareDomDocument()
{
	int i=0;
	
	//prepare DOM document
	QDomDocument *doc = new QDomDocument();
	
	//root element
	QDomElement root = doc->createElement("lab");
	doc->appendChild(root);
	
	
	//==========================================================================
	//adds lab name
	QDomElement labname = doc->createElement("name");
	root.appendChild(labname);
	QDomText labnamet = doc->createTextNode(LabFacadeController::getInstance()->getCurrentLab()->getName());
	labname.appendChild(labnamet);
	
	//adds lab date
	QDomElement labdate = doc->createElement("date");
	root.appendChild(labdate);
	QDomText labdatet = doc->createTextNode(LabFacadeController::getInstance()->getCurrentLab()->getDate());
	labdate.appendChild(labdatet);
	//==========================================================================
	
	
	
	//adds lab scene infos
	QDomElement scene = doc->createElement("scene");
	root.appendChild(scene);
	
	//adds scene dimensions
	QDomElement sceneDim = doc->createElement("dimensions");
	scene.appendChild(sceneDim);
	
	//X position value
	sceneDim.setAttribute("width", QString(QByteArray::number(LabHandler::getInstance()->
							getMainWindow()->getGraphicsView()->scene()->width())));
	/*
	QDomElement sceneDimX = doc->createElement("x");
	sceneDim.appendChild(sceneDimX);		
	QDomText sceneDimXt = doc->createTextNode(QByteArray::number(LabHandler::getInstance()->
							getMainWindow()->getGraphicsView()->scene()->width()));
	sceneDimX.appendChild(sceneDimXt);
	*/
	
	//Y position value
	sceneDim.setAttribute("height", QString(QByteArray::number(LabHandler::getInstance()->
							getMainWindow()->getGraphicsView()->scene()->height())));
	/*
	QDomElement sceneDimY = doc->createElement("y");
	sceneDim.appendChild(sceneDimY);
	QDomText sceneDimYt = doc->createTextNode(QByteArray::number(LabHandler::getInstance()->
			getMainWindow()->getGraphicsView()->scene()->height()));
	sceneDimY.appendChild(sceneDimYt);
	*/
	
	
	
	//adds lab scene items if any. (Note: at least the green border line is in the list)
	QList<QGraphicsItem *> itemsList = LabHandler::getInstance()->getMainWindow()->getGraphicsView()->scene()->items();
	if (!itemsList.isEmpty() && itemsList.size()>1) 
	{
		QDomElement items = doc->createElement("items");
		scene.appendChild(items);

	
		//==========================================================================	
		// adds virtualmachine and router items
		QList<VirtualMachineItem *> vmlist = VmMapper::getInstance()->getVmItems();
		if (vmlist.size() > 0) 
		{	
			QDomElement vms;
			QDomElement rs;
			
			for (i=0; i<vmlist.size(); i++)
			{
				QDomElement item;
				
				VmType type = dynamic_cast<VirtualMachineItem*>(vmlist.at(i))->getVmType();
				
				if (type == Host) 
				{
					//adds to XML all virtualmachine type items
					if (vms.isNull()) 
					{
						 vms = doc->createElement("virtualmachines");
						 items.appendChild(vms);
					}
					
					//creates a new virtualmachine item node
					item = doc->createElement("virtualmachine");
					vms.appendChild(item);
				} 
				else if (type == Router) 
				{
					//adds to XML all router type items
					if (rs.isNull()) 
					{
						rs = doc->createElement("routers");
						items.appendChild(rs);
					}
					
					//creates a new router item node
					item = doc->createElement("router");
					rs.appendChild(item);
				} 
				else 
				{
					qWarning()	<< "Failed preparing Document in XMLSaver.cpp: "
								<< "item type not regnized!";
				}
				
				
				//adds item's scene position
				QDomElement pos = doc->createElement("position");
				item.appendChild(pos);
				
				pos.setAttribute("x", QString(QByteArray::number(vmlist.at(i)->scenePos().x())));
				pos.setAttribute("y", QString(QByteArray::number(vmlist.at(i)->scenePos().y())));			
				/*
				QDomElement posx = doc->createElement("x");
				pos.appendChild(posx);
				QDomElement posy = doc->createElement("y");
				pos.appendChild(posy);
				
				//adds x and y position
				QDomText posxt = doc->createTextNode(QByteArray::number(vmlist.at(i)->scenePos().x()));
				QDomText posyt = doc->createTextNode(QByteArray::number(vmlist.at(i)->scenePos().y()));
				posx.appendChild(posxt);
				posy.appendChild(posyt);
				*/
				
				
				//adds item's label
				QDomElement label = doc->createElement("label");
				item.appendChild(label);
				
				//adds item's label text node
				QDomElement text = doc->createElement("text");
				label.appendChild(text);
				
				//adds item's label text
				QDomText labelt = doc->createTextNode(vmlist.at(i)->getLabel());
				text.appendChild(labelt);
				
				//adds item's label relative position
				QDomElement rpos = doc->createElement("position");
				label.appendChild(rpos);
				
				rpos.setAttribute("x", QString(QByteArray::number(vmlist.at(i)->getLabelItemPrivate()->pos().x())));
				rpos.setAttribute("y", QString(QByteArray::number(vmlist.at(i)->getLabelItemPrivate()->pos().y())));
				/*
				QDomElement rposx = doc->createElement("x");
				rpos.appendChild(rposx);
				QDomElement rposy = doc->createElement("y");
				rpos.appendChild(rposy);
				
				//adds x and y position
				QDomText rposxt = doc->createTextNode(QByteArray::number(vmlist.at(i)->getLabelItemPrivate()->pos().x()));
				QDomText rposyt = doc->createTextNode(QByteArray::number(vmlist.at(i)->getLabelItemPrivate()->pos().y()));
				rposx.appendChild(rposxt);
				rposy.appendChild(rposyt);
				*/
			}
		}
		//==========================================================================
	
		
		
		//==========================================================================	
		// adds collision domain items
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
				
				//adds collisiondomain's scene position
				QDomElement pos = doc->createElement("position");
				cd.appendChild(pos);
				
				pos.setAttribute("x", QString(QByteArray::number(cdsList.at(i)->scenePos().x())));
				pos.setAttribute("y", QString(QByteArray::number(cdsList.at(i)->scenePos().y())));			
				/*
				QDomElement posx = doc->createElement("x");
				pos.appendChild(posx);
				QDomElement posy = doc->createElement("y");
				pos.appendChild(posy);
				
				//adds x and y position
				QDomText posxt = doc->createTextNode(QByteArray::number(cdsList.at(i)->scenePos().x()));
				QDomText posyt = doc->createTextNode(QByteArray::number(cdsList.at(i)->scenePos().y()));
				posx.appendChild(posxt);
				posy.appendChild(posyt);
				*/
				
				//adds collisiondomain's label
				QDomElement label = doc->createElement("label");
				cd.appendChild(label);
				
				QDomElement text = doc->createElement("text");
				label.appendChild(text);
				
				//adds collisiondomain's label text
				QDomText labelt = doc->createTextNode(cdsList.at(i)->getLabel());
				text.appendChild(labelt);
				
				//adds collisiondomain's label relative position
				QDomElement rpos = doc->createElement("position");
				label.appendChild(pos);
				
				rpos.setAttribute("x", QString(QByteArray::number(cdsList.at(i)->pos().x())));
				rpos.setAttribute("y", QString(QByteArray::number(cdsList.at(i)->pos().y())));			
				/*
				QDomElement rposx = doc->createElement("x");
				rpos.appendChild(rposx);
				QDomElement rposy = doc->createElement("y");
				rpos.appendChild(rposy);
				
				//adds x and y position
				QDomText rposxt = doc->createTextNode(QByteArray::number(cdsList.at(i)->pos().x()));
				QDomText rposyt = doc->createTextNode(QByteArray::number(cdsList.at(i)->pos().y()));
				rposx.appendChild(rposxt);
				rposy.appendChild(rposyt);
				*/
			}
		}
		//==========================================================================	
		
		
		
		//==========================================================================	
		// adds link items
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
				
				//adds link's virtualmachine element
				QDomElement virtualmachine = doc->createElement("virtualmachine");
				link.appendChild(virtualmachine);
				
				VirtualMachineItem *vm = linksList.at(i)->getVirtualMachineItem();
				QDomText vmt = doc->createTextNode(vm->getLabel());
				virtualmachine.appendChild(vmt);
				
				//adds link's collisiondomain element
				QDomElement collisiondomain = doc->createElement("collisiondomain");
				link.appendChild(collisiondomain);
				
				CollisionDomainItem *cd = linksList.at(i)->getCollisionDomainItem();
				
				//we want to store only first part of the label (the collision domain name)
				QStringList strlist;
				QString str(cd->getLabel());
				strlist = str.split(QRegExp("\\s+"));
				QDomText cdt = doc->createTextNode(strlist.at(0));
				collisiondomain.appendChild(cdt);
				
				
				//adds link's label
				QDomElement label = doc->createElement("label");
				link.appendChild(label);
				
				QDomElement text = doc->createElement("text");
				label.appendChild(text);
				
				//adds link's label text
				QDomText labelt = doc->createTextNode(linksList.at(i)->getLabelItemPrivate()->text());
				text.appendChild(labelt);
				
				//adds link's label relative position
				QDomElement rpos = doc->createElement("position");
				label.appendChild(rpos);
				
				rpos.setAttribute("x", QString(QByteArray::number(linksList.at(i)->getLabelItemPrivate()->pos().x())));
				rpos.setAttribute("y", QString(QByteArray::number(linksList.at(i)->getLabelItemPrivate()->pos().y())));
				/*
				QDomElement rposx = doc->createElement("x");
				rpos.appendChild(rposx);
				QDomElement rposy = doc->createElement("y");
				rpos.appendChild(rposy);
				
				//adds x and y position
				QDomText rposxt = doc->createTextNode(QByteArray::number(linksList.at(i)->getLabelItemPrivate()->pos().x()));
				QDomText rposyt = doc->createTextNode(QByteArray::number(linksList.at(i)->getLabelItemPrivate()->pos().y()));
				rposx.appendChild(rposxt);
				rposy.appendChild(rposyt);
				*/
			}
		}
	}
	
	
	return doc;
}
