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
	
	connect(this, SIGNAL(userAddCd(QString, QPointF)),
				cdHandler, SLOT(handleAddNewCd(QString, QPointF)));	
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
	
	
	/**
	 * ok, all seems correct.. emit a signal, clear gui and close
	 */
	emit userAddCd(cdName, cdPos);
	
	cdNameLineEdit->clear();
	close();

error:
	return;
	
}
