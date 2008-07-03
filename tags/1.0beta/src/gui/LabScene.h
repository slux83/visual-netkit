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

#ifndef LABSCENE_H_
#define LABSCENE_H_

#include <QGraphicsScene>
#include <QPainterPath>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

class LabScene : public QGraphicsScene
{
	Q_OBJECT
	
private:
	QGraphicsRectItem *border;	//the path for the scene border
	QGraphicsRectItem *selectionRect;	//the selection rect
	QGraphicsLineItem *link;	//a fake link
	QGraphicsLineItem *initNewLinkLine();
	void initBorder();
	
private slots:
	void adjustSceneBorder(QRectF r);
	
public:
	LabScene();
	virtual ~LabScene();
	void clearScene();
	
protected:
	void mousePressEvent  		(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseMoveEvent   		(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseReleaseEvent		(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseDoubleClickEvent	(QGraphicsSceneMouseEvent *mouseEvent);

};

#endif /*LABSCENE_H_*/
