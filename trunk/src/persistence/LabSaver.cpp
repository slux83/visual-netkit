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

#include "LabSaver.h"

LabSaver::LabSaver()
{
}

LabSaver::~LabSaver()
{
}

/**
 * Saves current lab to filesystem.
 */
bool LabSaver::saveLab()
{
	bool allok = true;
	return allok;
}

/**
 * Saves current lab configuration to lab.conf file.
 */
bool LabSaver::saveLabConf()
{
	bool allok = true;
	return allok;
}

/**
 * Saves passed router or host configuration to filesystem.
 * This function creates both routerX.conf file and routerX folder (including subdirectories and subfiles).
 */
bool LabSaver::saveRouterConf()
{
	bool allok = true;
	return allok;
}

