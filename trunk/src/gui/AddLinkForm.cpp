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
	
	connect(this, SIGNAL(readyToAddLink(VirtualMachineItem *, CollisionDomainItem *, QString, bool)),
			LinkHandler::getInstance(), SLOT(createLink(VirtualMachineItem *, CollisionDomainItem *, QString, bool)));
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
	
	
	/* Render interfaces inside the table [NAME] */
	int row = 0;
	//Clear all table items and reset the view-size
	interfacesTable->clearContents();
	interfacesTable->setRowCount(0);		//resize (reset) the view	
	while(iter.hasNext())
	{
		iter.next();
		interfacesTable->setRowCount(interfacesTable->rowCount() + 1);
		QTableWidgetItem *ethName = new QTableWidgetItem();
		ethName->setData(Qt::DisplayRole, iter.key());
		interfacesTable->setItem(row, 0, ethName);
		row++;
	}
}

/**
 * [PRIVATE-SLOT]
 * Handle accept()
 */
void AddLinkForm::handleAccept()
{
	QRegExp validateEthName("^eth[0-9]+");
	
	/**
	 * test the interface name
	 */
	if(VmMapper::getInstance()->getMachineInterfaces(vmItem).contains(ethNumberSpinBox->prefix() + QString::number(ethNumberSpinBox->value())))
	{
		/* Show a warning message */
		QMessageBox::warning(this, tr("VisualNetkit - Error"),
			tr("The interface name must be unique!\nPlease, retry."),
			QMessageBox::Ok);
		
		return;
	}
	
	//if up --> state true, false otherwise
	bool ethState = ethUp->isChecked();
	
	//emit signal and close the gui
	emit readyToAddLink(vmItem, cdItem, 
			QString(ethNumberSpinBox->prefix() + QString::number(ethNumberSpinBox->value())),
			ethState);
	
	close();
}

