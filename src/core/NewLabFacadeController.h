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

#ifndef VISUALNETKITFACADECONTROLLER_H_
#define VISUALNETKITFACADECONTROLLER_H_

#include "Laboratory.h"

/**
 * The facade controller for the domain layer
 * This object is acceded by Singleton and manage operations for the scenario
 * 'New Laboratory'
 */
class NewLabFacadeController
{
	
private:
	static NewLabFacadeController *instance; 
	Laboratory *currentLab;
	
public:
	NewLabFacadeController();
	virtual ~NewLabFacadeController();
	static NewLabFacadeController * getInstance();
};

#endif /*VISUALNETKITFACADECONTROLLER_H_*/
