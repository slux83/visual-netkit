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
#include <QMessageBox>

/**
 * Constructor
 */
AreaItem::AreaItem() : QGraphicsRectItem()
{
	/* init the controller */
	aController =  AreaController::getInstance();
	
	/* set some stuff */
	setZValue(10);
	setRect(0, 0, 100, 100);	//the default size
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
	
	//build context menu
	initContextMenu();
}

/**
 * Deconstructor
 */
AreaItem::~AreaItem()
{
	delete deleteAction;
	delete colourMenu;
}

/**
 * [REIMPL]
 * Point event
 */
void AreaItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)
	
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
	bRect.setHeight(bRect.height() + pointSize + 1);
	bRect.setWidth(bRect.width()   + pointSize + 1);
	
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
	deleteAction = new QAction(tr("Delete Area"), this);
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

/**
 * [PRIVATE-SLOT]
 * Change background color
 */
void AreaItem::changeAreaColor(QAction *action)
{
	if(!backgroundColours.contains(action->text()))
		return;
	
	activeColor = backgroundColours.value(action->text());
	update(boundingRect());
	
	AreaController::getInstance()->setChangedLabState();

}

/**
 * Set the current color
 */
void AreaItem::setCurrentCorol(QColor &color)
{
	activeColor = color;
	update(boundingRect());
}

/**
 * [PROTECTED]
 * Controls the movements of this item and doesn't permit that it's drawed outside
 * the scene rect
 */
QVariant AreaItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
	// value is the new position.
	QPointF newPos = value.toPointF();
	if (change == ItemPositionChange && scene())
	{		
		
		QRectF rect = scene()->sceneRect();

		// Keep the item inside the scene rect. (- 2 is a padding)
		newPos.setX(qMin(rect.right() - boundingRect().width() - 2,
				qMax(newPos.x(), rect.left())));
		newPos.setY(qMin(rect.bottom()- boundingRect().height() - 2,
				qMax(newPos.y(), rect.top())));
		
		return newPos;
	}
	
	// item has changed position
	if(change == ItemPositionHasChanged && scene())
	{
		AreaController::getInstance()->setChangedLabState();
	}
	
	return QGraphicsItem::itemChange(change, value);
}

/**
 * [PRIVATE-SLOT]
 * Delete area handler
 */
void AreaItem::deleteAreaActionCalled()
{
	//ask before delete
	int resp = QMessageBox::question(NULL, tr("VisualNetkit - question"), tr("Do you want delete the selected Area?"),
			QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
	
	if(resp == QMessageBox::Yes)
		aController->deleteArea(this);
}

/**
 * [PRIVATE]
 * Change area text
 */
void AreaItem::setLabel(QString text)
{
	label = text;
	aController->setChangedLabState();
	update(boundingRect());
}
