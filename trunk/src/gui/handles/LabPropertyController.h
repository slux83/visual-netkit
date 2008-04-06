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

#include "../../core/Laboratory.h"

#include <QTableWidgetItem>
#include <QObject>
#include <QLinkedList>
#include <QPair>
#include <QMessageBox>
#include <QObject>
#include <QTableWidget>

class LabHandler;

/**
 * Class that map each property item (for lab) with the lab domain object
 */
class LabPropertyController : public QObject
{
	
private:
	/* the pair: <VIEW, DOMAIN> */
	QLinkedList<QTableWidgetItem *> myProperties;
	Laboratory *lab;
	
public:
	LabPropertyController();
	virtual ~LabPropertyController();
	
	void addProperty(QTableWidgetItem *p) { myProperties.append(p); };
	void clearMapping() { myProperties.clear(); };
	bool saveChangedProperty(QTableWidgetItem *item);
	void renderLabProperties(QTableWidget *tableWidget);
	void clearPropertyDock(QTableWidget *tableWidget);
	
	//Setter functions
	void setLab(Laboratory *l) { lab = l; };
};

#endif /*LABPROPERTYCONTROLLER_H_*/
