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

#include "LabPropertyController.h"

/**
 * Init the null instance for the singletone controller
 */
LabPropertyController* LabPropertyController::instance = NULL;

/**
 * Constructor
 */
LabPropertyController::LabPropertyController() : QObject()
{
}

/**
 * Constructor
 */
LabPropertyController::~LabPropertyController()
{
}

/**
 * Singletone get instance
 */
LabPropertyController * LabPropertyController::getInstance()
{
	if (instance == NULL)
	{
		instance = new LabPropertyController();
	}

	return instance;
}

/**
 * Get the lab (domain object) gived the view object
 */
Laboratory * LabPropertyController::getLabGivedView(QTableWidgetItem * item)
{
	Laboratory *ret = NULL;
	QLinkedListIterator< QPair<QTableWidgetItem *, Laboratory *> > i(mapping);
	while (i.hasNext())
	{
		QPair<QTableWidgetItem *, Laboratory *> m = i.next();
		if(m.first == item)
			ret = m.second;
	}
	
	if(ret == NULL)
		qWarning() << "Mapping QPair<QTableWidgetItem *, Laboratory *>: Laboratory is NULL.";
	
	return ret;
}
