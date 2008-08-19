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

#ifndef PLUGINTEST_H_
#define PLUGINTEST_H_

#include <QObject>
#include <QString>
#include "PluginInterface.h"
#include "PluginProxy.h"
#include "PluginProperty.h"
#include "VirtualMachine.h"

/**
 * A simple plugin that do nothing
 */
class PluginTest : public PluginInterface
{
	
private:	
	QList<PluginProperty*> properties;
	QMap<QString, QString> globalInfo;
	PluginProxy *myProxy;
	QString getTemplateLocation();
	void addPropertyChildsDeeply(PluginProperty* prop);
	
	void printProperties(PluginProperty* current = NULL);	//TESTING
	
public:
	PluginTest();
	virtual ~PluginTest();
	bool saveProperty(QString propId, QString propValue, QString *pluginAlertMsg = NULL);
	QMap<QString, QString> getTemplates();
	QString getName() { return globalInfo["plugin name"]; };
	QList<PluginProperty*> getPluginProperties() { return properties; };
	PluginProxy* getProxy() { return myProxy; };
	void setProxy(PluginProxy* p);
	QString getXMLResource() { return ":/test/xml-conf"; };
	QString getDefaultGraphisLabel() {return QString("test plugin"); };
	QString deleteProperty(QString propertyId, quint16 propertyCopy);
	QPair<PluginProperty*, QString> addProperty(QString propertyIdToAdd, QString parentPropertyId,
				quint16 parentPropertyCopy);
	bool init(QString laboratoryPath)
		{ Q_UNUSED(laboratoryPath) myProxy->changeGraphicsLabel("test plugin"); return false; };
};


/******************* PLUGIN FACTORIES ***********************/

/* Factory (creator) */
extern "C" PluginInterface* createPlugin()
{
    return new PluginTest();
}

/* Factory (destroyer) */
extern "C" void destroyPlugin(PluginInterface* p)
{
	delete p;
}


#endif
