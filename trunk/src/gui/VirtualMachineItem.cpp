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

#include "VirtualMachineItem.h"
#include "handles/SceneTreeMapper.h"
#include <QGraphicsScene>
#include <QCursor>
#include <QMessageBox>

/**
 * Contructor
 * by default, this svg item is showed as VmHost
 */
VirtualMachineItem::VirtualMachineItem(QString label) 
	: QObject(), QGraphicsItemGroup()
{
	isJoin = true; //start witha joined group
	
	/* Set the default svg file: VmHost */
	vmSvg = new SvgItemPrivate(QString::fromUtf8(":/svg/vm_host"), this);
	vmNameLabel = new LabelItemPrivate(label);
	vmNameLabel->setPos(0, 52);
	/* init shared area */
	pluginsSharedArea = new PluginsSharedArea();
	pluginsSharedArea->setPos(0, 64);
	
	/* Add svg and label to this group */	
	addToGroup(vmSvg);
	addToGroup(vmNameLabel);
	addToGroup(pluginsSharedArea);
	
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
	setZValue(1000);
	
	/* Context menu */
	initContextMenu();
	
	pluginsManager = new ManagePluginsDialog(this);
}

/**
 * Deconstructor
 */
VirtualMachineItem::~VirtualMachineItem()
{
}

/**
 * [PROTECTED]
 * Controls the movements of this item and doesn't permit that it's drawed outside
 * the scene rect
 */
QVariant VirtualMachineItem::itemChange(GraphicsItemChange change, const QVariant &value)
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
	
	if(change == ItemPositionHasChanged && scene())
			emit needBoundingrectRebuild();
	
	//emit a signal
	emit positionChanged();
	
	return QGraphicsItem::itemChange(change, value);
}

/**
 * [PROTECTED]
 * Context menu
 */
void VirtualMachineItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	Q_UNUSED(event);
	
	/* enable restoreGroup if the label is separated */
	restoreGroupAction->setDisabled(isJoin);
	ungroupAction->setDisabled(!isJoin);
	
	contextMenu.exec(QCursor::pos());
}

/**
 * [PRIVATE]
 * Create and initiate the cantext menu
 */
void VirtualMachineItem::initContextMenu()
{
	ungroupAction = new QAction(tr("Ungroup items"), this);
	ungroupAction->setIcon(QIcon(QString::fromUtf8(":/menu/delete_group")));
	deleteAction = new QAction(tr("Delete Virtual Machine"), this);
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
			this, SLOT(deleteVmActionCalled()));
	connect(restoreGroupAction, SIGNAL(triggered()),
				this, SLOT(restoreGroupActionCalled()));
	connect(managePluginsAction, SIGNAL(triggered()),
				this, SLOT(managePluginsActionCalled()));
}

/**
 * [PRIVATE-SLOT]
 * Ungroup action hanle
 */
void VirtualMachineItem::ungroupActionCalled()
{	
	//The svg item remain inside the group!
	removeFromGroup(vmNameLabel);
	removeFromGroup(pluginsSharedArea);
	isJoin = false;
}


/**
 * [PRIVATE-SLOT]
 * Delete action hanle
 */
void VirtualMachineItem::deleteVmActionCalled()
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
void VirtualMachineItem::restoreGroupActionCalled()
{	
	if(!isJoin)
	{
		addToGroup(vmNameLabel);
		addToGroup(pluginsSharedArea);
	}
	isJoin = true;
}

/**
 * [SLOT]
 * change the plugin line inside the shared area
 */
void VirtualMachineItem::setPluginLine(QString pluginName, QString content)
{
	pluginsSharedArea->changeMyLine(pluginName, content);
}

/**
 * Change the vm label
 */
void VirtualMachineItem::setLabelVmName(QString newLabel)
{
	vmNameLabel->setText(newLabel);
	SceneTreeMapper::getInstance()->changeVmLabel(this, newLabel);
}

/**
 * [SLOT]
 * Show the manage plugins dialog
 */
void VirtualMachineItem::managePluginsActionCalled()
{
	pluginsManager->buildGui();
	pluginsManager->show();
}
