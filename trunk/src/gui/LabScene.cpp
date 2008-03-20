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

#include "LabScene.h"

/**
 * Constructor: the scene size is Normal = 1000x1000
 */
LabScene::LabScene() : QGraphicsScene(0, 0, 1000, 1000)
{
}

/**
 * Deconstructor
 */
LabScene::~LabScene()
{
}

/**
 * Mouse press event
 */
void LabScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	/* The user want to add an element? */
	if(mouseEvent->button() == Qt::LeftButton)
	{
		
	}
	
	QGraphicsScene::mousePressEvent(mouseEvent);
}

/**
 * Mouse move event
 */
void LabScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	QGraphicsScene::mouseMoveEvent(mouseEvent);
}

/**
 * Mouse release event
 */
void LabScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
