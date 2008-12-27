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

#ifndef FSTREEVIEW_H_
#define FSTREEVIEW_H_

#include "handles/FsManager.h"

#include <QTreeView>
#include <QMenu>
#include <QContextMenuEvent>

/**
 * The file system  tree view of the current laboratory
 * (File management)
 */
class FsTreeView : public QTreeView
{
	Q_OBJECT

enum ActionEnum
{
	NewFile		= 0,
	NewFolder	= 1,
	TextEditor	= 2,
	Trash		= 3,
	Rename		= 4,
	Refresh		= 5
};

private:
	FsManager *fsManager;
	QMenu *contextMenu;
	QModelIndex current;
	QMap<ActionEnum, QAction*> menuActions;
	QDirModel *dirModel;

	void removeDirRecursive(QModelIndex index, QStringList *paths);
	void filterMenu();

private slots:
	void deleteFile();
	void newFile();
	void newFolder();
	void renameFile();
	void refreshView();
	void editFile();

protected:
	void contextMenuEvent(QContextMenuEvent *event);

public:
	FsTreeView(QWidget *parent = NULL);
	virtual ~FsTreeView();
	void setDirModel(QDirModel *model) { dirModel = model; setModel(dirModel); }
};

#endif /* FSTREEVIEW_H_ */
