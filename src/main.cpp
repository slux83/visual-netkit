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

#include <QApplication>
#include <QSplashScreen>
#include "gui/MainWindow.h"
#include "common/BugDumper.h"
#include "plugin_framework/PluginRegistry.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
	// Load splashscreen
	QPixmap pixmap(":/splashscreens/ss_00");
	QSplashScreen splash(pixmap);
	splash.setWindowFlags(Qt::WindowStaysOnTopHint);
	splash.setWindowFlags(Qt::SplashScreen);
	splash.show();
	
	//creating main gui
	MainWindow *win = new MainWindow();
	splash.showMessage("Loading main GUI");
	qDebug() << "Loading main GUI";
	
	/* Creates the bug dumper object */
	BugDumper *dumper = new BugDumper();
	Q_UNUSED(dumper);
	splash.showMessage("Loading bug dumper");
	qDebug() << "Loading bug dumper";
	
	PluginRegistry::getInstance()->fetchPlugins();
	splash.showMessage("Loading plugins");
	qDebug() << "Loading plugins";
	
	sleep(5);
	
	win->show();
	splash.finish(win);
	
	return app.exec();
}
