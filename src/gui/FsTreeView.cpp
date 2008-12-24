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
	m = NULL;

	//Get my controller singleton instance
	fsManager = FsManager::getInstance();
	contextMenu = new QMenu(this);

	//Create menu actions with connections
	menuActions.insert(NewFile, contextMenu->addAction(QIcon(":/menu/new_icon"), tr("New File"), this, SLOT(newFile())));
	menuActions.insert(NewFolder, contextMenu->addAction(QIcon(":/menu/new_folder_icon"), tr("New Folder"), this, SLOT(newFolder())));
	contextMenu->addSeparator();
	menuActions.insert(TextEditor, contextMenu->addAction(QIcon(":/menu/text_editor"), tr("Edit File"), this, SLOT(editFile())));
	contextMenu->addSeparator();
	menuActions.insert(Trash, contextMenu->addAction(QIcon(":/menu/trash"), tr("Delete"), this, SLOT(deleteFile()), QKeySequence(QKeySequence::DeleteEndOfLine)));
	menuActions.insert(Rename, contextMenu->addAction(QIcon(), tr("Rename..."), this, SLOT(renameFile())));
	contextMenu->addSeparator();
	menuActions.insert(Refresh, contextMenu->addAction(QIcon(":/menu/refresh"), tr("Refresh"), this, SLOT(refreshView())));

	connect(this, SIGNAL(refreshNeeded(bool)), this, SLOT(refreshView(bool)));
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
 * Create a new folder
 */
void FsTreeView::newFolder()
{
	bool okPressed;
	QString error;
	QString filePath;

	if(!current.isValid())
		qWarning() << "FsTreeView::newFile() Invalid node";

	QFileInfo fInfo(current.data(QDirModel::FilePathRole).toString());

	if(!fInfo.exists())
		qWarning() << "File/Dir:" << current.data(QDirModel::FilePathRole).toString() << "doesn't exists";


	//Current index is a file or a root file?
	if( (fsManager->getLabPath() == current.parent().data(QDirModel::FilePathRole).toString() && !fInfo.isDir()) ||
		(fsManager->getLabPath() != current.parent().data(QDirModel::FilePathRole).toString() && fInfo.isFile()) )
	{
		filePath = current.parent().data(QDirModel::FilePathRole).toString();
	}
	else
	{
		filePath = current.data(QDirModel::FilePathRole).toString();
	}

	//Get the file name
	QString folderName = QInputDialog::getText(
			this,
			tr("Make New Folder/Path"),
			tr("Parent: ") + filePath + "\n\n" +
			tr("Insert the folder name (or a path like 'foo/bar')"),
			QLineEdit::Normal,
			QString(), &okPressed);

	if(okPressed && !folderName.trimmed().isEmpty())
		error = fsManager->newFolder(filePath, folderName.trimmed());

	if(!error.isEmpty())
		QMessageBox::warning(this, tr("Error"), tr("Unable to create the folder") + ": " + error);
	else
		refreshView();
}

/**
 * [PRIVATE_SLOT]
 * Create a new empty file
 */
void FsTreeView::newFile()
{
	bool okPressed;
	QString error;
	QString filePath;

	if(!current.isValid())
		qWarning() << "FsTreeView::newFile() Invalid node";

	QFileInfo fInfo(current.data(QDirModel::FilePathRole).toString());

	if(!fInfo.exists())
		qWarning() << "File/Dir:" << current.data(QDirModel::FilePathRole).toString() << "doesn't exists";


	//Current index is a file or a root file?
	if( (fsManager->getLabPath() == current.parent().data(QDirModel::FilePathRole).toString() && !fInfo.isDir()) ||
		(fsManager->getLabPath() != current.parent().data(QDirModel::FilePathRole).toString() && fInfo.isFile()) )
	{
		filePath = current.parent().data(QDirModel::FilePathRole).toString();
	}
	else
	{
		filePath = current.data(QDirModel::FilePathRole).toString();
	}

	//Get the file name
	QString fileName = QInputDialog::getText(
			this,
			tr("Make New File"),
			tr("Parent: ") + filePath + "\n\n" +
			tr("Insert the file name"),
			QLineEdit::Normal,
			QString(), &okPressed);

	if(okPressed && !fileName.trimmed().isEmpty())
		error = fsManager->newFile(filePath, fileName.trimmed());

	if(!error.isEmpty())
		QMessageBox::warning(this, tr("Error"), tr("Unable to create an empty file") + ": " + error);
	else
		refreshView();
}

/**
 * [PRIVATE]
 * Enable/Disable actions for context menu in base of the file selected
 */
void FsTreeView::filterMenu()
{
	if(current.isValid())
	{
		QFileInfo fInfo(current.data(QDirModel::FilePathRole).toString());
		menuActions.value(TextEditor)->setEnabled(fInfo.isFile());
	}
}

/**
 * [PRIVATE_SLOT]
 * Refresh the view
 */
void FsTreeView::refreshView(bool expandCurrent)
{
	Q_UNUSED(expandCurrent);

	if(!m)
	{
		qWarning() << "FsTreeView QDirModel is NULL";
		return;
	}

	m->refresh();
}

/**
 * [PRIVATE]
 * Get all paths from a root index
 */
void FsTreeView::removeDirRecursive(QModelIndex index, QStringList *paths)
{
    if(!index.isValid())
        return;

    for(int i=0; i<m->rowCount(index); i++)
    {
        removeDirRecursive(index.child(i, 0), paths);
    }

    paths->append(index.data(QDirModel::FilePathRole).toString());
}

/**
 * [PRIVATE_SLOT]
 * Delete a file/folder recursively
 */
void FsTreeView::deleteFile()
{
	if(!m)
	{
		qWarning() << "FsTreeView QDirModel is NULL";
		return;
	}

	if(!current.isValid())
	{
		qWarning() << "FsTreeView current index invalid";
		return;
	}

	int resp = QMessageBox::question(this, tr("Confirm Delete?"),
							tr("Do you want delete the selected item?") + "\n\n" +
							tr("Item: ") + current.data(QDirModel::FilePathRole).toString(),
							QMessageBox::Yes | QMessageBox::No);

	if(resp == QMessageBox::No)
		return;


	QStringList *l = new QStringList();
	removeDirRecursive(current, l);
	foreach(QString path, *l)
	{
		if(m->isDir(m->index(path,0)))
			qDebug() << path << m->index(path,0).isValid() << m->rmdir(m->index(path,0));
		else
			qDebug() << path << m->index(path,0).isValid() << m->remove(m->index(path,0));
	}

	l->clear();
	delete l;

}

/**
 * [PRIVATE_SLOT]
 * Rename an item
 */
void FsTreeView::renameFile()
{
	if(current.isValid())
		edit(current);
}

/**
 * [PRIVATE_SLOT]
 * Open an editor of a file
 */
void FsTreeView::editFile()
{
	if(!current.isValid())
		return;

	if(! fsManager->openEditor(current.data(QDirModel::FilePathRole).toString()))
		QMessageBox::warning(this,
			"Visual Netkit - Error",
			tr("Cannot open the file").append(" ").append(current.data(QDirModel::FilePathRole).toString()),
			QMessageBox::Ok);
}
