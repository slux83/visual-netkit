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
#include "../common/NetworkAddress.h"
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
	labelCd->setText(cd->getLabel());
	
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
}

/**
 * [PRIVATE-SLOT]
 * Handle accept()
 */
void AddLinkForm::handleAccept()
{
	
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
	
	/* validate netmask and ip address */
	if(!NetworkAddress::validateIp(ip) ||
			!NetworkAddress::validateNetmask(QHostAddress(netmask)))
	{
		broadcastLineEdit->setText(tr("invalid ip/netmask"));
		return;
	}
	
	/* 
	 * check if the ip is conform with the collision domain subnet
	 * comparing the two broadcast addresses 
	 */
	NetworkAddress cdNetwork = CdMapper::getInstance()->getNetworkAddress(cdItem);
	if(NetworkAddress::generateBroadcast(cdNetwork.ip(), cdNetwork.netmask()) !=
		NetworkAddress::generateBroadcast(QHostAddress(ip), QHostAddress(netmask)))
	{
		broadcastLineEdit->setText(tr("ip/netmask must be the compatible with the subnet: ") + cdNetwork.toString(true));
		return;
	}
	else
	{
		broadcastLineEdit->setText(
			NetworkAddress::generateBroadcast(QHostAddress(ip), QHostAddress(netmask)).toString());
		
		return;
	}
}
