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

#include "AreaItem.h"
#include <QDebug>

/**
 * Constructor
 */
AreaItem::AreaItem() : QGraphicsRectItem()
{
	setZValue(10);
	setRect(0, 0, 200, 200);	//the default size
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
	
	//default text
	label = "Add some text here.";
	
	//Color stuff
	activeColor = QColor(255, 253, 216);
	backgroundColours.insert(tr("Light yellow"), QColor(255, 253, 216));
	backgroundColours.insert(tr("Light red"), QColor(255, 135, 135));
	backgroundColours.insert(tr("White"), QColor(255, 255, 255));
	backgroundColours.insert(tr("Grey"), QColor(197, 197, 197));
	backgroundColours.insert(tr("Green"), QColor(122, 197, 120));
	backgroundColours.insert(tr("Cyan"), Qt::cyan);
	backgroundColours.insert(tr("Transparent"), Qt::transparent);
	
	//build context menu
	initContextMenu();
}

/**
 * Deconstructor
 */
AreaItem::~AreaItem()
{
}

/**
 * [REIMPL]
 * Point event
 */
void AreaItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	/* Draw the area */
	painter->save();
	painter->setPen(QPen(Qt::black, 1));
	(isSelected())? activeColor.setAlpha(areaOpacity) : activeColor.setAlpha(255);
	painter->setBrush(activeColor);
	painter->drawRect(rect());
	painter->restore();
	
	/* draw some text */
	painter->save();
	QString text(label);
	painter->drawText(5, 5, rect().width() - 5, rect().height() - 5,
			Qt::TextWordWrap, text);
	painter->restore();
	
	/* If the object is selected, draw the control ball (to resize) */
	if(isSelected())
	{
		painter->save();
		painter->setPen(QPen(Qt::blue, 1));
		painter->setBrush(QColor(56, 202, 255, 180));
		
		painter->drawEllipse(QRectF(rect().width() - pointSize,
				rect().height() - pointSize,
				pointSize * 2, pointSize * 2));
		painter->restore();
	}
}

/**
 * [REIMPL]
 * Bounding rect encreased +5px
 */
QRectF AreaItem::boundingRect() const
{
	QRectF bRect = QGraphicsRectItem::boundingRect();
	bRect.setHeight(bRect.height() + 5);
	bRect.setWidth(bRect.width() + 5);
	
	return bRect;
}

/**
 * [REIMPL]
 * Shape (for selections)
 */
QPainterPath AreaItem::shape() const
{
	QPainterPath path;
	path.addRect(boundingRect());
	return path;
}

/**
 * [PROTECTED]
 * Context menu
 */
void AreaItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	Q_UNUSED(event);	
	contextMenu.exec(QCursor::pos());
}

/**
 * [PRIVATE]
 * Create and initiate the cantext menu
 */
void AreaItem::initContextMenu()
{
	deleteAction = new QAction(tr("Delete Virtual Machine"), this);
	deleteAction->setIcon(QIcon(QString::fromUtf8(":/menu/delete")));
	
	contextMenu.addAction(deleteAction);
	colourMenu = contextMenu.addMenu(tr("Background color"));
	colourMenu->setIcon(QIcon(QString::fromUtf8(":/menu/color_edit")));
	
	/* Connect */
	connect(deleteAction, SIGNAL(triggered()),
			this, SLOT(deleteAreaActionCalled()));
	
	/* Fill the colours sub-menu */
	QMapIterator<QString, QColor> colourIt(backgroundColours);
	while(colourIt.hasNext())
	{
		colourIt.next();
		colourMenu->addAction(colourIt.key());
	}
	
	/* Connect all the submenu */
	connect(colourMenu, SIGNAL(triggered(QAction *)),
			this, SLOT(changeAreaColor(QAction *)));
}
