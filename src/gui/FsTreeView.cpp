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
#include <QApplication>

/**
 * Constructor
 */
FsTreeView::FsTreeView(QWidget *parent) : QTreeView(parent)
{
	dirModel = NULL;

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

	connect(this, SIGNAL(expanded(const QModelIndex &)), this, SLOT(adjustExpand(const QModelIndex &)));

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
			tr("Insert the folder name"),
			QLineEdit::Normal,
			QString(), &okPressed);

	if(folderName.trimmed().isEmpty())
		error.append(tr("Please, enter a valid name"));

	if(okPressed && error.isEmpty())
		if(! dirModel->mkdir( dirModel->index(filePath, 0), folderName.trimmed() ).isValid() )
			error.append(tr("There was an error during the folder creation."));

	if(!error.isEmpty())
		QMessageBox::warning(this, "Error", error);
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
		menuActions.value(TextEditor)->setDisabled(dirModel->isDir(current));
}

/**
 * [PRIVATE_SLOT]
 * Refresh the view
 */
void FsTreeView::refreshView()
{
	if(!dirModel)
	{
		qWarning() << "FsTreeView QDirModel is NULL";
		return;
	}

	dirModel->refresh();
}

/**
 * [PRIVATE]
 * Get all paths from a root index
 */
void FsTreeView::removeDirRecursive(QModelIndex index, QStringList *paths)
{
    if(!index.isValid())
    {
    	qWarning() << "FsTreeView::removeDirRecursive()" << index << "is invalid";
    	return;
    }

    for(int i=0; i<dirModel->rowCount(index); i++)
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
	if(!dirModel)
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


	QApplication::setOverrideCursor(Qt::WaitCursor);

	QStringList *l = new QStringList();
	removeDirRecursive(current, l);

	foreach(QString path, *l)
	{
		const QModelIndex elem = dirModel->index(path, 0);
		if(!elem.isValid()) qDebug() << "Element" << elem << "is not a valid index";

		if(dirModel->isDir(elem))
			dirModel->rmdir(elem);
		else
			dirModel->remove(elem);
	}

	l->clear();
	delete l;

	QApplication::restoreOverrideCursor();

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

/**
 * [PROTECTED]
 * Open text editor on leaf double click
 */
void FsTreeView::mouseDoubleClickEvent(QMouseEvent *event)
{
	QModelIndex mIndex = indexAt(event->pos());		//get the selected item

	if(mIndex.isValid() && !dirModel->isDir(mIndex))
	{
		current = mIndex;
		editFile();
	}
	else if(mIndex.isValid() && dirModel->isDir(mIndex))
		expand(mIndex);
}

/**
 * Set the dir model
 */
void FsTreeView::setDirModel(QDirModel *model)
{
	//set and save the model
	dirModel = model;
	setModel(dirModel);
}

/**
 * [PRIVATE_SLOT]
 * adjust row size when expanded
 */
void FsTreeView::adjustExpand(const QModelIndex &index)
{
	Q_UNUSED(index);

	resizeColumnToContents(0);
}
