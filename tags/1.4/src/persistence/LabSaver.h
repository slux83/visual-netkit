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

#ifndef LABSAVER_H_
#define LABSAVER_H_

#include <QString>
#include <QDir>
#include "../core/handles/LabFacadeController.h"

class LabSaver
{
private:
	Laboratory *currentLab;
	QString curFile;
	QString curPath;
	QString errorString;
	QStringList excludedPaths;
	bool needBackup;

	bool saveLabConf();
	bool createFolderSystem();
	bool saveStartups();
	bool saveTemplates();
	QString prepareLabConfText();
	QString strippedName(const QString &fullFileName);
	QStringList removeUnusedStuff();

public:
	LabSaver(const QString &path, bool backup = true);
	virtual ~LabSaver();

	bool saveLab();
	QString getErrorString() { return errorString; };
	static bool removeDir(const QString d);
};

#endif /*LABSAVER_H_*/
