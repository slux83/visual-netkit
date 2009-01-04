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

	connect(this,
			SIGNAL(readyToAddLink(VirtualMachineItem *, CollisionDomainItem *, QStringList, bool, QString, bool)),
			LinkHandler::getInstance(),
			SLOT(createLink(VirtualMachineItem *, CollisionDomainItem *, QStringList, bool, QString, bool)));

	connect(pluginsList, SIGNAL(itemClicked(QListWidgetItem *)),
				this, SLOT(showPluginInfos(QListWidgetItem *)));

	/* Init plugin chooser */
	availablePlugins = PluginRegistry::getInstance()->getAllPluginFactories();
	fillPluginChooser();
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
	QStringListIterator iter(VmMapper::getInstance()->getMachineInterfaces(vm));


	/* Render interfaces inside the table [NAME] */
	int row = 0;
	//Clear all table items and reset the view-size
	interfacesTable->clearContents();
	interfacesTable->setRowCount(0);		//resize (reset) the view
	while(iter.hasNext())
	{
		interfacesTable->setRowCount(interfacesTable->rowCount() + 1);
		QTableWidgetItem *ethName = new QTableWidgetItem();
		ethName->setData(Qt::DisplayRole, iter.next());
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

	/* Ok, get active plugins and foward the request */
	QStringList selPlugins = getSelectedPlugins();

	//emit signal and close the gui
	emit readyToAddLink(vmItem, cdItem, selPlugins, (initPropertiesCheck->checkState() == Qt::Unchecked),
			QString(ethNumberSpinBox->prefix() + QString::number(ethNumberSpinBox->value())),
			ethState);

	close();
}

/**
 * [PRIVATE]
 * fill the plugin chooser with a list of checkboxs
 */
void AddLinkForm::fillPluginChooser()
{
	QListIterator<PluginLoaderFactory*> it(availablePlugins);
	while(it.hasNext())
	{
		PluginLoaderFactory *factory = it.next();

		//it's a mine plugin?
		if(factory->getType() != "link")
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
void AddLinkForm::showPluginInfos(QListWidgetItem *item)
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
			pDeps->setText(factory->getDeps().join(", "));
			pAuthor->setText(factory->getAuthor());
			pVersion->setText(factory->getVersion());

			/* control dependencies if it's checked */
			if(item->checkState() == Qt::Checked)
			{
				QStringList notFoundDeps;
				//select all deps
				foreach(QString dep, factory->getDeps())
				{
					QList<QListWidgetItem *> listItems =
						pluginsList->findItems(dep, Qt::MatchExactly);
					if(listItems.size() == 0)
						notFoundDeps << dep;	//save the not founded dep
					else
						listItems.first()->setCheckState(Qt::Checked);		//check the dep
				}

				//we have not found dependencies??
				if(!notFoundDeps.isEmpty())
					QMessageBox::warning(this, "Warning",
							tr("There are dependency problems") + "\nPlugins (" + notFoundDeps.join(", ") + ") not found.");
			}

			break;
		}
	}
}

/**
 * [PRIVATE]
 * Get the selected plugins
 */
QStringList AddLinkForm::getSelectedPlugins()
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
