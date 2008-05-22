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

#ifndef PLUGINPROPERTIES_H_
#define PLUGINPROPERTIES_H_

#include <QString>

class PluginProperties
{
private:
	QString name;
	QString defaultValue;
	QString description;
	QString value;
	
public:
	PluginProperties();
	PluginProperties(QString pluginName, QString defValue, QString desc);
	PluginProperties(QString pluginName, QString defValue, QString initValue, QString desc);
	virtual ~PluginProperties();
	
	QString getName() { return name; };
	QString getDefaultValue() { return defaultValue; };
	QString getDescription() { return description; };
	QString getValue() { return value; };
	
	void setName(QString &n) { name = n; };
	void setDefaultValue(QString &d) { defaultValue = d; };
	void setValue(QString &d) { value = d; };
	void setDescription(QString &d) { description = d; };
};

#endif /*PLUGINPROPERTIES_H_*/
