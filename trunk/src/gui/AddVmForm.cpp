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
#include "../common/Types.h"
#include <QDesktopWidget>
#include <QTreeWidgetItem>
#include <QRect>
#include <QMessageBox>

#define REGEXP_ALL_DAEMONS QString("Zebra|Ripd|Bgpd|Ospfd|Ospf6d|Ripngd")

/**
 * Contructor
 */
AddVmForm::AddVmForm(QWidget *parent) : QWidget(parent)
{
	/* Connect the UI resource to this QWidget */
	setupUi(this);
	fillDaemonsList();
	
	/* Get the controller instance */
	vmHandler = VmHandler::getInstance();
	
	/* Centering the gui */
	QDesktopWidget dw;
	QRect geometry = dw.screenGeometry();
	setGeometry((geometry.width() / 2) - (width() / 2),		//center X
				(geometry.height() / 2) - (height() / 2),	//center Y
				width(),
				height());
	
	/* Set the vm preview image */
	spacePreview = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	vmType = new QLabel(tr("Type: ") + "Host");
	vmPreview = new QSvgWidget(QString::fromUtf8(":/svg/vm_host"));
	vmPreview->setFixedSize(48, 48);
	
	previewGroup->layout()->setAlignment(Qt::AlignHCenter);
	previewGroup->layout()->addWidget(vmPreview);
	previewGroup->layout()->addWidget(vmType);
	previewGroup->layout()->addItem(spacePreview);
	
	/* Connections */
	connect(daemonsList, SIGNAL(itemChanged(QTreeWidgetItem *, int)),
			this, SLOT(updateVmPreview(QTreeWidgetItem *, int)));
	connect(addVmButtonBox, SIGNAL(accepted()),
			this, SLOT(handleAcceptedSignal()));
	connect(this, SIGNAL(userAddedVm(QString, QList<Daemon>)),
			vmHandler, SLOT(createVm(QString, QList<Daemon>)));
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
	zebraItem->setCheckState (0, Qt::Unchecked);
	
	//Ripd
	ripd = new QTreeWidgetItem();
	ripd->setData(0, Qt::DisplayRole, "Ripd");
	ripd->setData(0, Qt::UserRole, Ripd);
	ripd->setCheckState (0, Qt::Unchecked);
	zebraItem->addChild(ripd);
	
	//Bgpd
	bgpd = new QTreeWidgetItem();
	bgpd->setData(0, Qt::DisplayRole, "Bgpd");
	bgpd->setData(0, Qt::UserRole, Bgpd);
	bgpd->setCheckState (0, Qt::Unchecked);
	zebraItem->addChild(bgpd);
	
	//Ospfd
	ospfd = new QTreeWidgetItem();
	ospfd->setData(0, Qt::DisplayRole, "Ospfd");
	ospfd->setData(0, Qt::UserRole, Ospfd);
	ospfd->setCheckState (0, Qt::Unchecked);
	zebraItem->addChild(ospfd);
	
	//Ospf6d
	ospf6d = new QTreeWidgetItem();
	ospf6d->setData(0, Qt::DisplayRole, "Ospf6d");
	ospf6d->setData(0, Qt::UserRole, Ospf6d);
	ospf6d->setCheckState (0, Qt::Unchecked);
	zebraItem->addChild(ospf6d);
	
	//Ripngd
	ripngd = new QTreeWidgetItem();
	ripngd->setData(0, Qt::DisplayRole, "Ripngd");
	ripngd->setData(0, Qt::UserRole, Ripngd);
	ripngd->setCheckState (0, Qt::Unchecked);
	zebraItem->addChild(ripngd);
	
	daemonsList->addTopLevelItem(zebraItem);
	daemonsList->expandItem(zebraItem);	//expand the level
	
}

/**
 * [PRIVATE-SLOT]
 * Check the user daemon choise, and update the preview
 */
void AddVmForm::updateVmPreview(QTreeWidgetItem *item, int column)
{
	Q_UNUSED(item)
	Q_UNUSED(column)
	
	/* Get all items */
	QList<QTreeWidgetItem *> items = daemonsList->findItems(
			REGEXP_ALL_DAEMONS,	Qt::MatchRegExp | Qt::MatchRecursive, 0);
	
	bool routerDaemonChecked = false;
	foreach(QTreeWidgetItem *item, items)
	{
		if(item->checkState(0) == Qt::Checked)
			routerDaemonChecked = true;
	}
	
	if(routerDaemonChecked)
	{
		vmPreview->load(QString::fromUtf8(":/svg/vm_router"));
		vmType->setText(tr("Type: ") + "Router");
	}
	else
	{
		vmPreview->load(QString::fromUtf8(":/svg/vm_host"));
		vmType->setText(tr("Type: ") + "Host");
	}
}

/**
 * [PRIVATE-SLOT]
 * Handle the OK action on this form
 *  -> Check the vm name uniqueness and correctness
 *  -> Foward the request to the undo stack
 */
void AddVmForm::handleAcceptedSignal()
{
	bool allCorrect = true;
	QString newVmName = vmName->text();
	
	/* Empty name? */
	if(newVmName.trimmed() == "")
	{
		/* Show a warning message */
		QMessageBox::warning(this, tr("VisualNetkit - Error"),
				tr("The virtual machine name cannot be empty!"),
				QMessageBox::Ok);
		
		allCorrect = false;
	}
	
	/* ok if don't exist */
	if(vmHandler->vmNameExist(newVmName))
	{
		/* Show a warning message */
		QMessageBox::warning(this, tr("VisualNetkit - Error"),
				tr("The virtual machine name must be unique!\nPlease, retry."),
				QMessageBox::Ok);
		allCorrect = false;
	}
		
	
	/* Clear vm name and close this form */
	if(allCorrect)
	{
		/* Ok, get active daemons and foward the request */
		emit userAddedVm(newVmName, getSelectedDaemons());
		
		vmName->clear();
		close();
	}
}

/**
 * [PRIVATE]
 * Return the list of selected daemons
 */
QList<Daemon> AddVmForm::getSelectedDaemons()
{
	QList<Daemon> activeDaemons;
	
	/* Get all items */
	QList<QTreeWidgetItem *> items = daemonsList->findItems(
			REGEXP_ALL_DAEMONS,	Qt::MatchRegExp | Qt::MatchRecursive, 0);
	
	//scan
	foreach(QTreeWidgetItem *item, items)
	{
		if(item->checkState(0) == Qt::Checked)
			activeDaemons.append((Daemon)item->data(0, Qt::UserRole).toInt());
	}
	
	return activeDaemons;
}
