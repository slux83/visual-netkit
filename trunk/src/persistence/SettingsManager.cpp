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

/**
 * Get the lab open history
 * It's a QStringList that contains paths
 */
QStringList SettingsManager::getLabHistory()
{
	QStringList history;
	
	beginGroup("OpenedLabHistory");
	history = value("history").toStringList();
	endGroup();
	
	return history;
}

/**
 * Set a new path to the history list
 */
void SettingsManager::setLabHistory(const QString &newPath)
{
	QStringList oldHistory = getLabHistory();
	
	/* store only unknown paths */
	if(oldHistory.contains(newPath))
		return;
	
	QStringList newHistory;
	
	newHistory.prepend(newPath);	//the first of the list
	
	/* Now get the 0 to N-2 old paths */
	if(oldHistory.size() == LAB_OPENED_HISTORY_SIZE)
		for(int i=0; i<LAB_OPENED_HISTORY_SIZE-1; i++)
			newHistory.append(oldHistory.at(i));
	else
		newHistory << oldHistory;
	
	beginGroup("OpenedLabHistory");
	setValue("history", newHistory);
	endGroup();
	
	sync();
	
	emit historyChanged();
}

/**
 * Clear the lab opened history
 */
void SettingsManager::clearLabHistory()
{
	beginGroup("OpenedLabHistory");
	setValue("history", QStringList());
	endGroup();
	
	emit historyChanged();	//emit history changed signal
}
