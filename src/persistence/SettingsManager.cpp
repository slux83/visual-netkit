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

#include "SettingsManager.h"

/**
 * Constructor
 */
SettingsManager::SettingsManager(const QString &fileName, Format format, QObject *parent)
	: QSettings(fileName, format, parent)
{
}

/**
 * Deconstructor
 */
SettingsManager::~SettingsManager()
{
}

/**
 * Save the window state
 */
void SettingsManager::saveWindowSettings(const QByteArray &windowState,
		const QByteArray &windowGeometry)
{
	beginGroup("MainWindow");
	setValue("geometry", windowGeometry);
	setValue("state", windowState);
    endGroup();
}

/**
 * Restore the window geometry reading the settings
 */
QByteArray SettingsManager::restoreWindowGeometry()
{
	QByteArray geometry;	//the return value
	
	beginGroup("MainWindow");
	geometry = value("geometry").toByteArray();
	endGroup();
	
	return geometry;
}

/**
 * Restore the window state reading the settings
 */
QByteArray SettingsManager::restoreWindowState()
{
	QByteArray state;	//the return value
	
	beginGroup("MainWindow");
	state = value("state").toByteArray();
	endGroup();
	
	return state;
}
