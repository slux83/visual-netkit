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

#include "AddLinkForm.h"
#include "handles/CdMapper.h"
#include "handles/LinkHandler.h"
#include <QDesktopWidget>
#include <QHeaderView>

/**
 * Constructor
 */
AddLinkForm::AddLinkForm(QWidget *parent) : QDialog(parent)
{
	/* Connect the UI resource to this QDialog */
	setupUi(this);
	
	/* Centering the gui */
	QDesktopWidget dw;
	QRect geometry = dw.screenGeometry();
	setGeometry((geometry.width() / 2) - (width() / 2),		//center X
				(geometry.height() / 2) - (height() / 2),	//center Y
				width(),
				height());

	
	cdItem = NULL;
	vmItem = NULL;
	
	interfacesTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	
	/* connects */
	connect(addLinkButtonBox, SIGNAL(accepted()),
			this, SLOT(handleAccept()));
	
	connect(ipLineEdit, SIGNAL(textChanged(const QString &)),
			this, SLOT(validateIp(const QString &)));
	
	connect(this, SIGNAL(readyToAddLink(VirtualMachineItem *, CollisionDomainItem *, QString, bool, NetworkAddress)),
			LinkHandler::getInstance(), SLOT(createLink(VirtualMachineItem *, CollisionDomainItem *, QString, bool, NetworkAddress)));
}

/**
 * Deconstructor
 */
AddLinkForm::~AddLinkForm()
{
}

/**
 * Update items and gui labels/infos
 */
void AddLinkForm::updateItems(VirtualMachineItem *vm, CollisionDomainItem* cd)
{
	vmItem = vm;
	cdItem = cd;
	
	//just a test
	if(vmItem == NULL || cdItem == NULL)
	{
		qWarning() << "AddLinkForm: vmItem or cdItem is null";
		return;
	}
	
	//update gui
	labelVm->setText(vm->getLabel());
	labelCd->setText(cd->getLabel() + " (" +
			CdMapper::getInstance()->getNetworkAddress(cd).netmask().toString() + ")");
	
	//Set the same ip/netmask of the collision domain selected
	ipLineEdit->setText(CdMapper::getInstance()->getNetworkAddress(cd).toString(false));
	
	/* Get interfaces for the vm */
	QMapIterator<QString, QString> iter(VmMapper::getInstance()->getMachineInterfaces(vm));
	
	
	/* Render interfaces inside the table [NAME | IP] */
	int row = 0;
	//Clear all table items and reset the view-size
	interfacesTable->clearContents();
	interfacesTable->setRowCount(0);		//resize (reset) the view	
	while(iter.hasNext())
	{
		iter.next();
		interfacesTable->setRowCount(interfacesTable->rowCount() + 1);
		QTableWidgetItem *ethName = new QTableWidgetItem();
		QTableWidgetItem *ethContent = new QTableWidgetItem();
		ethName->setData(Qt::DisplayRole, iter.key());
		ethContent->setData(Qt::DisplayRole, iter.value());
		interfacesTable->setItem(row, 0, ethName);
		interfacesTable->setItem(row, 1, ethContent);
		row++;
	}
	
	/* Render addresses allready connected to the collision domain */
	addressesListWidget->clear();
	addressesListWidget->addItems(CdMapper::getInstance()->getUsedAddresses(cdItem));
}

/**
 * [PRIVATE-SLOT]
 * Handle accept()
 */
void AddLinkForm::handleAccept()
{
	QStringList splitted = ipLineEdit->text().split("/");
	
	/* get ip and netmask */
	QString ip = splitted[0];
	QString netmask = splitted[1];
	
	/**
	 * if the broadcast in correct, also the ip/netmask it is
	 */
	if(!NetworkAddress::validateIp(broadcastLineEdit->text()))
	{
		/* Show a warning message */
		QMessageBox::warning(this, tr("VisualNetkit - Error"),
			tr("The ip/netmask seems to be incorrect.\nPlease, retry."),
			QMessageBox::Ok);
				
		return;
	}
	
	/**
	 * test the interface name
	 */
	if(ethNameLineEdit->text().trimmed() == "" ||
			VmMapper::getInstance()->getMachineInterfaces(vmItem).contains(ethNameLineEdit->text().trimmed()))
		
	{
		/* Show a warning message */
		QMessageBox::warning(this, tr("VisualNetkit - Error"),
			tr("The interface name must be unique and not empty!\nPlease, retry."),
			QMessageBox::Ok);
		
		return;
	}
	
	//if up --> state true, false otherwise
	bool ethState = ethUp->isChecked();
	
	NetworkAddress address;
	bool cidrNetmask;
	netmask.toInt(&cidrNetmask);
	
	//cidr or normal netmask?
	if(cidrNetmask)
		address = NetworkAddress(QHostAddress(ip), netmask.toInt());
	else
		address = NetworkAddress(QHostAddress(ip), QHostAddress(netmask));
	
	address.setBroadcast(QHostAddress(broadcastLineEdit->text()));
	
	//emit signal and close the gui
	emit readyToAddLink(vmItem, cdItem, ethNameLineEdit->text().trimmed(),
			ethState, address);
	
	close();
}

/**
 * [PRIVATE-SLOT]
 * Check the inserted ip address
 */
void AddLinkForm::validateIp(const QString &text)
{
	QStringList splitted = text.split("/");
	if(splitted.size() != 2)
	{
		broadcastLineEdit->setText(tr("invalid ip/netmask"));
		return;
	}
	
	/* get ip and netmask */
	QString ip = splitted[0];
	QString netmask = splitted[1];
	
	bool isCidr;
	int cidrNetmask = netmask.toInt(&isCidr);
	
	/* the netmask is in cidr notation? */
	if(isCidr && cidrNetmask >= 1 && cidrNetmask <=32)
	{
		netmask = NetworkAddress::cidr2netmask(cidrNetmask).toString();
	}
	
	/* validate netmask and ip address */
	if(!NetworkAddress::validateIp(ip) ||
			!NetworkAddress::validateNetmask(QHostAddress(netmask)))
	{
		broadcastLineEdit->setText(tr("invalid ip/netmask"));
		return;
	}
	
	/* 
	 * check if the ip is conform with the collision domain subnet
	 * comparing the two broadcast addresses and verifying the network compatibility
	 */
	NetworkAddress cdNetwork = CdMapper::getInstance()->getNetworkAddress(cdItem);
	
	if((NetworkAddress::generateBroadcast(cdNetwork.ip(), cdNetwork.netmask()) !=
		NetworkAddress::generateBroadcast(QHostAddress(ip), QHostAddress(netmask))) ||
		(QHostAddress(ip) == cdNetwork.ip()))
	{
		broadcastLineEdit->setText(tr("ip/netmask incompatible with ") + cdNetwork.toString(true));
		return;
	}
	else
	{
		/* allready used ip? */
		if(CdMapper::getInstance()->getUsedAddresses(cdItem).contains(ip))
			broadcastLineEdit->setText(tr("You have tu use a free address."));
		else
			broadcastLineEdit->setText(
					NetworkAddress::generateBroadcast(QHostAddress(ip), QHostAddress(netmask)).toString());
		
		return;
	}
}
