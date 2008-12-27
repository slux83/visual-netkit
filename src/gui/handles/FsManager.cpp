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

#include "FsManager.h"
#include "../../persistence/FilesystemExpert.h"
#include "../../core/handles/LabFacadeController.h"
#include "../../gui/handles/TabController.h"

FsManager* FsManager::instance = NULL;

/**
 * Constructor
 */
FsManager::FsManager()
{
	fsModel = new QDirModel();
	fsModel->setFilter(QDir::Hidden | QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
	fsModel->setReadOnly(false);
}

/**
 * Destructor
 */
FsManager::~FsManager()
{
	delete fsModel;
}

/**
 * [STATIC]
 * Get singleton instance
 */
FsManager* FsManager::getInstance()
{
	if(instance == NULL)
		instance = new FsManager();

	return instance;
}

/**
 * Change the root path of the model
 */
const QModelIndex FsManager::changePath(QString path)
{
	return fsModel->index(path, 0);
}

/**
 * Create a new file empty
 */
QString FsManager::newFile(QString path, QString fileName)
{
	return FilesystemExpert::newFile(path, fileName);
}

/**
 * Create a new file empty
 */
QString FsManager::newFolder(QString path, QString folderName)
{
	return FilesystemExpert::newFolder(path, folderName);
}

/**
 * Get the current lab path
 */
QString FsManager::getLabPath()
{
	Laboratory *l = LabFacadeController::getInstance()->getCurrentLab();
	if(l == NULL)
		qWarning() << "FsManager::getLabPath()" << "NULL Laboratory";

	return l->getLabPath().absolutePath();

}

/**
 * Open new text editor
 */
bool FsManager::openEditor(QString filePath)
{
	return TabController::getInstance()->openTab(filePath);
}

