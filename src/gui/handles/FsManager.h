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

#ifndef FSMANAGER_H_
#define FSMANAGER_H_

#include <QDirModel>

/**
 * [SINGLETON]
 * The file system manager of the laboratory
 */
class FsManager
{

private:
	static FsManager *instance;
	QDirModel *fsModel;

public:
	FsManager();
	virtual ~FsManager();
	static FsManager* getInstance();
	QDirModel * getFsModel() { return fsModel; }
	QModelIndex changePath(QString path);
	QString newFile(QString path, QString fileName);
	QString newFolder(QString path, QString folderName);
	QDirModel * newDirModel();
	QModelIndex rootLabPath();
	bool remove(QString path);	//TODO: remove this function
	QString getLabPath();
	bool openEditor(QString filePath);
};

#endif /*FSMANAGER_H_*/
