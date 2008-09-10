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

#ifndef PLUGINLOADERFACTORY_H_
#define PLUGINLOADERFACTORY_H_

#include <QLibrary>
#include <QObject>
#include <QSettings>
#include <QDebug>
#include "PluginInterface.h"
#include "PluginProperty.h"
#include "PluginProxy.h"

/**
 * This clas is a loader for a single external plugin
 */
class PluginLoaderFactory : public QLibrary
{
	
private:
	createPlugin_t* createPluginFactory;
	destroyPlugin_t* destroyPluginFactory;
	QString name, description, version, author, type;
	QStringList deps;
	QList<PluginProperty*> properties;	//a copy for a fast access (unused)
	
public:
	PluginLoaderFactory(const QString &fileName, QObject *parent = 0);
	virtual ~PluginLoaderFactory();
	PluginProxy* createPlugin();
	void destroyPlugin(PluginProxy *p);
	bool initPluginLibrary();
	QList<PluginProperty*> getPropertiesDescription() { return properties; };
	
	/* Getter functions */
	QString getType() { return type; };
	QString getName() { return name; };
	QString getDescription() { return description; };
	QString getVersion() { return version; };
	QStringList getDeps() { return deps; };
	QString getAuthor() { return author; };
	
};

#endif /*PLUGINLOADERFACTORY_H_*/
