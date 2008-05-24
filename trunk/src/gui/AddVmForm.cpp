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
#include "../plugin_framework/PluginRegistry.h"
#include "../common/Types.h"
#include <QDesktopWidget>
#include <QTreeWidgetItem>
#include <QRect>
#include <QMessageBox>

/**
 * Contructor
 */
AddVmForm::AddVmForm(QWidget *parent) : QWidget(parent)
{
	/* Connect the UI resource to this QWidget */
	setupUi(this);
	
	/* Get the controller instance */
	vmHandler = VmHandler::getInstance();
	
	/* Centering the gui */
	QDesktopWidget dw;
	QRect geometry = dw.screenGeometry();
	setGeometry((geometry.width() / 2) - (width() / 2),		//center X
				(geometry.height() / 2) - (height() / 2),	//center Y
				width(),
				height());
	
	/* Connections */
	connect(addVmButtonBox, SIGNAL(accepted()),
			this, SLOT(handleAcceptedSignal()));
	connect(this, SIGNAL(userAddedVm(QString, QStringList, bool, QPointF)),
			vmHandler, SLOT(createVm(QString, QStringList, bool, QPointF)));
	connect(vmName, SIGNAL(returnPressed()),
			this, SLOT(handleAcceptedSignal()));
	connect(pluginsList, SIGNAL(itemClicked(QListWidgetItem *)),
				this, SLOT(showPluginInfos(QListWidgetItem *)));
	
	/* Init plugin chooser */
	availablePlugins = PluginRegistry::getInstance()->getAllPluginFactories();
	qDebug() << availablePlugins;
	fillPluginChooser();
}

/**
 * Decontructor
 */
AddVmForm::~AddVmForm()
{
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
		/* Ok, get active plugins and foward the request */
		QStringList selPlugins = getSelectedPlugins();
		
		emit userAddedVm(newVmName, selPlugins,
				(initPropertiesCheck->checkState() == Qt::Unchecked), machinePos);
		vmName->clear();
		close();
	}
}

/**
 * [PRIVATE]
 * fill the plugin chooser with a list of checkboxs
 */
void AddVmForm::fillPluginChooser()
{
	QListIterator<PluginLoaderFactory*> it(availablePlugins);
	while(it.hasNext())
	{
		PluginLoaderFactory *factory = it.next();
		
		//it's a mine plugin?
		if(factory->getType() != "vm")
			continue; //Skeep this plugin
		
		//create entry
		QListWidgetItem *pluginItem = new QListWidgetItem();
		pluginItem->setIcon(QIcon(QString::fromUtf8(":/small/plugin")));
		pluginItem->setData(Qt::DisplayRole, factory->getName());	//it's the unique ID
		pluginItem->setData(Qt::ToolTipRole, tr("Select this plugin to show extra infos."));
		pluginItem->setCheckState(Qt::Unchecked);
		
		pluginsList->addItem(pluginItem);
	}
}

/**
 * [PRIVATE-SLOT]
 * show the infos of the selected plugin
 */
void AddVmForm::showPluginInfos(QListWidgetItem *item)
{
	QString selectedPluginName = item->data(Qt::DisplayRole).toString();
	QListIterator<PluginLoaderFactory*> it(availablePlugins);
	while(it.hasNext())
	{
		PluginLoaderFactory *factory = it.next();
		
		if(factory->getName() == selectedPluginName)
		{
			/* Render infos */
			pName->setText(factory->getName());
			pDescription->setText(factory->getDescription());
			pDeps->setText(factory->getDeps());
			pAuthor->setText(factory->getAuthor());
			pVersion->setText(factory->getVersion());
			
			break;
		}
	}
}

/**
 * [PRIVATE]
 * Get the selected plugins
 */
QStringList AddVmForm::getSelectedPlugins()
{
	QStringList selectedPlugins;
	
	/* Get all list items and select only selected */
	QList<QListWidgetItem *> listItems =
		pluginsList->findItems(".+", Qt::MatchRegExp);
	
	QListIterator<QListWidgetItem *> itemIter(listItems);
	while(itemIter.hasNext())
	{
		QListWidgetItem * item = itemIter.next();
		//save the plugin name if the plugin is checked
		if(item->checkState() == Qt::Checked)
			selectedPlugins.append(item->data(Qt::DisplayRole).toString());
	}
	
	return selectedPlugins;
}

