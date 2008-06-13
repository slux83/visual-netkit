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

#ifndef PLUGINPROXY_H_
#define PLUGINPROXY_H_

#include <QString>
#include <QMap>
#include <QSettings>
#include <QObject>
#include <QTableWidgetItem>

#include "../core/VirtualMachine.h"
#include "PluginProperty.h"
#include "PluginInterface.h"

class PluginRegistry;
class PluginInterface;

class PluginProxy : public QObject
{
	Q_OBJECT
	
private:
	PluginInterface *pluginInterface;
	PluginRegistry *registry;
	QString labPath;	//This is updated by the slot setLabPath()

public:
	PluginProxy(PluginRegistry *r);
	virtual ~PluginProxy();

	bool saveProperty(QString propName, QString propValue, QString *pluginAlertMsg = NULL);
	QMap<QString, PluginProperty*> getPluginProperties();
	QMap<QString, QString> getTemplates();
	QSettings* getPluginSettings();
	void setPluginInterface(PluginInterface *pi) { pluginInterface = pi; };
	QObject* getBaseElement();
	PluginInterface * getPlugin() { return pluginInterface; };
	void setPluginGroupID(qint32 id);
	qint32 getPluginGroupID();
	void changeGraphicsLabel(QString label);
	void showDefaultGraphicsLabel();
	void initPlugin(QString path) { pluginInterface->init(path); };
	
signals:
	void needLabelChangedVm(VirtualMachine *vm, QString pluginName, QString label);
	void needLabelChangedHi(HardwareInterface *hi, QString pluginName, QString label);
	void needLabelChangedCd(CollisionDomain *cd, QString pluginName, QString label);
};

#endif /*PLUGINPROXY_H_*/
