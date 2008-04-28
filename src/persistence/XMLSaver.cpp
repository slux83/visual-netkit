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
	
	//adds lab scene items
	QDomElement items = doc->createElement("items");
	scene.appendChild(items);
	

	
	//==========================================================================	
	// adds virtualmachine and router items
	QList<VirtualMachineItem *> vmlist = VmMapper::getInstance()->getVmItems();
	if (vmlist.size() > 0) 
	{	
		//adding to XML all virtualmachine type items
		QDomElement vms = doc->createElement("virtualmachines");
		items.appendChild(vms);

		//adding to XML all router type items
		QDomElement rs = doc->createElement("routers");
		items.appendChild(rs);
		
		for (int i=0; i<vmlist.size(); i++)
		{
			QDomElement item;
			
			VmType type = dynamic_cast<VirtualMachineItem*>(vmlist.at(i))->getVmType();
			qDebug() << type;
			if (type == Host) {
				//creates a new virtualmachine item node
				item = doc->createElement("virtualmachine");
				vms.appendChild(item);
			} else if (type == Router) {
				//creates a new router item node
				item = doc->createElement("router");
				rs.appendChild(item);
			} else {
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
			QDomText posx = doc->createTextNode(0);
			QDomText posy = doc->createTextNode(0);
			pos.appendChild(posx);
			pos.appendChild(posy);
		}
	}
	//==========================================================================

	
	//==========================================================================	
	// adds link items
	QList<LinkItem *> links;// = VmMapper::getInstance()->getVmItems();
	if (links.size() > 0) 
	{
		//adding to XML all link type items
		QDomElement links = doc->createElement("links");
		items.appendChild(links);
		
		/*
		for (int j=0; j < links.size(); j++) {
			QDomElement l = doc->createElement("link");
			links.appendChild(l);
			
			//adds link's startnode element
			QDomElement startnode = doc->createElement("startnode");
			
			SvgItemPrivate sn = dynamic_cast<SvgItemPrivate*>(link.at(j))->getStartNode();
			QString name = sn->getLabel();
			QDomText snt = doc->createTextNode(name);
			label.appendChild(snt);
			link.appendChild(startnode);
			
			//adds link's endnode element
			QDomElement endnode = doc->createElement("endnode");
			SvgItemPrivate en = dynamic_cast<SvgItemPrivate*>(link.at(j))->getEndNode();
			QString name = en->getLabel();
			QDomText ent = doc->createTextNode(name);
			label.appendChild(ent);
			link.appendChild(endnode);			
			
			//adds link's label
			QDomElement label = doc->createElement("label");
			link.appendChild(label);
			
			//adds link's label text
			QDomText labelt = doc->createTextNode(vmlist.at(i)->getLabel());
			label.appendChild(labelt);
		}
		*/
	}
	
	
	
	
	return doc;
}
