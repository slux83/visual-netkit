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

#include <QApplication>
#include <QString>
#include <QDebug>
#include <QLibrary>

#include "PluginInterface.h"

void loadPlugin(const char *path)
{
	QLibrary loader(path);

	//load the library
	qDebug() << "starting..." << "library name:" << loader.fileName();

	/* 
	 * Get symbols for create/destroy factories
	 * The library is loaded if needed
	 */
	createPlugin_t* createPluginFactory = (createPlugin_t *)loader.resolve("createPlugin");
	destroyPlugin_t* destroyPluginFactory = (destroyPlugin_t *)loader.resolve("destroyPlugin");
	
	if(!createPluginFactory || !destroyPluginFactory)
	{
		qWarning() << "ERROR: loading factories failed." << loader.errorString();
		return;
	}
	
	qDebug() << "factories are loaded!";
	
	PluginInterface *plugin = createPluginFactory();
	
	qDebug() << "The plugin is instantied!";
	
	qDebug() << "by the plugin loaded:" << plugin->getDescription();
	
	qDebug() << "destroing the Plugin instance";
	
	destroyPluginFactory(plugin);
	
	qDebug() << "Unloading the plugin library";
	
	loader.unload();
	
	qDebug() << "----------------------------------";
	
}

/**
 * An example of an architecture based on plugins
 */
int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	
	loadPlugin("plugins/plugin1/libplugin1.so");
	loadPlugin("plugins/plugin2/libplugin2.so");
	
	return 0;
	
}
