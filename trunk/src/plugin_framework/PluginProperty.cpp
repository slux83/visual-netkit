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

#include "PluginProperty.h"
#include <QDebug>
//for abs()
#include <stdlib.h>

/** 
 * Constructor
 */
PluginProperty::PluginProperty(QString propName, QString defValue, QString desc,
		QString idVal, quint16 minOcc, quint16 maxOcc)
{
	name = propName;
	defaultValue = defValue;
	description = desc;
	id = idVal;
	min = minOcc;
	max = maxOcc;
	copyNumber = 0;
	parent = NULL;
	
	calculateUniqueId();
}

/** 
 * Deconstructor 
 */
PluginProperty::~PluginProperty()
{
	qDeleteAll(childItems);
}

/**
 * Destroy a child in depth
 */
bool PluginProperty::destroyChild(PluginProperty* child)
{
	if(!childItems.contains(child))
		return false;
	
	childItems.removeAll(child);
	
	delete child;
	
	return true;
}

/**
 * [PRIVATE]
 * Init the unique random id
 */
void PluginProperty::calculateUniqueId()
{
	QDateTime currTime = QDateTime::currentDateTime();
	
	QString idPart = currTime.toString("ddHHmmsszzz");
	
	uint randomNumber = abs(qrand());
	
	idPart.append("_" + QString::number(randomNumber));
	
	uniqueId = idPart;
}
