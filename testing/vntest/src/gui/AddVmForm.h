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

#ifndef ADDVMFORM_H_
#define ADDVMFORM_H_

#include <QWidget>
#include <QSvgWidget>
#include <QSpacerItem>
#include <QDebug>
#include "ui_addVm.h"
#include "handles/VmHandler.h"

/**
 * This class implement the form for add vm action
 */
class AddVmForm : public QWidget, public Ui::AddVmForm
{
	Q_OBJECT
	
private:
	QSpacerItem *spacePreview;
	QSvgWidget *vmPreview;
	QLabel *vmType;
	VmHandler *vmHandler;
	
	void fillDaemonsList();
	QList<Daemon> getSelectedDaemons();

private slots:
	void updateVmPreview(QTreeWidgetItem * item, int column);
	void handleAcceptedSignal();
	
public:
	AddVmForm(QWidget *parent = 0);
	virtual ~AddVmForm();
	
signals:
	void userAddedVm(QString vmNewName, QList<Daemon> activeDaemons);
};

#endif /*ADDVMFORM_H_*/
