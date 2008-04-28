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

#include "SvgItemPrivate.h"


/**
 * Constructor
 */
SvgItemPrivate::SvgItemPrivate(const QString & fileName, QGraphicsItemGroup *group, 
		QGraphicsItem *parent) : QGraphicsSvgItem(fileName, parent)
{
	myGroup = group;
	
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);	
}

/**
 * Deconstructor
 */ 
SvgItemPrivate::~SvgItemPrivate()
{
}

/**
 * [PROTECTED]
 * Control the moviment of this item and don't permit that it's drowed outside
 * the scene rect
 */
/*QVariant SvgItemPrivate::itemChange(GraphicsItemChange change, const QVariant &value)
{
	qDebug() << "boo";
	if (change == ItemPositionChange && scene())
	{
		//emit a signal
		emit svgPositionChanged();
		qDebug() << "emitted!";
	}
	
	return QGraphicsItem::itemChange(change, value);
}

*/
