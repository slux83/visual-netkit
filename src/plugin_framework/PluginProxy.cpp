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

#include "PluginProxy.h"
#include "PluginInterface.h"
#include "PluginRegistry.h"

/**
 * Constructor
 */
PluginProxy::PluginProxy()
{
}

/**
 * Deconstructor
 */
PluginProxy::~PluginProxy()
{
}


bool PluginProxy::changeGraphicsLabel(QString label)
{
	return true;
}

bool PluginProxy::saveProperty(QTableWidgetItem* property)
{
	return true;
}

/**
 * Returns the associated plugin properties.
 */
QMap< QString, PluginProperties* > PluginProxy::getPluginProperties()
{
	return pluginInterface->getPluginProperties();
}

/**
 * Returns the plugin template.
 */
QString PluginProxy::getTemplate()
{
	return pluginInterface->getTemplate();
}

/**
 * Returns template location where to save the file.
 */
QString PluginProxy::getTemplateLocation()
{
	return pluginInterface->getTemplateLocation();
}

QSettings* PluginProxy::getPluginSettings()
{
	return pluginInterface->getMySettings();
}

QObject* PluginProxy::getBaseElement()
{
	return PluginRegistry::getInstance()->getBaseElement(this);
}
