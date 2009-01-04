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
#include "../plugin_framework/PluginRegistry.h"
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

	connect(this, SIGNAL(userAddCd(QString, QStringList, bool, QPointF)),
				cdHandler, SLOT(handleAddNewCd(QString, QStringList, bool, QPointF)));

	connect(pluginsList, SIGNAL(itemClicked(QListWidgetItem *)),
					this, SLOT(showPluginInfos(QListWidgetItem *)));

	/* Init plugin chooser */
	availablePlugins = PluginRegistry::getInstance()->getAllPluginFactories();
	fillPluginChooser();
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
	bool ok = true;
	QRegExpValidator nameValidator(QRegExp("^[a-zA-Z0-9]+$"), this);
	int pos = 0;

	/* validate name */
	if(nameValidator.validate(cdName, pos) != QValidator::Acceptable)
	{
		/* Show a warning message */
		QMessageBox::warning(this, tr("VisualNetkit - Error"),
				tr("The collision domain name must match ^[a-zA-Z0-9]+$"),
				QMessageBox::Ok);

		ok = false;
	}

	if(ok && CdHandler::getInstance()->cdNameExist(cdName))
	{
		/* Show a warning message */
		QMessageBox::warning(this, tr("VisualNetkit - Error"),
				tr("The collision domain name must be unique!\nPlease, retry."),
				QMessageBox::Ok);

		ok = false;
	}


	/**
	 * ok, all seems correct.. emit a signal, clear gui and close
	 */
	if(ok)
	{
		QStringList selPlugins = getSelectedPlugins();

		emit userAddCd(cdName, selPlugins,
				(initPropertiesCheck->checkState() == Qt::Unchecked), cdPos);

		cdNameLineEdit->clear();
		close();
	}

}

/**
 * [PRIVATE]
 * fill the plugin chooser with a list of checkboxs
 */
void AddCdForm::fillPluginChooser()
{
	QListIterator<PluginLoaderFactory*> it(availablePlugins);
	while(it.hasNext())
	{
		PluginLoaderFactory *factory = it.next();

		//it's a mine plugin?
		if(factory->getType() != "cd")
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
void AddCdForm::showPluginInfos(QListWidgetItem *item)
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
QStringList AddCdForm::getSelectedPlugins()
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

