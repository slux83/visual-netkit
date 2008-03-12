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

#ifndef LABPROPERTYCONTROLLER_H_
#define LABPROPERTYCONTROLLER_H_

#include "../core/Laboratory.h"
#include <QTableWidgetItem>
#include <QObject>
#include <QLinkedList>
#include <QPair>

/**
 * Singletone class that map each property item (for lab) with the lab domain
 * object
 */
class LabPropertyController
{
	
private:
	static LabPropertyController *instance;
	
	/* the pair: <VIEW, DOMAIN> */
	QLinkedList< QPair<QTableWidgetItem *, Laboratory *> > mapping;
	
	
public:
	LabPropertyController();
	virtual ~LabPropertyController();
	static LabPropertyController * getInstance();
	void addMapping(QPair<QTableWidgetItem *, Laboratory *> m) { mapping.append(m); };
	void clearMapping() { mapping.clear(); };
	Laboratory * getLabGivedView(QTableWidgetItem *item);
};

#endif /*LABPROPERTYCONTROLLER_H_*/
