/**
 * The IPv4 plugin for Visual Netkit 
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

#ifndef PLUGINIPV4_H_
#define PLUGINIPV4_H_

#include <QObject>
#include <QString>
#include <QSettings>

#include "PluginInterface.h"
#include "PluginProxy.h"
#include "PluginProperty.h"

#include "NetworkAddress.h"

//true if the netmask must be printed in CIDR notation, false otherwise
#define PRINT_CIDR_NETMASK true

/**
 * An IPv4 plugin
 */
class PluginIPv4 : public PluginInterface
{
	
private:
	QList<PluginProperty*> properties;
	PluginProxy *myProxy;
	QMap<QString, QString> globalInfo;

	QString getTemplateLocation();
	void refreshLabel();
	
public:
	PluginIPv4();
	virtual ~PluginIPv4();
	bool saveProperty(QString propUniqueId, QString propValue, QString *pluginAlertMsg = NULL);
	QMap<QString, QString> getTemplates();
	QString getName() { return globalInfo["plugin name"]; };
	QList<PluginProperty*> getPluginProperties() { return properties; };
	PluginProxy* getProxy() { return myProxy; };
	void setProxy(PluginProxy* p);
	
	QString getDefaultGraphisLabel() {return QString("ipv4"); };
	QString getXMLResource() { return ":/ipv4/xml-conf"; };
	
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
    return new PluginIPv4();
}

/* Factory (destroyer) */
extern "C" void destroyPlugin(PluginInterface* p)
{
	delete p;
}


#endif
