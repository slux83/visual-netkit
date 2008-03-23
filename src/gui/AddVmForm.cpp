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

#include "AddVmForm.h"
#include "../core/Types.h"
#include <QDesktopWidget>
#include <QTreeWidgetItem>
#include <QRect>
#include <QCheckBox> 

/**
 * Contructor
 */
AddVmForm::AddVmForm(QWidget *parent) : QWidget(parent)
{
	/* Connect the UI resource to this QWidget */
	setupUi(this);
	fillDaemonsList();
	
	/* Centering the gui */
	QDesktopWidget dw;
	QRect geometry = dw.screenGeometry();
	setGeometry((geometry.width() / 2) - (width() / 2),		//center X
				(geometry.height() / 2) - (height() / 2),	//center Y
				width(),
				height());
}

/**
 * Decontructor
 */
AddVmForm::~AddVmForm()
{
}

/**
 * [PRIVATE]
 * Add the daemons to the list view
 */
void AddVmForm::fillDaemonsList()
{	
	QTreeWidgetItem *zebraItem;
	QTreeWidgetItem *ripd, *bgpd, *ospfd, *ospf6d, *ripngd;
	
	//Zebra
	zebraItem = new QTreeWidgetItem();
	zebraItem->setData(0, Qt::DisplayRole, "Zebra");
	zebraItem->setData(0, Qt::UserRole, Zebra);
	
	//Ripd
	ripd = new QTreeWidgetItem();
	ripd->setData(0, Qt::DisplayRole, "Ripd");
	ripd->setData(0, Qt::UserRole, Ripd);
	zebraItem->addChild(ripd);
	
	//Bgpd
	bgpd = new QTreeWidgetItem();
	bgpd->setData(0, Qt::DisplayRole, "Bgpd");
	bgpd->setData(0, Qt::UserRole, Bgpd);
	zebraItem->addChild(bgpd);
	
	//Ospfd
	ospfd = new QTreeWidgetItem();
	ospfd->setData(0, Qt::DisplayRole, "Ospfd");
	ospfd->setData(0, Qt::UserRole, Ospfd);
	zebraItem->addChild(ospfd);
	
	//Ospf6d
	ospf6d = new QTreeWidgetItem();
	ospf6d->setData(0, Qt::DisplayRole, "Ospf6d");
	ospf6d->setData(0, Qt::UserRole, Ospf6d);
	zebraItem->addChild(ospf6d);
	
	//Ripngd
	ripngd = new QTreeWidgetItem();
	ripngd->setData(0, Qt::DisplayRole, "Ripngd");
	ripngd->setData(0, Qt::UserRole, Ripngd);
	zebraItem->addChild(ripngd);
	
	daemonsList->addTopLevelItem(zebraItem);
	daemonsList->expandItem(zebraItem);	//expand the level
	
	/* Add checkboxes */
	daemonsList->setItemWidget(zebraItem,	1, new QCheckBox());
	daemonsList->setItemWidget(ripd,		1, new QCheckBox());
	daemonsList->setItemWidget(bgpd,		1, new QCheckBox());
	daemonsList->setItemWidget(ospfd,		1, new QCheckBox());
	daemonsList->setItemWidget(ospf6d,		1, new QCheckBox());
	daemonsList->setItemWidget(ripngd,		1, new QCheckBox());
	
}