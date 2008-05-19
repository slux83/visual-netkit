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

#ifndef SCENETREEMAPPER_H_
#define SCENETREEMAPPER_H_

#include <QObject>
#include <QMap>
#include <QTreeWidget>
#include "../VirtualMachineItem.h"
#include "../CollisionDomainItem.h"
#include "../LinkItem.h"
#include "LabHandler.h"

/* Icon defines */
#define LAB_ICON	QString(":/medium/lab_home")
#define VM_ICON		QString(":/medium/vm")
#define CD_ICON		QString(":/medium/cd")
#define ETH_ICON	QString(":/medium/eth")

/**
 * [SINGLETONE]
 * This is a controller for tree widget that show the tree of scene elements.
 * 
 * This class also maps all graphics group items with the corrispective tree item
 * inside the tree scene view.
 */
class SceneTreeMapper : public QObject
{
	Q_OBJECT

private:
	static SceneTreeMapper* instance;
	QTreeWidget *sceneTree;
	QMap<VirtualMachineItem*, QTreeWidgetItem*> vmMap;
	QMap<CollisionDomainItem*, QTreeWidgetItem*> cdMap;
	QMap<LinkItem*, QTreeWidgetItem*> linkMap;
	QTreeWidgetItem *rootElement;
	
private slots:
	void handleItemClicked(QTreeWidgetItem *item, int column);

public:
	SceneTreeMapper();
	virtual ~SceneTreeMapper();
	static SceneTreeMapper* getInstance();
	void createRootElement();
	void addVmElement(VirtualMachineItem* vmItem);
	void addCdElement(CollisionDomainItem* cdItem);
	void addEthernetElement(LinkItem* linkItem);
};

#endif /*SCENETREEMAPPER_H_*/
