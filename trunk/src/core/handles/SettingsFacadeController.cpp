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

#include "SettingsFacadeController.h"
#include "LabFacadeController.h"

/**
 * Init the null instance pointer
 */
SettingsFacadeController* SettingsFacadeController::instance = NULL;

/**
 * Constructor
 */
SettingsFacadeController::SettingsFacadeController() : QObject()
{
	QString iniPath =
		LabFacadeController::getInstance()->getAppHome().absolutePath() + "/visualnetkit.ini";
	
	// init the settings
	settings = new SettingsManager(iniPath);
	
	// foreward the signal
	connect(settings, SIGNAL(historyChanged()), this, SIGNAL(historyChanged()));
}

/**
 * Deconstructor
 */
SettingsFacadeController::~SettingsFacadeController()
{
	delete settings;
}

/**
 * [STATIC]
 * Get the instance of this (facade) controller
 */
SettingsFacadeController * SettingsFacadeController::getInstance()
{
	if(instance == NULL)
		instance = new SettingsFacadeController();
	
	return instance;
}

/**
 * Save the window settings
 */
void SettingsFacadeController::saveWindowSettings(const QByteArray &windowState,
		const QByteArray &windowGeometry)
{
	settings->saveWindowSettings(windowState, windowGeometry);
}

/**
 * Get the saved window geometry
 */
QByteArray SettingsFacadeController::restoreWindowGeometry()
{
	return settings->restoreWindowGeometry();
}

/**
 * Get the saved window state
 */
QByteArray SettingsFacadeController::restoreWindowState()
{
	return settings->restoreWindowState();
}
