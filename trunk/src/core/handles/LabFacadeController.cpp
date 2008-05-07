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

#include "LabFacadeController.h"
#include "../../gui/handles/LabHandler.h"
#include "../../gui/handles/VmMapper.h"
#include "../../gui/handles/CdMapper.h"

#include "../../persistence/LabSaver.h"
#include "../../persistence/XMLSaver.h"
#include "../../persistence/XMLParser.h"

/**
 * Init the null instance for the singletone controller
 */
LabFacadeController* LabFacadeController::instance = NULL;

/**
 * Constructor
 */
LabFacadeController::LabFacadeController() : QObject()
{
	currentLab = NULL;
	connect(this, SIGNAL(createdNewLab(Laboratory *)),
			LabHandler::getInstance(), SLOT(addCreatedLabOnTree(Laboratory *)));
}

/**
 * Deconstructor
 */
LabFacadeController::~LabFacadeController()
{
}

/**
 * Singletone get instance
 */
LabFacadeController* LabFacadeController::getInstance()
{
	if (instance == NULL)
	{
		instance = new LabFacadeController();
	}

	return instance;
}

/**
 * Create a new laboratory and set it as current laboratory
 * This is also the Factory for laboratory instances
 */
void LabFacadeController::newLaboratory()
{
	/* Check if current lab exist */
	if(currentLab != NULL)
	{
		qWarning() << "Alert, a current lab already exists!";
		return;
		//TODO: manage this scenario (variant)
	}
	
	currentLab = new Laboratory();
	
	emit createdNewLab(currentLab);
}

/**
 * Opens an existent laboratory. TODO
 */
void LabFacadeController::openLab()
{
//	XMLParser xp;
//	xp.loadXML();
//	xp.parseXML();
}

/**
 * Saves the current laboratory. TODO
 */
bool LabFacadeController::saveLab(const QString &savePath)
{
	/* Check if current lab exist */
	if(currentLab != NULL)
	{
		/* join all items inside the scene */
		VmMapper::getInstance()->ensureAllItemsAreJoined();
		CdMapper::getInstance()->ensureAllItemsAreJoined();
		
		// TODO: delegate to different thread
		// saves lab structure dirs and files
		LabSaver ls(savePath);
		// saves lab XML
		XMLSaver xs(savePath + "/" + currentLab->getName());
		
		if (ls.saveLab() && xs.saveLab())
		{
			qDebug() << "Lab saved!";
			return true;
		}
		else
		{
			qWarning() << "Lab not saved...something wrong!";
			return false;
		}
		
	}
	
	return false;
}

