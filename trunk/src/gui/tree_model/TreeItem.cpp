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

#include <QStringList>
#include <QDebug>

#include "TreeItem.h"
#include "../handles/AbstractPropertyHandler.h"

/**
 * Constructor
 */
TreeItem::TreeItem(const QVector<QVariant> &data, TreeItem *parent)
{
	parentItem = parent;
	itemData = data;
	myHandler = NULL;
	isProp = false;
	isPluginHeader = false;
	isElementProp = false;
}

/**
 * Deconstructor
 * remove all childs
 */
TreeItem::~TreeItem()
{
	qDeleteAll(childItems);
}

/**
 * Return a child giveing the number
 */
TreeItem *TreeItem::child(int number)
{
	return childItems.value(number);
}

/**
 * Get the child count
 */
int TreeItem::childCount() const
{
	return childItems.count();
}

/**
 * Get my child number (respect my parent node)
 */
int TreeItem::childNumber() const
{
	if (parentItem)
		return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));

	return 0;
}

/**
 * Get the column count
 */
int TreeItem::columnCount() const
{
	return itemData.count();
}

/**
 * Return node data
 */
QVariant TreeItem::data(int column) const
{
	return itemData.value(column);
}

/**
 * Insert a new children
 */
bool TreeItem::insertChildren(int position, int count, int columns)
{
	if (position < 0 || position > childItems.size())
		return false;

	for (int row = 0; row < count; ++row) {
		QVector<QVariant> data(columns);
		TreeItem *item = new TreeItem(data, this);
		childItems.insert(position, item);
	}

	return true;
}

/**
 * Insert 'columns' new columns
 */
bool TreeItem::insertColumns(int position, int columns)
{
	if (position < 0 || position > itemData.size())
		return false;

	for (int column = 0; column < columns; ++column)
		itemData.insert(position, QVariant());

	foreach (TreeItem *child, childItems)
		child->insertColumns(position, columns);

	return true;
}	

/**
 * Get my parent node
 */ 
TreeItem *TreeItem::parent()
{
	return parentItem;
}

/**
 * Remove a children
 */
bool TreeItem::removeChildren(int position, int count)
{
	if (position < 0 || position + count > childItems.size())
		return false;

	for (int row = 0; row < count; ++row)
		delete childItems.takeAt(position);

	return true;
}

/**
 * Remove a column
 */
bool TreeItem::removeColumns(int position, int columns)
{
	if (position < 0 || position + columns > itemData.size())
		return false;

	for (int column = 0; column < columns; ++column)
		itemData.remove(position);

	foreach(TreeItem *child, childItems)
		child->removeColumns(position, columns);

	return true;
}

/**
 * Set data
 */
bool TreeItem::setData(int column, const QVariant &value)
{
	if (column < 0 || column >= itemData.size())
	{
		qDebug() << "TreeItem::setData() column =" << column;
		return false;
	}

	QVariant oldValue = itemData[column];
	itemData[column] = value;
	
	//save the property changed
	if(myHandler && column == 1)
	{
		if(!myHandler->saveChangedProperty(this))
			itemData[column] = oldValue;	//restore value on error
	}
		
	return true;
}

/**
 * Append a child description
 */
void TreeItem::appendChildDescription(QString childName, QString childId)
{
	//Build struct ChildDescription
	ChildDescription cd;
	cd.name = childName;
	cd.id = childId;
	
	childPropertyDescription << cd;
}

/**
 * Convenience function
 */
void TreeItem::appendChildsDescription(QList< QPair<QString, QString> > childs)
{
	for(int i=0; i<childs.size(); i++)
	{
		QPair<QString, QString> des = childs.at(i);
		appendChildDescription(des.first, des.second);
	}
}
