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

#ifndef VIRTUALMACHINEITEM_H_
#define VIRTUALMACHINEITEM_H_

#include <QGraphicsItemGroup>
#include <QSvgRenderer>
#include <QString>
#include <QDebug>
#include <QVariant>
#include <QMenu>
#include <QObject>
#include <QGraphicsSceneContextMenuEvent>

#include "../common/CommonConfigs.h"
#include "SvgItemPrivate.h"
#include "LabelItemPrivate.h"
#include "PluginsSharedArea.h"
#include "ManagePluginsDialog.h"

/**
 * This class is a graphic group element of type Virtual Machine
 * and cantain a label and a svg item showed as virtual machine
 * (it's a QGraphicsItemGroup)
 */
class VirtualMachineItem : public QObject, public QGraphicsItemGroup
{
	Q_OBJECT
	
private:
	SvgItemPrivate *vmSvg;
	LabelItemPrivate *vmNameLabel;
	PluginsSharedArea *pluginsSharedArea;
	QMenu contextMenu;
	QAction *ungroupAction;
	QAction *restoreGroupAction;
	QAction *deleteAction;
	QAction *managePluginsAction;
	//ManagePluginsDialog *pluginsManager;
	
	bool isJoin;
		
	void initContextMenu();
	void refreshGroup();
	
public:
	VirtualMachineItem(QString label) ;
	virtual ~VirtualMachineItem();
	QString getLabel() { return vmNameLabel->text(); };
	SvgItemPrivate *getSvgPrivate() { return vmSvg; };
	void setLabelVmName(QString newLabel);
	void setLabelPos(QPointF &pos) { vmNameLabel->setPos(pos); refreshGroup(); };
	void setMultiLabelPos(QPointF &pos) { pluginsSharedArea->setPos(pos); refreshGroup(); };
	LabelItemPrivate* getLabelItemPrivate() { return vmNameLabel; };
	
	PluginsSharedArea* getPluginsSharedArea() { return pluginsSharedArea; };
	
	/* Reimpl */
	int type() const { return UserType + VmItem; };
	
protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value);
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
	
public slots:
	void ungroupActionCalled();
	void deleteVmActionCalled();
	void restoreGroupActionCalled();
	//void managePluginsActionCalled();
	void setPluginLine(QString pluginName, QString content);
	
signals:
	void positionChanged();
	void needBoundingrectRebuild();

};

#endif /*VIRTUALMACHINEITEM_H_*/
