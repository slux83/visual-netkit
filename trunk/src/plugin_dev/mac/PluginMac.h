/**
 * A level 2 (MAC) plugin for Visual Netkit 
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

#ifndef PLUGINMAC_H_
#define PLUGINMAC_H_

#include <QObject>
#include <QString>
#include <QMap>

#include "PluginInterface.h"
#include "PluginProxy.h"
#include "PluginProperty.h"


/**
 * MAC (level 2) plugin
 */
class PluginMac : public PluginInterface
{
	
private:
	QList<PluginProperty*> properties;
	QMap<QString, QString> globalInfo;
	PluginProxy *myProxy;

	QString getTemplateLocation();
	void refreshLabel();
	bool validateMacAddress(QString &mac);
	
public:
	PluginMac();
	virtual ~PluginMac();
	bool saveProperty(QString propUniqueId, QString propValue, QString *pluginAlertMsg = NULL);
	QMap<QString, QString> getTemplates();
	QString getName() { return globalInfo["plugin name"]; };
	QList<PluginProperty*> getPluginProperties() { return properties; };
	PluginProxy* getProxy() { return myProxy; };
	void setProxy(PluginProxy* p);
	
	QString getDefaultGraphisLabel() {return QString("mac"); };
	QString getXMLResource() { return ":/mac/xml-conf"; };
	
	//add and delete properties are unused for this plugin
	QString deleteProperty(QString propertyUniqueId)
	{
		Q_UNUSED(propertyUniqueId);
		return QObject::tr("You cannot delete properties for this plugin.");
	};
	QPair<PluginProperty*, QString> addProperty(QString propertyIdToAdd, QString parentPropertyUniqueId);
	
	bool init(QString laboratoryPath);
};


/******************* PLUGIN FACTORIES ***********************/

/* Factory (creator) */
extern "C" PluginInterface* createPlugin()
{
    return new PluginMac();
}

/* Factory (destroyer) */
extern "C" void destroyPlugin(PluginInterface* p)
{
	delete p;
}


#endif
