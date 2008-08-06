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

#include "InitPluginsPropertiesDialog.h"
#include <QLineEdit>
#include <QScrollArea>
#include <QMessageBox>

#include "../plugin_framework/PluginProxy.h"
#include "../common/CommonConfigs.h"

/**
 * Constructor
 */
InitPluginsPropertiesDialog::InitPluginsPropertiesDialog(
		const QList<PluginLoaderFactory *> plugins, QWidget *parent) : QDialog(parent)
{
	/* Connect the UI resource to this QWidget */
	setupUi(this);
	
	/* Connects */
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(handleUserConfirm()));
	
	/* save the factories (for a fast access) */
	availablePlugins = plugins;
}

/**
 * Decontructor
 */
InitPluginsPropertiesDialog::~InitPluginsPropertiesDialog()
{
	
}


/**
 * [PRIVATE-SLOT]
 * Handle the user form confirm
 * pre-operations on the input
 */
void InitPluginsPropertiesDialog::handleUserConfirm()
{
	QStringList keys = propertiesAssoc.keys();
	
	//for each plugin inside "pluginsToManage"
	for (int j=0; j < pluginsToManage.size(); j++) 
	{
		int yesToAll = 0;
		
		//scan properties
		for (int i=0; i < keys.size(); i++) 
		{
			QStringList l = keys.at(i).split(SEPARATOR);
			QString pluginName = l.first();
			QString propName = l.last();
			
			if (pluginsToManage.at(j)->getPlugin()->getName() == pluginName)
			{
				QString altMsg;
				bool allok = true;
				
				if (yesToAll==QMessageBox::YesToAll)
				{
					pluginsToManage.at(j)->saveProperty(propName, propertiesAssoc.value(keys.at(i))->text());
				}
				else
				{
					allok = pluginsToManage.at(j)->saveProperty(propName, propertiesAssoc.value(keys.at(i))->text(), &altMsg);
				}
				
				// some warning or error returned by initProperty function
				if (!allok && (yesToAll!=QMessageBox::YesToAll))
				{	
					//Prepare the alert message
					QString question =
								tr("The plugin ") + pluginsToManage.at(j)->getPlugin()->getName() +
								tr(" have returned a warning:\n") +
								altMsg + ".\n\n" +
								tr("Ignore this warning?");
					
					yesToAll = QMessageBox::question(this, tr("VisualNetkit - Warning"),
						tr(question.toUtf8()),
						QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::No,
						QMessageBox::Yes);
					
					if(yesToAll == QMessageBox::Yes)
					{
						pluginsToManage.at(j)->saveProperty(propName, propertiesAssoc.value(keys.at(i))->text());
					}
					else if(yesToAll == QMessageBox::YesToAll)
					{
						pluginsToManage.at(j)->saveProperty(propName, propertiesAssoc.value(keys.at(i))->text());
						yesToAll = QMessageBox::YesToAll;
					}
					else
					{
						propertiesAssoc.value(keys.at(i))->selectAll();
						return;
					}
				}
			}
		}
	}

	close();
}

/**
 * Build the widgets based on plugins properties's structure
 */
void InitPluginsPropertiesDialog::buildGui(QList<PluginProxy*> plugins)
{	
	clearAll();
	pluginsToManage = plugins;
	
	QListIterator<PluginProxy *> it(plugins);
	
	/* foreach plugin */
	quint8 tabCount = 0;
	while(it.hasNext())
	{
		// gets the current plugin proxy
		PluginProxy *proxy = it.next();
		
		// if any property has to be set, build the UI
		if(proxy->getPluginProperties().size() > 0)
		{
			QWidget *w = new QWidget();	//the container
			QVBoxLayout *layout = new QVBoxLayout(w);
			layout->setMargin(3);
			layout->setSpacing(3);
			
			QListIterator<PluginProperty*> ii(proxy->getPluginProperties());
			
			/* foraech property */
			while(ii.hasNext())
			{
				PluginProperty *prop = ii.next();
				QLabel *label = new QLabel(prop->getName());
				QLineEdit *lineEdit = new QLineEdit(prop->getDefaultValue());
				lineEdit->setToolTip(prop->getDescription());
				layout->addWidget(label);
				layout->addWidget(lineEdit);
				
				// save mapping; the key is "PluginName|000n|PropertyName
				QString orderString = QString::number(prop->getOrder());
				orderString.prepend(QString().fill('0', 5 - orderString.size()));
								
				propertiesAssoc.insert(proxy->getPlugin()->getName() + 
										SEPARATOR + orderString + 
										SEPARATOR + prop->getName(),
										lineEdit);
			}
			/* The plugin have at most one property */
			QSpacerItem *spacerItem = 
				new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
			layout->addItem(spacerItem);
			pluginsToolBox->insertItem(tabCount++, w, QIcon(QString::fromUtf8(":/small/plugin")),
				proxy->getPlugin()->getName());
		}
	}
}

/**
 * [PRIVATE]
 * Clear all: delete Widgets inside the tool box, and associations map
 */
void InitPluginsPropertiesDialog::clearAll()
{
	/* get all widgets inside the tool box */
	for(int i=0; i<pluginsToolBox->count(); i++)
	{
		//get widget
		QWidget *w = pluginsToolBox->widget(i);
		
		//delete only from toolbox
		pluginsToolBox->removeItem(i);
				
		//destroy widget
		delete w;
		
		/* HACK: delete remaining scroll areas */
		QListIterator<QObject*> iter(pluginsToolBox->children());
		while(iter.hasNext())
		{
			QObject* o = iter.next();
			if(dynamic_cast<QScrollArea*>(o) != NULL)
			{
				delete o;
			}
		}
	}
	
	/* 
	 * Clear and reset the assoc map
	 * NOTE: QLineEdit(s) are deleted by QWidget`s deconstructor
	 */
	propertiesAssoc.clear(); 
}

