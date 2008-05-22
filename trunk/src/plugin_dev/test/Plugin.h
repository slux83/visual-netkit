/**
 * A simple test plugin for Visual Netkit 
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

#ifndef PLUGIN_H_
#define PLUGIN_H_

#include <QObject>
#include <QString>
#include <QSettings>
#include "../../plugin_framework/PluginInterface.h"
#include "../../plugin_framework/PluginProxy.h"
#include "../../plugin_framework/PluginProperties.h"

/**
 * A simple plugin that do nothing
 */
class Plugin : public PluginInterface
{
	
private:
	QMap<QString, PluginProperties*> properties;
	QSettings *mySettings;
	PluginProxy *myProxy;
	QString myName;

	bool fetchProperties();
	
public:
	Plugin();
	virtual ~Plugin();
	bool saveProperty(QTableWidgetItem* property) { return false; };
	QSettings* getMySettings() { return mySettings; };
	QString getTemplateLocation();
	QString getTemplate();
	QMap<QString, PluginProperties*> getPluginProperties() { return properties; };
	PluginProxy* getProxy() { return myProxy; };
	
	void setGroupID(qint32 id) { Q_UNUSED(id) /* do nothing */ };	
	qint32 getGroupID() { return -1; /* Unused */ };
};


/******************* PLUGIN FACTORIES ***********************/

/* Factory (creator) */
extern "C" PluginInterface* createPlugin()
{
    return new Plugin();
}

/* Factory (destroyer) */
extern "C" void destroyPlugin(PluginInterface* p)
{
	delete p;
	qDebug() << "Plugin destroyed";
}


#endif
