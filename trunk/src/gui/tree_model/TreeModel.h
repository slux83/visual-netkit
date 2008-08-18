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

// THIS CLASS IS DERIVED BY EDITABLETREEMODEL EXAMPLE OF Qt4.4 doc

#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QObject>

class TreeItem;

/**
 * Tree Model that extend QAbstractItemModel
 */
class TreeModel : public QAbstractItemModel
{
	Q_OBJECT

private:
	TreeItem *rootItem;
	
public:
	TreeModel(const QStringList &headers, QObject *parent = 0);
	~TreeModel();

	QVariant data(const QModelIndex &index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation,
						int role = Qt::DisplayRole) const;

	TreeItem *getItem(const QModelIndex &index) const;


	QModelIndex index(int row, int column,
  					const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &index) const;

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	bool setData(const QModelIndex &index, const QVariant &value,
 				int role = Qt::EditRole);
	bool setHeaderData(int section, Qt::Orientation orientation,
  					 const QVariant &value, int role = Qt::EditRole);

	bool insertColumns(int position, int columns,
					   const QModelIndex &parent = QModelIndex());
	bool removeColumns(int position, int columns,
					   const QModelIndex &parent = QModelIndex());
	bool insertRows(int position, int rows,
					const QModelIndex &parent = QModelIndex());
	bool removeRows(int position, int rows,
					const QModelIndex &parent = QModelIndex());
	
	TreeItem* getRootItem() { return rootItem; };
};

#endif
