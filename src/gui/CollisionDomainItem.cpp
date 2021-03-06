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
#include "handles/SceneTreeMapper.h"
#include "handles/CdHandler.h"

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
	myLabel->setPos(0, 35);
	
	/* init shared area */
	pluginsSharedArea = new PluginsSharedArea();
	pluginsSharedArea->setPos(0, 50);
	
	//by default the svg collision domain is marked as unconnected icon
	collisionDomainSvg = new SvgItemPrivate(QString::fromUtf8(":/svg/cs_off"), this);
	
	addToGroup(collisionDomainSvg);
	addToGroup(myLabel);
	addToGroup(pluginsSharedArea);
	
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
	setZValue(1000);
	
	/* init the context menu */
	initContextMenu();
	
	//pluginsManager = new ManagePluginsDialog(this);

}

/**
 * Deconstructor
 */
CollisionDomainItem::~CollisionDomainItem()
{
	delete collisionDomainSvg;
	delete myLabel;
	delete pluginsSharedArea;
	delete ungroupAction;
	delete restoreGroupAction;
	delete deleteAction;
	delete managePluginsAction;
	//delete pluginsManager;
}

/**
 * [PRIVATE]
 * Create and initiate the cantext menu
 */
void CollisionDomainItem::initContextMenu()
{
	ungroupAction = new QAction(tr("Ungroup items"), this);
	ungroupAction->setIcon(QIcon(QString::fromUtf8(":/menu/delete_group")));
	deleteAction = new QAction(tr("Delete Collision Domain"), this);
	deleteAction->setIcon(QIcon(QString::fromUtf8(":/menu/delete")));
	restoreGroupAction = new QAction(tr("Restore group") , this);
	restoreGroupAction->setIcon(QIcon(QString::fromUtf8(":/menu/create_group")));
	managePluginsAction = new QAction(tr("Manage plugins"), this);
	managePluginsAction->setIcon(QIcon(QString::fromUtf8(":/small/plugin")));
	
	contextMenu.addAction(ungroupAction);
	contextMenu.addAction(restoreGroupAction);
	contextMenu.addAction(deleteAction);
	contextMenu.addAction(managePluginsAction);
	
	/* Connects */
	connect(ungroupAction, SIGNAL(triggered()),
			this, SLOT(ungroupActionCalled()));
	connect(deleteAction, SIGNAL(triggered()),
			this, SLOT(deleteCdActionCalled()));
	connect(restoreGroupAction, SIGNAL(triggered()),
				this, SLOT(restoreGroupActionCalled()));
//	connect(managePluginsAction, SIGNAL(triggered()),
//				this, SLOT(managePluginsActionCalled()));
	
	//disabled because not finished
	managePluginsAction->setVisible(false);
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
	
	//Prevent coredump when item is removed: no need to adjust the link :D
	if(change != ItemChildRemovedChange && change != ItemVisibleHasChanged)
	{
		//emit a signal
		emit positionChanged();
	}
	
	if(change == ItemPositionHasChanged && scene())
	{
		LabHandler::getInstance()->setChangedLabState();
		emit needBoundingrectRebuild();		
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
	removeFromGroup(pluginsSharedArea);
	isJoin = false;
}


/**
 * [PRIVATE-SLOT]
 * Delete action hanle
 */
void CollisionDomainItem::deleteCdActionCalled()
{
	int resp = QMessageBox::question(NULL,
		tr("Visual Netkit - question"),
		tr("Do you want delete the selected collision domain with all connected links?") + "\nCollision domain: "
		+ myLabel->text(),
		QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
	
	if(resp == QMessageBox::No)
		return;
	
	/* before delete this group, ensure its joined */
	restoreGroupActionCalled();
	
	CdHandler::getInstance()->deleteCd(this);
	
}

/**
 * [PRIVATE-SLOT]
 * Restore the group
 */
void CollisionDomainItem::restoreGroupActionCalled()
{
	if(!isJoin)
	{
		addToGroup(myLabel);
		addToGroup(pluginsSharedArea);
	}
	isJoin = true;
}

/**
 * A link is added/removed.. update the link counter.
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

/**
 * Set the cd name inside the label
 */
void CollisionDomainItem::setLabelCdName(QString newLabel)
{
	myLabel->setText(newLabel);
	SceneTreeMapper::getInstance()->changeCdLabel(this, newLabel);
	refreshGroup();
}

/**
 * [SLOT]
 * change the plugin line inside the shared area
 */
void CollisionDomainItem::setPluginLine(QString pluginName, QString content)
{
	pluginsSharedArea->changeMyLine(pluginName, content);
	refreshGroup();
}
/**
 * [SLOT]
 * Show the manage plugins dialog
 */
//void CollisionDomainItem::managePluginsActionCalled()
//{
//	pluginsManager->buildGui();
//	pluginsManager->show();
//}

/**
 * [PRIVATE]
 * Refresh the group size
 */
void CollisionDomainItem::refreshGroup()
{
	 removeFromGroup(myLabel);
	 addToGroup(myLabel);
}

