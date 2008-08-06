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
#include <QProcess>

#include "gui/MainWindow.h"
#include "common/BugDumper.h"
#include "plugin_framework/PluginRegistry.h"

/**
 * The main ^^'
 */
int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
	/* Create the home dir (if not exist) */
	QDir visualNetkitHome(QDir::homePath().append("/.visualnetkit"));
	if(!visualNetkitHome.exists())
	{
		if(!visualNetkitHome.mkpath(visualNetkitHome.absolutePath().append("/plugins")))
		{
			qWarning() << "Cannot create the application home dir:" << visualNetkitHome.absolutePath();
			qWarning() << "ABORTING...";
			app.exit(1);
		}
		
		qDebug() << "Created the home dir:" << visualNetkitHome.absolutePath();
		
	}
	
	LabFacadeController::getInstance()->setAppHome(visualNetkitHome);
	
	/* Load splashscreen */
	QPixmap pixmap(":/splashscreens/ss");
	QSplashScreen splash(pixmap);
	splash.setWindowFlags(Qt::WindowStaysOnTopHint);
	splash.setWindowFlags(Qt::SplashScreen);
	splash.show();
	splash.showMessage("Starting...");
	
	/* Load plugins */
	splash.showMessage("Loading plugins");
	qDebug() << "Loading plugins";
	QStringList env = QProcess::systemEnvironment();
	QRegExp envRegExp("VISUAL_NETKIT_PLUGINS=(.+)");
	QStringList paths;	//all plugin paths
	
	//search the env var
	bool foundEnv = false;
	foreach(QString envLine, env)
	{
		if(envRegExp.exactMatch(envLine))
		{
			envRegExp.indexIn(envLine);
			paths = envRegExp.capturedTexts()[1].split(":", QString::SkipEmptyParts);
			foundEnv = true;
			break;
		}
	}
	
	//add plugin paths to the registry
	foreach(QString path, paths)
		PluginRegistry::getInstance()->addPluginPath(path);
	
	if(!foundEnv)
		qWarning("PLUGINS DISABLED: You don't have the 'VISUAL_NETKIT_PLUGINS' container exported as environment variable!");
	
	PluginRegistry::getInstance()->fetchPlugins();
	qDebug() << "Founded" << PluginRegistry::getInstance()->getAllPluginFactories().size() << "plugin(s)";
	
	/* creating main gui */
	splash.showMessage("Loading main GUI");
	qDebug() << "Loading main GUI";
	MainWindow *win = new MainWindow();
	
	/* Creates the bug dumper object */
	splash.showMessage("Loading bug dumper");
	qDebug() << "Loading bug dumper";
	BugDumper *dumper = new BugDumper();
	Q_UNUSED(dumper);
	
	splash.finish(win);
	win->setWindowState(Qt::WindowMaximized);
	win->show();
	
	
	return app.exec();
}
