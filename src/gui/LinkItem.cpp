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

#include "LinkItem.h"
#include "handles/LinkMapper.h"
#include <QPointF>
#include <QPen>

/**
 * Constructor
 */
LinkItem::LinkItem(VirtualMachineItem* vmItem, CollisionDomainItem* cdItem, QString label)
	: QObject(), QGraphicsItemGroup()
{
	vm = vmItem;
	cd = cdItem;
	
	lineItem = new LineItemPrivate();
	
	myLabel = new LabelItemPrivate(label);
	
	pluginsSharedArea = new PluginsSharedArea();
	
	setZValue(800);
	setFlags(QGraphicsItem::ItemIsSelectable);
	
	//set the line position
	updateLinkPos();
	
	addToGroup(lineItem);
	addToGroup(myLabel);
	addToGroup(pluginsSharedArea);
	
	/* connects */
	connect(vm, SIGNAL(positionChanged()),
			this, SLOT(updateLinkPos()));
	connect(cd, SIGNAL(positionChanged()),
			this, SLOT(updateLinkPos()));
	connect(cd, SIGNAL(needBoundingrectRebuild()),
				this, SLOT(forceBoundingrectRebuild()));
	connect(vm, SIGNAL(needBoundingrectRebuild()),
					this, SLOT(forceBoundingrectRebuild()));
	
	/* init the context menu */
	initContextMenu();
	
	pluginsManager = new ManagePluginsDialog(this);
}

/**
 * Deconstructor
 */
LinkItem::~LinkItem()
{
}

/**
 * [PUBLIC-SLOT]
 * Update the link and label position/orientation
 */
void LinkItem::updateLinkPos()
{
	QPointF vmCenter, cdCenter, labelCenter;

	//start point
	vmCenter.setX(vm->getSvgPrivate()->scenePos().x() +
			vm->getSvgPrivate()->boundingRect().width() / 2);
	vmCenter.setY(vm->getSvgPrivate()->scenePos().y() + 
			vm->getSvgPrivate()->boundingRect().height() / 2);
	
	//end point
	cdCenter.setX(cd->getSvgPrivate()->scenePos().x() +
			cd->getSvgPrivate()->boundingRect().width() / 2);
	cdCenter.setY(cd->getSvgPrivate()->scenePos().y() +
			cd->getSvgPrivate()->boundingRect().height() / 2);
	
	lineItem->setLine(QLineF(vmCenter, cdCenter));
	
	/* adjust the label position */
	labelCenter.setX(((cdCenter.x() + vmCenter.x()) * 0.5) + 4);
	labelCenter.setY((cdCenter.y() + vmCenter.y()) * 0.5);
	myLabel->setPos(labelCenter);
	
	/* adjust the label shared area */
	labelCenter.setX(((cdCenter.x() + vmCenter.x()) * 0.5) + 4);
	labelCenter.setY(((cdCenter.y() + vmCenter.y()) * 0.5) + 14);
	pluginsSharedArea->setPos(labelCenter);
		
}

/**
 * [PUBLIC-SLOT]
 * Update the link state
 */
void LinkItem::updateLinkState()
{
	lineItem->setLineColorByState(LinkMapper::getInstance()->getHardwareIterface(this)->getState());		
}

/**
 * [PUBLIC-SLOT]
 * change the plugin line inside the shared area
 */
void LinkItem::setPluginLine(QString pluginName, QString content)
{
	pluginsSharedArea->changeMyLine(pluginName, content);
}

/**
 * Change the label name for this link and change also the tree item name
 */
void LinkItem::setLinkLabel(QString newName)
{
	myLabel->setText(newName);
	SceneTreeMapper::getInstance()->changeLinkLabel(this, newName);
}

/**
 * [SLOT]
 * recalculate the bounding rect for this link
 */
void LinkItem::forceBoundingrectRebuild()
{
	//Hack: force re-calculate the bounding rect for this link
	removeFromGroup(myLabel);
	addToGroup(myLabel);
}

/**
 * [PRIVATE]
 * Create and initiate the cantext menu
 */
void LinkItem::initContextMenu()
{
	deleteAction = new QAction(tr("Delete Hardware Interface"), this);
	deleteAction->setIcon(QIcon(QString::fromUtf8(":/menu/delete")));
	managePluginsAction = new QAction(tr("Manage plugins"), this);
	managePluginsAction->setIcon(QIcon(QString::fromUtf8(":/small/plugin")));
	
	contextMenu.addAction(deleteAction);
	contextMenu.addAction(managePluginsAction);
	
	/* Connects */
	connect(deleteAction, SIGNAL(triggered()),
			this, SLOT(deleteLinkActionCalled()));
	connect(managePluginsAction, SIGNAL(triggered()),
			this, SLOT(managePluginsActionCalled()));

}

/**
 * [PRIVATE-SLOT]
 * Delete this link (handle call)
 */
void LinkItem::deleteLinkActionCalled()
{
	QMessageBox::warning(NULL,
		tr("NOT IMPLEMENTED"),
		tr("This function is not implemented yet >_<"),
		QMessageBox::Ok);
}

/**
 * [PROTECTED]
 * Context menu
 */
void LinkItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	Q_UNUSED(event);
	
	contextMenu.exec(QCursor::pos());
}

/**
 * [SLOT]
 * Show the manage plugins dialog
 */
void LinkItem::managePluginsActionCalled()
{
	pluginsManager->show();
}

/**
 * [REIMPL]
 * Returns the shape of this item as a QPainterPath in local coordinates.
 */
/*QRectF LinkItem::boundingRect() const
{
	QPainterPath path;
	QRectF rect;
	
	// link line optimized bounding rect
	qreal extra = (lineItem->pen().width() + 20) / 2.0;
	//QRectF lineBR(lineItem->line().p1(), QSizeF(lineItem->line().p2().x() - lineItem->line().p1().x(), lineItem->line().p2().y() - lineItem->line().p1().y()));
	//path.addRect(lineBR.normalized().adjusted(-extra, -extra, extra, extra));	// adds the line path
	
	QPolygonF lineBR;
	lineBR << QPointF(lineItem->line().p1().x(), lineItem->line().p1().y()+extra) 
		   << QPointF(lineItem->line().p2().x(), lineItem->line().p2().y()+extra) 
		   << QPointF(lineItem->line().p2().x(), lineItem->line().p2().y()-extra) 
		   << QPointF(lineItem->line().p1().x(), lineItem->line().p1().y()-extra);
	
	path.addPolygon(lineBR);
	//path.addRect(myLabel->shape().boundingRect());	// adds the label path
	
	//QRectF base_rect(myLabel->shape().boundingRect());
	
	//adds a QRectF with height as n*base_rect.height
	 */
	/*path.addRect(myLabel->shape().boundingRect().x(),
				 myLabel->shape().boundingRect().y(), 
				 base_rect.width(), 
				 base_rect.height() * pluginsSharedArea->getPluginsArea().size());
	*/
/*
	rect = path.boundingRect();
	
	return rect;
}
*/