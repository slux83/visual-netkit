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

#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>
#include <QVector>
#include <QPair>

class AbstractPropertyHandler;

/*
 * Child description struct to know what the user can add a new property
 * as child of this node
 */
struct ChildDescription
{
	QString name;
	QString id;
};

/**
 * Tree item class that model a note (the visual side of a property)
 */
class TreeItem
{

private:
	QList<TreeItem*> childItems;
	QVector<QVariant> itemData;
	QList<ChildDescription> childPropertyDescription;
	TreeItem *parentItem;
	AbstractPropertyHandler *myHandler;
	bool isProp;
	bool isPluginHeader;
	bool isElementProp;
	QString id;
	QString description;
	
public:
	TreeItem(const QVector<QVariant> &data, TreeItem *parent = 0);
	~TreeItem();

	TreeItem *child(int number);
	int childCount() const;
	int columnCount() const;
	QVariant data(int column) const;
	bool insertChildren(int position, int count, int columns);
	bool insertColumns(int position, int columns);
	TreeItem *parent();
	bool removeChildren(int position, int count);
	bool removeColumns(int position, int columns);
	int childNumber() const;
	bool setData(int column, const QVariant &value);
	void appendChildDescription(QString childName, QString childId);
	void appendChildsDescription(QList< QPair<QString, QString> > childs);
	bool isProperty() { return isProp; };
	void setProperty(bool prop) { isProp = prop;};
	void setIsPluginHeader(bool val) { isPluginHeader = val; };
	bool isPluginHeaderNode() { return isPluginHeader; };
	QList<ChildDescription> getChildDescription() { return childPropertyDescription; };
	
	void setIsElementProperty(bool val) { isElementProp = val; };
	bool isElementProperty() { return isElementProp; };
	
	void setId(QString newId) { id = newId; };
	QString getId() { return id; };
	
	void setDescription(QString newDescr) { description = newDescr; };
	QString getDescrioption() { return description; };
	
	void setPropertyHandler(AbstractPropertyHandler * newHandler) { myHandler = newHandler; };
	
};

#endif
