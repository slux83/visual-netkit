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

/**
 * Constructor
 */
InitPluginsPropertiesDialog::InitPluginsPropertiesDialog(
		const QList<PluginLoaderFactory *> plugins, QWidget *parent) : QDialog(parent)
{
	/* Connect the UI resource to this QWidget */
	setupUi(this);
	
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
		PluginProxy *proxy = it.next();
		
		if(proxy->getPluginProperties().values().size() > 0)
		{
			QWidget *w = new QWidget();	//the container
			QVBoxLayout *layout = new QVBoxLayout(w);
			layout->setMargin(3);
			layout->setSpacing(3);
			
			QListIterator<PluginProperty*> ii(proxy->getPluginProperties().values());
			/* foraech property */
			while(ii.hasNext())
			{
				PluginProperty *prop = ii.next();
				QLabel *label = new QLabel(prop->getName());
				QLineEdit *lineEdit = new QLineEdit(prop->getDefaultValue());
				lineEdit->setToolTip(prop->getDescription());
				layout->addWidget(label);
				layout->addWidget(lineEdit);
				
				//save mapping
				propertiesAssoc.insert(proxy->getPlugin()->getName() + 
										QString('\0') + prop->getName(),
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
