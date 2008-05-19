/**
 * A simple plugin architecture.
 * Copyright (C) 2008  Alessio Di Fazio
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

#include "Plugin.h"
#include <QDebug>
#include <QResource>
#include <QFile>

/* constructor */
Plugin::Plugin() : PluginInterface()
{
	mySettings = new QSettings(":/ini", QSettings::NativeFormat);
	myProxy = NULL;
	name = "I'm a Plugin";
	description = "I'm doing nothing :3 But i'm here!";
}


/**
 * Returns the plugin template if resource file exists, otherwise returns an empty QString.
 */
QString Plugin::getTemplate()
{
	QResource resource(":/basic_tpl");
	QString tplPath = resource.absoluteFilePath();
	
	// if resource file path is not null
	if (!tplPath.isNull()) 
	{
		QFile data(tplPath);
		if (data.open(QFile::ReadOnly)) 
		{
			// if file exists, read it
			QTextStream in(&data);
			tpl = in.readAll();
		}	
	} else {
		return "";
	}
		
	return tpl;
}
