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

#ifndef LABFACADECONTROLLER_H_
#define LABFACADECONTROLLER_H_

#include <QObject>

#include "../Laboratory.h"

/**
 * The facade controller for the domain layer
 * This object is acceded by Singleton and manage operations with the current
 * Laboratory
 */
class LabFacadeController : QObject
{
	Q_OBJECT
	
private:
	static LabFacadeController *instance; 
	Laboratory *currentLab;
	
public:
	LabFacadeController();
	virtual ~LabFacadeController();
	static LabFacadeController * getInstance();
	void newLaboratory();
	void openLab();
	bool saveLab(QString const &savePath);
	Laboratory * getCurrentLab() { return currentLab; };

signals:
	void createdNewLab(Laboratory *newLab);
	void savedLab(Laboratory *currentLab);
};

#endif /*LABFACADECONTROLLER_H_*/
