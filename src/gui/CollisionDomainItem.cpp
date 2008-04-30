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

#include "CollisionDomainItem.h"
#include <QGraphicsScene>
#include <QCursor>
#include <QMessageBox>

/**
 * Contructor
 */
CollisionDomainItem::CollisionDomainItem(QString label) 
	: QObject(), QGraphicsItemGroup()
{
	//the group is joined
	isJoin = true;
	
	//no links connected
	connectedlinks = 0;
	
	/* init label and svg item */
	myLabel = new LabelItemPrivate(label);
	myLabel->setFont(GRAPHICS_FONT);
	myLabel->setPos(0, 35);
	
	//by default the svg collision domain is marked as unconnected icon
	collisionDomainSvg = new SvgItemPrivate(QString::fromUtf8(":/svg/cs_off"), this);
	
	addToGroup(collisionDomainSvg);
	addToGroup(myLabel);
	
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
	setZValue(1000);
	
	/* init the context menu */
	initContextMenu();
}

/**
 * Deconstructor
 */
CollisionDomainItem::~CollisionDomainItem()
{
}

/**
 * [PRIVATE]
 * Create and initiate the cantext menu
 */
void CollisionDomainItem::initContextMenu()
{
	ungroupAction = new QAction(tr("Ungroup items"), this);
	ungroupAction->setIcon(QIcon(QString::fromUtf8(":/small/delete_group")));
	deleteAction = new QAction(tr("Delete Virtual Machine"), this);
	deleteAction->setIcon(QIcon(QString::fromUtf8(":/small/delete")));
	restoreGroupAction = new QAction(tr("Restore group") , this);
	restoreGroupAction->setIcon(QIcon(QString::fromUtf8(":/small/create_group")));
	
	contextMenu.addAction(ungroupAction);
	contextMenu.addAction(restoreGroupAction);
	contextMenu.addAction(deleteAction);
	
	/* Connects */
	connect(ungroupAction, SIGNAL(triggered()),
			this, SLOT(ungroupActionCalled()));
	connect(deleteAction, SIGNAL(triggered()),
			this, SLOT(deleteVmActionCalled()));
	connect(restoreGroupAction, SIGNAL(triggered()),
				this, SLOT(restoreGroupActionCalled()));
}

/**
 * [PROTECTED]
 * Control the moviment of this item and don't permit that it's drowed outside
 * the scene rect
 */
QVariant CollisionDomainItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
	//qDebug() << boundingRect() << boundingRect().x() << boundingRect().y();
	if (change == ItemPositionChange && scene())
	{
		//emit a signal
		emit positionChanged();
		
		// value is the new position.
		QPointF newPos = value.toPointF();
		QRectF rect = scene()->sceneRect();

		// Keep the item inside the scene rect. (- 2 is a padding)
		newPos.setX(qMin(rect.right() - boundingRect().width() - 2,
				qMax(newPos.x(), rect.left())));
		newPos.setY(qMin(rect.bottom()- boundingRect().height() - 2,
				qMax(newPos.y(), rect.top())));

		return newPos;
	}
	
	return QGraphicsItem::itemChange(change, value);
}

/**
 * [PROTECTED]
 * Context menu
 */
void CollisionDomainItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	Q_UNUSED(event);
	
	/* enable restoreGroup if the label is separated */
	restoreGroupAction->setDisabled(isJoin);
	ungroupAction->setDisabled(!isJoin);
	
	contextMenu.exec(QCursor::pos());
}

/**
 * [PRIVATE-SLOT]
 * Ungroup action hanle
 */
void CollisionDomainItem::ungroupActionCalled()
{	
	//The svg item remain inside the group!
	removeFromGroup(myLabel);
	isJoin = false;
}


/**
 * [PRIVATE-SLOT]
 * Delete action hanle
 */
void CollisionDomainItem::deleteVmActionCalled()
{
	QMessageBox::warning(NULL,
			tr("NOT IMPLEMENTED"),
			tr("This function is not implemented yet >_<"),
			QMessageBox::Ok);
}

/**
 * [PRIVATE-SLOT]
 * Restore the group
 */
void CollisionDomainItem::restoreGroupActionCalled()
{	
	addToGroup(myLabel);
	isJoin = true;
}

/**
 * A new link is added.. update the link counter.
 */
void CollisionDomainItem::increaseDecreaseLinkCounter(bool increase)
{
	//update the counter
	(increase)?	connectedlinks++ : connectedlinks--;
		
	//update the svg image
	if(connectedlinks > 0)
		collisionDomainSvg->renderer()->load(QString::fromUtf8(":/svg/cs_on"));
	else
		collisionDomainSvg->renderer()->load(QString::fromUtf8(":/svg/cs_off"));
	
}
