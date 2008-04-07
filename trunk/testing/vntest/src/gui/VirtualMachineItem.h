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

#include <QGraphicsSvgItem>
#include <QSvgRenderer>
#include <QString>
#include <QDebug>

#include "../common/Types.h"
#include "SvgItemLink.h"

class SvgItemLink;

/**
 * This class is a graphic (SVG) element of type Virtual Machine
 */
class VirtualMachineItem : public QGraphicsSvgItem
{
	Q_OBJECT
	
private:
	QMap<VmType, QString> svgFiles;
	
	//added by Paolo
	QList<SvgItemLink *> links;
	QMenu *myContextMenu;
	
public:
	VirtualMachineItem(VmType type = Host);
	virtual ~VirtualMachineItem();
	
	//added  by Paolo
	QList<SvgItemLink *> getLinks();
    void removeLink(SvgItemLink *link);
    void removeLinks();
    void addLink(SvgItemLink *link);

public slots:
	void changeSvgFile(VmType type);
	

//added by Paolo
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
	
};

#endif /*VIRTUALMACHINEITEM_H_*/
