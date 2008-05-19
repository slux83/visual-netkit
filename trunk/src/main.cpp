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
	
	/* Load splashscreen */
	QPixmap pixmap(":/splashscreens/ss_10");
	QSplashScreen splash(pixmap);
	splash.setWindowFlags(Qt::WindowStaysOnTopHint);
	splash.setWindowFlags(Qt::SplashScreen);
	splash.show();
	splash.showMessage("Starting...");
	
	/* creating main gui */
	splash.showMessage("Loading main GUI");
	qDebug() << "Loading main GUI";
	MainWindow *win = new MainWindow();
	
	/* Creates the bug dumper object */
	splash.showMessage("Loading bug dumper");
	qDebug() << "Loading bug dumper";
	BugDumper *dumper = new BugDumper();
	Q_UNUSED(dumper);
	
	/* Load plugins */
	splash.showMessage("Loading plugins");
	qDebug() << "Loading plugins";
	PluginRegistry::getInstance()->fetchPlugins();
	
	splash.finish(win);
	win->setWindowState(Qt::WindowMaximized);
	win->show();
	
	
	return app.exec();
}
