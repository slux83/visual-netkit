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

#ifndef SETTINGSFACADECONTROLLER_H_
#define SETTINGSFACADECONTROLLER_H_

#include "../../persistence/SettingsManager.h"

#include <QObject>

/**
 * The facade controller to access to settings actions
 */
class SettingsFacadeController : public QObject
{
	Q_OBJECT
	
signals:
	void historyChanged();

private:
	static SettingsFacadeController *instance;
	SettingsManager *settings;
	
public:
	SettingsFacadeController();
	virtual ~SettingsFacadeController();
	static SettingsFacadeController* getInstance();
	
	void saveWindowSettings(const QByteArray &windowState, const QByteArray &windowGeometry);
	QByteArray restoreWindowGeometry();
	QByteArray restoreWindowState();
	QStringList getLabHistory() { return settings->getLabHistory(); };
	void setLabHistory(const QString &newPath) { settings->setLabHistory(newPath); };
};

#endif /*SETTINGSFACADECONTROLLER_H_*/
