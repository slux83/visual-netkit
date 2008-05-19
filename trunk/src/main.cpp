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

#include <QThread>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	MainWindow *win = new MainWindow();
	
	/* Create the bug dumper object */
	BugDumper *dumper = new BugDumper();
	Q_UNUSED(dumper);
	
	// Load splashscreen image and window
	QPixmap pixmap(":/splashscreens/ss_00");
	QSplashScreen splash(pixmap);
	splash.show();

	// Loading some items
	splash.showMessage("Loaded modules");
	qDebug() << "Loaded modules";

	app.processEvents();

	// Establishing connections
	splash.showMessage("Established connections");
	qDebug() << "Established connections";
	
	app.processEvents();
	
	for(int i=0; i<150000; i++)
	{
		i--;
		i++;
	}
		
	
	win->show();
	splash.finish(win);
	
	return app.exec();
}
