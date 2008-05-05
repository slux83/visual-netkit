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

#ifndef BUGDUMPER_H_
#define BUGDUMPER_H_

/* ISO headers */
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>

/* Qt4 headers */
#include <QDebug>
#include <QString>

/**
 * This class is a trace dumper expert to dump a stack-trace when the 
 * program crash 
 */
class BugDumper
{
private:
	static void dumper(int code);
	
public:
	BugDumper();
	virtual ~BugDumper();
};

#endif /*BUGDUMPER_H_*/
