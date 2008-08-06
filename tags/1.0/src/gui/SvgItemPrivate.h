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

#ifndef SVGITEMPRIVATE_H_
#define SVGITEMPRIVATE_H_

#include <QGraphicsSvgItem>
#include <QDebug>
#include <QMenu>
#include <QObject>
#include <QGraphicsSceneContextMenuEvent>
#include "../common/Types.h"

/**
 * This is a private class used by group items to include an svg graphics item
 * that have more features compared to the base QGraphiscSvgItem Qt class
 */
class SvgItemPrivate : public QGraphicsSvgItem
{
	Q_OBJECT
	
private:
	QGraphicsItemGroup *myGroup;
	
public:
	SvgItemPrivate(const QString &fileName, QGraphicsItemGroup *group, 
			QGraphicsItem *parent = 0);
	virtual ~SvgItemPrivate();
	QGraphicsItemGroup *getGroup() { return myGroup; };
	
	/* Reimpl */
	int type() const { return UserType + SvgItem; };
};

#endif /*SVGITEMPRIVATE_H_*/
