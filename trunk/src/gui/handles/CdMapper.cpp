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

#include "CdMapper.h"
#include "SceneTreeMapper.h"
#include <QListIterator>

/**
 * Init the null instance for the singletone controller
 */
CdMapper* CdMapper::instance = NULL;

/**
 * Constructor
 */
CdMapper::CdMapper() : QObject()
{
	addCdForm = new AddCdForm();
}

/**
 * Deconstructor
 */
CdMapper::~CdMapper()
{
}

/**
 * Singletone get instance
 */
CdMapper * CdMapper::getInstance()
{
	if (instance == NULL)
	{
		instance = new CdMapper();
	}

	return instance;
}

/**
 * Show the dialog
 */
void CdMapper::showAddCdForm(QPointF pos)
{
	addCdForm->setCollisionDomainPos(pos);
	addCdForm->show();
}

/**
 * Add a new mapping
 */
void CdMapper::addNewMapping(CollisionDomainItem *cdItem, CollisionDomain *cd)
{
	//add mapping
	mappings.insert(cdItem, cd);
	LabHandler *labHandler = LabHandler::getInstance();
	labHandler->getMainWindow()->graphicsView->scene()->addItem(cdItem);
	labHandler->getMainWindow()->graphicsView->ensureVisible(cdItem);
	
	labHandler->getMainWindow()->writeLogMessage(tr("Created a new collision domain "));
	
	/* Add element inside the scene tree */
	SceneTreeMapper::getInstance()->addCdElement(cdItem);
}

/**
 * Update the counter of the collision domain item because a link is added
 */
void CdMapper::updateCdCounter(CollisionDomain *cd, bool increase)
{
	mappings.key(cd)->increaseDecreaseLinkCounter(increase);
}

/**
 * Ensure that all graphics items are in a joined state (label is joined inside
 * the group)
 */
void CdMapper::ensureAllItemsAreJoined()
{
	QListIterator<CollisionDomainItem*> itemIterator(mappings.keys());
	while(itemIterator.hasNext())
		itemIterator.next()->restoreGroupActionCalled();
}
