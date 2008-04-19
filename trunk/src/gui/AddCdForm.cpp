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

#include "AddCdForm.h"
#include "handles/CdHandler.h"

#include <QDesktopWidget>
#include <QStringList>
#include <QHostAddress>
#include <QMessageBox>

/**
 * Constructor
 */
AddCdForm::AddCdForm(QWidget *parent) : QDialog(parent)
{
	/* Connect the UI resource to this QWidget */
	setupUi(this);
	
	/* Centering the gui */
	QDesktopWidget dw;
	QRect geometry = dw.screenGeometry();
	setGeometry((geometry.width() / 2) - (width() / 2),		//center X
				(geometry.height() / 2) - (height() / 2),	//center Y
				width(),
				height());
	
	cdHandler = CdHandler::getInstance();
	
	/* Connects */
	connect(cdFormConfirmBox, SIGNAL(accepted()),
			this, SLOT(handleUserConfirm()));
	
	connect(this, SIGNAL(userAddCd(QString, NetworkAddress *, QPointF)),
				cdHandler, SLOT(handleAddNewCd(QString, NetworkAddress *, QPointF)));	
}

/**
 * Deconstructor
 */
AddCdForm::~AddCdForm()
{
}

/**
 * [PRIVATE-SLOT]
 * Handle the user form confirm
 * pre-operations on the input
 */
void AddCdForm::handleUserConfirm()
{
	QString cdName = cdNameLineEdit->text();
	QStringList subnet = subnetLineEdit->text().split("/");
	NetworkAddress *cdSubnet = new NetworkAddress();
	quint8 cidrNetmask = 0;
	
	/*
	 * Checks on cd name
	 */
	if(cdName.trimmed() == "")
	{
		/* Show a warning message */
		QMessageBox::warning(this, tr("VisualNetkit - Error"),
				tr("The collision domain name cannot be empty!"),
				QMessageBox::Ok);
		
		goto error;
	}
	
	if(CdHandler::getInstance()->cdNameExist(cdName))
	{
		/* Show a warning message */
		QMessageBox::warning(this, tr("VisualNetkit - Error"),
				tr("The collision domain name must be unique!\nPlease, retry."),
				QMessageBox::Ok);
		
		goto error;
	}
	
	/*
	 * Checks on subnet
	 */
	if(subnet.size() != 2)
	{	
		/* Show a warning message */
		QMessageBox::warning(this, tr("VisualNetkit - Error"),
				tr("Subnet not valid."),
				QMessageBox::Ok);
		
		goto error;
	}
	else
	{
		QString networkString = subnet[0];
		QString netmaskString = subnet[1];
		
		/* CIDR notation for the netmask? */
		bool isInt;
		cidrNetmask = netmaskString.toInt(&isInt);
		
		/* Maybe the netmask is in normal notation? 255.255.0.0 */
		if(!isInt)
		{
			QHostAddress netmask(netmaskString);
			qDebug() << netmask << netmaskString;
			if(!NetworkAddress::validateNetmask(netmask))
			{
				/* Show a warning message */
				QMessageBox::warning(this, tr("VisualNetkit - Error"),
						tr("Invalid netmask."),
						QMessageBox::Ok);
				
				goto error;
			}
			else //Netmask ok
			{
				cdSubnet->setNetmask(QHostAddress(netmaskString));				
			}				
			
		}
		else
		{
			/* Validate cidr netmask */
			if(cidrNetmask > 32 || cidrNetmask < 1)			
			{
				/* Show a warning message */
				QMessageBox::warning(this, tr("VisualNetkit - Error"),
						tr("Invalid netmask.\nIn the CIDR nonation the netmask is included between 1 and 32."),
						QMessageBox::Ok);
				
				goto error;	
				
			}
			else	//save the netmask
			{
				cdSubnet->setCidrNetmask(cidrNetmask);
			}
				
		}
		
		/* Check the network */
		if(!NetworkAddress::validateIp(networkString))
		{
			/* Show a warning message */
			QMessageBox::warning(this, tr("VisualNetkit - Error"),
					tr("Invalid Network."),
					QMessageBox::Ok);
			
			goto error;				
		}
		
		/* 
		 * Network is inside the netmask? ok, transale it
		 * (10.0.1.3/24 -> 10.0.1.0/24) 
		 */
		if(NetworkAddress::toGeneralNetwork(QHostAddress(networkString), cdSubnet->netmask())
			!= QHostAddress(networkString))
		{
			QString question =
						"You have inserted this subnet: " + networkString + "/"
						+ cdSubnet->netmask().toString()
						+ "\nThe correct network for the gived netmask is:\n"
						+ NetworkAddress::toGeneralNetwork(QHostAddress(networkString), cdSubnet->netmask()).toString()
						+ "\n\nContinue?";
			
			int ret = QMessageBox::question(this, tr("VisualNetkit - Warning"),
				tr(question.toUtf8()),
				QMessageBox::Yes | QMessageBox::No,
				QMessageBox::Yes);
			
			if(ret == QMessageBox::Yes)
			{
				cdSubnet->setIp(NetworkAddress::toGeneralNetwork(QHostAddress(networkString), cdSubnet->netmask()));
			}
			else
				goto error;
		}
		else
			cdSubnet->setIp(QHostAddress(networkString));
			
	}
	
	/**
	 * ok, all seems correct.. emit a signal, clear gui and close
	 */
	emit userAddCd(cdName, cdSubnet, cdPos);
	
	cdNameLineEdit->clear();
	subnetLineEdit->clear();
	close();

error:
	return;
	
}
