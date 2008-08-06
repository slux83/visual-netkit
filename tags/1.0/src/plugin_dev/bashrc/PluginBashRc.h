/**
 * A simple Wiki (how-to) plugin for Visual Netkit 
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

#ifndef PLUGINBASHRC_H_
#define PLUGINBASHRC_H_

#include <QObject>
#include <QString>
#include <QSettings>
#include "PluginInterface.h"
#include "PluginProxy.h"
#include "PluginProperty.h"
#include "VirtualMachine.h"

/**
 * A simple plugin that create a modular .bashrc
 */
class PluginBashRc : public PluginInterface
{
	
private:
	QMap<QString, PluginProperty*> properties;
	QSettings *mySettings;
	PluginProxy *myProxy;
	QString myName;

	void fetchProperties();
	QString getTemplateLocation();
	
public:
	PluginBashRc();
	virtual ~PluginBashRc();
	bool saveProperty(QString propName, QString propValue, QString *pluginAlertMsg = NULL);
	QSettings* getMySettings() { return mySettings; };
	QMap<QString, QString> getTemplates();
	QString getName() { return myName; };
	QList<PluginProperty*> getPluginProperties() { return properties.values(); };
	PluginProxy* getProxy() { return myProxy; };
	void setProxy(PluginProxy* p) { myProxy = p; };
	
	QString getDefaultGraphisLabel() {return QString(".bashrc"); };
	
	/* Do nothing on init. Just show a label "/root/.bashrc" */ 
	bool init(QString laboratoryPath)
		{ Q_UNUSED(laboratoryPath) myProxy->changeGraphicsLabel(".bashrc"); return false; };
};


/******************* PLUGIN FACTORIES ***********************/

/* Factory (creator) */
extern "C" PluginInterface* createPlugin()
{
    return new PluginBashRc();
}

/* Factory (destroyer) */
extern "C" void destroyPlugin(PluginInterface* p)
{
	delete p;
}


#endif
