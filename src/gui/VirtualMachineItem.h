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

/**
 * This class is a graphic group element of type Virtual Machine
 * and cantain a label and a svg item showed as virtual machine
 * (it's a QGraphicsItemGroup)
 */
class VirtualMachineItem : public QObject, public QGraphicsItemGroup
{
	Q_OBJECT
	
private:
	QMap<VmType, QString> svgFiles;
	SvgItemPrivate *vmSvg;
	LabelItemPrivate *vmNameLabel;
	QMenu contextMenu;
	QAction *ungroupAction;
	QAction *restoreGroupAction;
	QAction *deleteAction;
	QAction *manageLinks;
	
	bool isJoin;
	
	VmType vmType;
	
	void initContextMenu();
	
public:
	VirtualMachineItem(QString label, VmType type = Host) ;
	virtual ~VirtualMachineItem();
	void changeSvgFile(VmType type);
	QString getLabel() { return vmNameLabel->text(); };
	SvgItemPrivate *getSvgPrivate() { return vmSvg; };

	LabelItemPrivate* getLabelItemPrivate() { return vmNameLabel; };
	
	/* Reimpl */
	int type() const { return UserType + VmItem; };
	
	VmType getVmType() { return vmType; };
	
protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value);
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
	
public slots:
	void ungroupActionCalled();
	void deleteVmActionCalled();
	void restoreGroupActionCalled();
	
signals:
	void positionChanged();

};

#endif /*VIRTUALMACHINEITEM_H_*/
