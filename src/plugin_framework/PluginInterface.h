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

#ifndef PLUGIN_INTERFACE_H_
#define PLUGIN_INTERFACE_H_

#include <QTableWidgetItem>
#include <QSettings>

class PluginProxy;
class PluginProperty;

class PluginInterface
{
public:
	virtual ~PluginInterface() {};
	
	/**
	 * *pluginAlertMsg is a pointer to QString to show to the user as alert and
	 * if the returned value is false.
	 * If the pluginAlertMsg doesn't passed (default as NULL), the property is
	 * saved without any consistent check!
	 */
	virtual bool saveProperty(QTableWidgetItem* property, QString *pluginAlertMsg = NULL) = 0;
	
	virtual QSettings* getMySettings() = 0;
	virtual QString getTemplateLocation() = 0;
	virtual QString getTemplate() = 0;
	virtual QMap<QString, PluginProperty*> getPluginProperties() = 0;
	virtual PluginProxy* getProxy() = 0;
	
	//If unused, return -1 inside the plugin implementation
	virtual void setGroupID(qint32 id) = 0;	
	virtual qint32 getGroupID() = 0;
};

/**
 *  the types of factories
 */
typedef PluginInterface* createPlugin_t();
typedef void destroyPlugin_t(PluginInterface*);


#endif /*PLUGIN_INTERFACE_H_*/

