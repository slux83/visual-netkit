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
#include "../gui/handles/LinkMapper.h"
#include "../common/Types.h"
#include "../common/CommonConfigs.h"
#include "../gui/LinkItem.h"

XMLSaver::XMLSaver()
{
	ex = new XMLExpert();
}

XMLSaver::~XMLSaver()
{
}

bool XMLSaver::saveLab()
{
	return ex->dumpDocument(prepareDomDocument(), new QString("lab.xml"));
}

QDomDocument* XMLSaver::prepareDomDocument()
{
	//prepare DOM document
	QDomDocument *doc = new QDomDocument("il mio doctype");
	
	//root element
	QDomElement root = doc->createElement("lab");
	doc->appendChild(root);
	
	
	//==========================================================================
	//adds lab name
	QDomElement labname = doc->createElement("name");
	root.appendChild(labname);
	QDomText labnamet = doc->createTextNode("lab_name");
	labname.appendChild(labnamet);
	
	//adds lab date
	QDomElement labdate = doc->createElement("date");
	root.appendChild(labdate);
	QDomText labdatet = doc->createTextNode("lab_date");
	labdate.appendChild(labdatet);
	//==========================================================================
	
	
	
	//adds lab scene infos
	QDomElement scene = doc->createElement("scene");
	root.appendChild(scene);
	
	//adds scene dimensions
	QDomElement sceneDim = doc->createElement("dimensions");
	scene.appendChild(sceneDim);
	
	//X position value
	QDomElement sceneDimX = doc->createElement("x");
	sceneDim.appendChild(sceneDimX);		
	QDomText sceneDimXt = doc->createTextNode(QString(int(LabHandler::getInstance()->
							getMainWindow()->getGraphicsView()->scene()->width())));
	sceneDimX.appendChild(sceneDimXt);
	
	//Y position value
	QDomElement sceneDimY = doc->createElement("y");
	sceneDim.appendChild(sceneDimY);
	QDomText sceneDimYt = doc->createTextNode("undefined");
	sceneDimY.appendChild(sceneDimYt);
	
	
	
	//adds lab scene items
	QDomElement items = doc->createElement("items");
	scene.appendChild(items);
	

	
	//==========================================================================	
	// adds virtualmachine and router items
	QList<VirtualMachineItem *> vmlist = VmMapper::getInstance()->getVmItems();
	if (vmlist.size() > 0) 
	{	
		QDomElement vms;
		QDomElement rs;
		
		for (int i=0; i<vmlist.size(); i++)
		{
			QDomElement item;
			
			VmType type = dynamic_cast<VirtualMachineItem*>(vmlist.at(i))->getVmType();
			qDebug() << type;
			
			if (type == Host) 
			{
				//adds to XML all virtualmachine type items
				if (rs.isNull()) 
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
			
			//adds item's label
			QDomElement label = doc->createElement("label");
			item.appendChild(label);
			
			//adds item's label text
			QDomText labelt = doc->createTextNode(vmlist.at(i)->getLabel());
			label.appendChild(labelt);
			
			//adds item's scene position
			QDomElement pos = doc->createElement("position");
			item.appendChild(pos);
			
			//adds x and y position
			QDomText posx = doc->createTextNode("undefined");
			QDomText posy = doc->createTextNode("undefined");
			pos.appendChild(posx);
			pos.appendChild(posy);
		}
	}
	//==========================================================================

	
	//==========================================================================	
	// getting link items
	QList<LinkItem *> linksList = LinkMapper::getInstance()->getLinkItems();
	
	// adds link items
	if (linksList.size() > 0) 
	{
		//adding to XML all link type items
		QDomElement links = doc->createElement("links");
		items.appendChild(links);
		
		//
		for (int j=0; j < linksList.size(); j++) 
		{
			QDomElement link = doc->createElement("link");
			links.appendChild(link);
			
			//adds link's virtualmachine element
			QDomElement virtualmachine = doc->createElement("virtualmachine");
			link.appendChild(virtualmachine);
			
			VirtualMachineItem *vm = linksList.at(j)->getVirtualMachineItem();
			QDomText vmt = doc->createTextNode(vm->getLabel());
			virtualmachine.appendChild(vmt);
			
			//adds link's collisiondomain element
			QDomElement collisiondomain = doc->createElement("collisiondomain");
			link.appendChild(collisiondomain);
			
			CollisionDomainItem *cd = linksList.at(j)->getCollisionDomainItem();
			QDomText cdt = doc->createTextNode(cd->getLabel());
			collisiondomain.appendChild(cdt);		
			
			
			//adds link's label
			QDomElement label = doc->createElement("label");
			link.appendChild(label);
			
			//adds link's label text
			QDomText labelt = doc->createTextNode(linksList.at(j)->getLabelItemPrivate()->text());
			label.appendChild(labelt);
		}
	}
	
	
	return doc;
}
