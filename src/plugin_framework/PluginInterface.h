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
#include "PluginProxy.h"

class PluginInterface
{
protected:
	PluginProxy *pluginProxy;
	
public:
	virtual QString getConfigFile() = 0;
	virtual QString getPluginSettings() = 0;
	virtual QString getConfigPath() = 0;
	virtual bool saveProperty(QTableWidgetItem* property) = 0;
	virtual QStettings getQSettings() = 0;
	virtual QString getTemplatePath() = 0;
	virtual QString getTemplate() = 0;
	virtual QMap<QString, QString> getPluginProperties() = 0;
};

#endif /*PLUGIN_INTERFACE_H_*/
