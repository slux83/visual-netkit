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

#ifndef ADDLINKFORM_H_
#define ADDLINKFORM_H_

#include <QDialog>
#include <QWidget>
#include "VirtualMachineItem.h"
#include "CollisionDomainItem.h"
#include "handles/VmMapper.h"
#include "ui_addLink.h"
#include "../plugin_framework/PluginRegistry.h"

class AddLinkForm : public QDialog, public Ui::AddLinkForm
{
	Q_OBJECT
	
private:
	VirtualMachineItem* vmItem;
	CollisionDomainItem* cdItem;
	QList<PluginLoaderFactory *> availablePlugins;

	void fillPluginChooser();
	QStringList getSelectedPlugins();
	
private slots:
	void handleAccept();
	void showPluginInfos(QListWidgetItem *item);
	
public:
	AddLinkForm(QWidget *parent = 0);
	virtual ~AddLinkForm();
	void updateItems(VirtualMachineItem *vm, CollisionDomainItem* cd);
	
signals:
	void readyToAddLink(VirtualMachineItem *vmItem, CollisionDomainItem *cdItem,
			QStringList selectedPlugins, bool manualInit, QString ethName, bool state);
};

#endif /*ADDLINKFORM_H_*/
