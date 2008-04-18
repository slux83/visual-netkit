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

#ifndef ADDCDFORM_H_
#define ADDCDFORM_H_

#include <QWidget>
#include <QDebug>
#include <QPointF>
#include "ui_addCd.h"

/**
 * This class implement the form for add collision domain action
 */
class AddCdForm : public QDialog, public Ui::AddCdForm
{
	Q_OBJECT
	
private:
	QPointF cdPos;

public:
	AddCdForm(QWidget *parent = 0);
	virtual ~AddCdForm();
	void setCollisionDomainPos(QPointF pos) { cdPos = pos; };
	
private slots:
	void handleUserConfirm();
};

#endif /*ADDCDFORM_H_*/
