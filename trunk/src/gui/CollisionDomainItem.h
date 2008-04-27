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

#ifndef COLLISIONDOMAINITEM_H_
#define COLLISIONDOMAINITEM_H_

#include <QGraphicsItemGroup>
#include <QSvgRenderer>
#include <QString>
#include <QDebug>
#include <QVariant>
#include <QMenu>
#include <QObject>
#include <QGraphicsSceneContextMenuEvent>

#include "SvgItemPrivate.h"
#include "LabelItemPrivate.h"
#include "../common/CommonConfigs.h"

/**
 * This class is a graphic group element of type Collision Domain
 * and cantain a label and a svg item showed as Collision domain
 * (it's a QGraphicsItemGroup)
 */
class CollisionDomainItem : public QObject, public QGraphicsItemGroup
{
	Q_OBJECT

private:
	SvgItemPrivate *collisionDomainSvg;
	LabelItemPrivate *myLabel;
	QMenu contextMenu;
	QAction *ungroupAction;
	QAction *restoreGroupAction;
	QAction *deleteAction;
	bool isJoin;
	
	void initContextMenu();

public:
	CollisionDomainItem(QString label);
	virtual ~CollisionDomainItem();
	QString getLabel() { return myLabel->text(); };
	SvgItemPrivate *getSvgPrivate() { return collisionDomainSvg; };
	
	/* Reimpl */
	int type() const { return UserType + CdItem; };
	
protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value);
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
	
private slots:
	void ungroupActionCalled();
	void deleteVmActionCalled();
	void restoreGroupActionCalled();
};

#endif /*COLLISIONDOMAINITEM_H_*/
