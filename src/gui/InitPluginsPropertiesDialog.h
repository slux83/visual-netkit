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

#ifndef INITPLUGINSPROPERTIESDIALOG_H_
#define INITPLUGINSPROPERTIESDIALOG_H_

#include <QDebug>
#include <QWidget>
#include "ui_initPluginsProperties.h"
#include "../plugin_framework/PluginLoaderFactory.h"

/**
 * This gui is a dialog that initialize the plugins properties
 */
class InitPluginsPropertiesDialog : public QDialog, public Ui::InitPluginsProperties
{
	Q_OBJECT

private:
	QList<PluginLoaderFactory *> availablePlugins;
	QList<PluginProxy *> pluginsToManage;

public:
	InitPluginsPropertiesDialog(const QList<PluginLoaderFactory *> plugins, QWidget *parent = 0);
	void updatePluginsToolBox(QStringList selectedPlugins);
	virtual ~InitPluginsPropertiesDialog();
	void buildGuiByPlugins(QList<PluginProxy*> plugins);
};

#endif /*INITPLUGINSPROPERTIESDIALOG_H_*/
