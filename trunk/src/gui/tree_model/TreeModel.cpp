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

#include "TreeItem.h"
#include "TreeModel.h"

#include <QStringList>
#include <QDebug>

/**
 * Constructor
 */
TreeModel::TreeModel(const QStringList &headers, QObject *parent)
	: QAbstractItemModel(parent)
{
	QVector<QVariant> rootData;
	foreach(QString header, headers)
		rootData << header;

	rootItem = new TreeItem(rootData);
}

/**
 * Deconstructor
 * destroy the tree root
 */
TreeModel::~TreeModel()
{
	qDebug("Destroying model");
	delete rootItem;
}

/**
 * Get column count gived a parent
 */
int TreeModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent)
	return rootItem->columnCount();
}

/**
 * Get data of a node
 */
QVariant TreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole && role != Qt::EditRole)
		return QVariant();

	TreeItem *item = getItem(index);

	return item->data(index.column());
}

/**
 * Flags of a node
 */
Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::ItemIsEnabled;
	
	
	//is editable only 'value' column
	if(index.column() == 1)
	{
		//Plugin name (not editable)
		if(this->index(index.row(), 0, index.parent()).data(Qt::DisplayRole).toString() == "Plugin")
			return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
		
		return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	}
	else
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

/**
 * Get item 
 */
TreeItem *TreeModel::getItem(const QModelIndex &index) const
{
	if (index.isValid())
	{
		TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
		if (item) return item;
	}
	return rootItem;
}

/**
 * Get header data
 */
QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
							   int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return rootItem->data(section);

	return QVariant();
}

/**
 * Get index model by parent, row and column
 */
QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
	if (parent.isValid() && parent.column() != 0)
		return QModelIndex();

	TreeItem *parentItem = getItem(parent);

	TreeItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

/**
 * Insert columns
 */
bool TreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
	bool success;

	beginInsertColumns(parent, position, position + columns - 1);
	success = rootItem->insertColumns(position, columns);
	endInsertColumns();

	return success;
}

/**
 * Insert Rows
 */
bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
	TreeItem *parentItem = getItem(parent);
	bool success;

	beginInsertRows(parent, position, position + rows - 1);
	success = parentItem->insertChildren(position, rows, rootItem->columnCount());
	endInsertRows();

	return success;
}

/**
 * Get the parent model index of another index model 
 */
QModelIndex TreeModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	TreeItem *childItem = getItem(index);
	TreeItem *parentItem = childItem->parent();

	if (parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->childNumber(), 0, parentItem);
}

/**
 * Remove columns
 */
bool TreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
	bool success;

	beginRemoveColumns(parent, position, position + columns - 1);
	success = rootItem->removeColumns(position, columns);
	endRemoveColumns();

	if (rootItem->columnCount() == 0)
		removeRows(0, rowCount());

	return success;
}

/**
 * Remove rows
 */
bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
	TreeItem *parentItem = getItem(parent);
	bool success = true;

	beginRemoveRows(parent, position, position + rows - 1);
	success = parentItem->removeChildren(position, rows);
	endRemoveRows();

	return success;
}

/**
 * Row count
 */
int TreeModel::rowCount(const QModelIndex &parent) const
{
	TreeItem *parentItem = getItem(parent);

	return parentItem->childCount();
}

/**
 * Set data
 */
bool TreeModel::setData(const QModelIndex &index, const QVariant &value,
						int role)
{
	if (role != Qt::EditRole)
		return false;

	TreeItem *item = getItem(index);

	return item->setData(index.column(), value);
}

/**
 * Set header data
 */
bool TreeModel::setHeaderData(int section, Qt::Orientation orientation,
							  const QVariant &value, int role)
{
	if (role != Qt::EditRole || orientation != Qt::Horizontal)
		return false;

	return rootItem->setData(section, value);
}

