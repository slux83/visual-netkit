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

#ifndef AREAITEM_H_
#define AREAITEM_H_

#include <QGraphicsRectItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QMap>
#include <QMenu>
#include <QObject>

static const int pointSize = 5;
static const quint8 areaOpacity = 100; //when the area is selected

/**
 * This class model a graphic area that can be used to group elements and/or
 * put some text stick notes on scene
 */
class AreaItem : public QObject, public QGraphicsRectItem
{
	Q_OBJECT
	
private:
	QString label;
	QMap<QString, QColor> backgroundColours;
	QColor activeColor;
	QMenu contextMenu, *colourMenu;
	QAction *deleteAction;

	void initContextMenu();

private slots:
	void deleteAreaActionCalled() {};
	void changeAreaColor(QAction *action);
	
public:
	AreaItem();
	virtual ~AreaItem();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0 ); 
	QRectF boundingRect() const;
	QPainterPath shape() const;
	
	int type() const { return QGraphicsItem::Type + 100; };

	void setLabel(QString text) { label = text; };
	QString getLabel() { return label; };

protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value);
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
};

#endif /*AREAITEM_H_*/
