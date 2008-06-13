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
	 * pluginAlertMsg is a pointer to QString to show to the user an alert
	 * if the returned value is false.
	 * If it is initialized, the function returns the result of consistency check.
	 * If any check is failed, it returns false and an error string.
	 * If the pluginAlertMsg doesn't passed (default as NULL) the property is
	 * saved without any consistent check, and allways return TRUE.
	 */
	virtual bool saveProperty(QString propName, QString propValue, QString *pluginAlertMsg = NULL) = 0;
	
	/**
	 * Get a QSettings* that contains the description of the plugin ini file
	 */
	virtual QSettings* getMySettings() = 0;
	
	/**
	 * Get a map of templates: the KEY is the template path relative to the laboratory
	 * root; the Value represent the content of the file.
	 */
	virtual QMap<QString, QString> getTemplates() = 0;
	
	/**
	 * This function return a map of properties owned by this plugin
	 */
	virtual QMap<QString, PluginProperty*> getPluginProperties() = 0;
	
	/**
	 * Return my proxy
	 */
	virtual PluginProxy* getProxy() = 0;
	
	/**
	 * Set my proxy
	 * Called by the system at initialization time
	 */
	virtual void setProxy(PluginProxy* p) = 0;
	
	/**
	 * This function is the way that the system use to know the element group.
	 * Default unused and return -1
	 */
	virtual void setGroupID(qint32 id) { Q_UNUSED(id) };
	virtual qint32 getGroupID() { return -1; };
	
	/**
	 * This function is called by the system (through the proxy) when the plugin
	 * is created and attached to a base element. If you don't want show a label
	 * on plugin startup, you have to return a empty QString()
	 */
	virtual QString getDefaultGraphisLabel() = 0;
	
	/**
	 * Get the name of this plugin, normally it's the name inside the plugin.ini
	 * file
	 */
	virtual QString getName() = 0;
	
	/**
	 * This function is called by the system (through the proxy) when the plugin
	 * is created and attached to a base element during laboratory open.
	 * Normally inside this function you have to parse config files (lab path is
	 * passed as argument), initialize properties and finally if you want you can
	 * set your own graphics label, through the proxy changeGraphicsLabel()
	 * function.
	 * 
	 * The default implementation do nothing.
	 */
	virtual void init(QString laboratoryPath) { Q_UNUSED(laboratoryPath) };
	
};

/**
 *  the types of factories
 */
typedef PluginInterface* createPlugin_t();
typedef void destroyPlugin_t(PluginInterface*);


#endif /*PLUGIN_INTERFACE_H_*/

