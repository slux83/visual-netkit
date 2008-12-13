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

#include "FsTreeView.h"
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>

/**
 * Constructor
 */
FsTreeView::FsTreeView(QWidget *parent) : QTreeView(parent)
{
	//Get my controller singleton instance
	fsManager = FsManager::getInstance();
	contextMenu = new QMenu(this);

	//Create menu actions with connections
	menuActions.insert(NewFile, contextMenu->addAction(QIcon(":/menu/new_icon"), tr("New File"), this, SLOT(newFile())));
	menuActions.insert(NewFolder, contextMenu->addAction(QIcon(":/menu/new_folder_icon"), tr("New Folder"), this, SLOT(newFolder())));
	contextMenu->addSeparator();
	menuActions.insert(TextEditor, contextMenu->addAction(QIcon(":/menu/text_editor"), tr("Edit File"), this, SLOT(editFile())));
	contextMenu->addSeparator();
	menuActions.insert(Delete, contextMenu->addAction(QIcon(":/menu/trash"), tr("Delete"), this, SLOT(deleteFile())));
	menuActions.insert(Rename, contextMenu->addAction(QIcon(), tr("Rename..."), this, SLOT(renameFile())));
	contextMenu->addSeparator();
	menuActions.insert(Refresh, contextMenu->addAction(QIcon(":/menu/refresh"), tr("Refresh"), this, SLOT(refreshView())));
}

/**
 * Destructor
 */
FsTreeView::~FsTreeView() {}

/**
 * [PROTECTED]
 * Context menu action
 */
void FsTreeView::contextMenuEvent(QContextMenuEvent *event)
{
	QModelIndex mIndex = indexAt(event->pos());		//get the selected item

	if(mIndex.isValid())
	{
		current = mIndex;
		filterMenu();
		contextMenu->exec(event->globalPos());
	}
}

/**
 * [PRIVATE_SLOT]
 * Create a new empty file
 */
void FsTreeView::newFile()
{
	//TODO: to fix some core dumps :p

	bool okPressed;
	QString error;
	QString filePath;

	QFileInfo fInfo(current.data(QDirModel::FilePathRole).toString());

	//Current index is a root file?
	if(fsManager->getLabPath() == current.parent().data(QDirModel::FilePathRole).toString() && fInfo.isFile())
		filePath = current.parent().data(QDirModel::FilePathRole).toString();
	else
		filePath = current.data(QDirModel::FilePathRole).toString();

	qDebug() << filePath;// << fsManager->getLabPath();// << current.parent().data(QDirModel::FilePathRole).toString();

	//Get the file name
	QString fileName = QInputDialog::getText(
			this,
			tr("Path: ") + filePath + "\n" +
			tr("Insert the file name"),
			tr("File Name"), QLineEdit::Normal,
			QString(), &okPressed);

	if(okPressed && !fileName.trimmed().isEmpty())
		error = fsManager->newFile(filePath, fileName.trimmed());

	if(!error.isEmpty())
		QMessageBox::warning(this, tr("Error"), tr("Unable to create an empty file") + ": " + error);
	else
		refreshView(true);
}

/**
 * [PRIVATE]
 * Enable/Disable actions for context menu in base of the file selected
 */
void FsTreeView::filterMenu()
{
	QFileInfo fInfo(current.data(QDirModel::FilePathRole).toString());

	menuActions.value(TextEditor)->setEnabled(fInfo.isFile());
}

/**
 * [PRIVATE_SLOT]
 * Refresh the view
 */
void FsTreeView::refreshView(bool expandCurrent)
{
	fsManager->getFsModel()->refresh(current.parent());

	if(expandCurrent)
		expand(current);
}
