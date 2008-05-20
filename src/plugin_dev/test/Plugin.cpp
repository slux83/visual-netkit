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

#include "Plugin.h"
#include "VirtualMachine.h"
#include <QDebug>
#include <QResource>
#include <QFile>

/* constructor */
Plugin::Plugin() : PluginInterface()
{
	mySettings = new QSettings(":/ini", QSettings::NativeFormat);
	myProxy = new PluginProxy();
}


/**
 * Returns the plugin template if resource file exists,
 * otherwise returns an empty QString.
 */
QString Plugin::getTemplate()
{
	QString templateContent;
	
	QFile data(":/basic_tpl");
	if (data.open(QFile::ReadOnly)) 
	{
		QTextStream in(&data);
		templateContent = in.readAll();
	}
	else
	{
		qWarning() << "The plugin getTemplate() failed:" << data.errorString();
	}
		
	return templateContent;
}

/**
 * Get the path where save/append the templace content
 */
QString Plugin::getTemplateLocation()
{
	VirtualMachine *vm = dynamic_cast<VirtualMachine*>(myProxy->getBaseElement());
	if (vm == NULL)
	{
		qWarning() << "Plugin::getTemplateLocation(): null VirtualMachine name.";
		return QString();
	}
	
	return QString(vm->getName() + "/etc/basic.conf");
}
