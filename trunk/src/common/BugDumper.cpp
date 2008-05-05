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

#include "BugDumper.h"

/**
 * Constructor
 */
BugDumper::BugDumper()
{
	//connect the sigsegv signal to the dumper
	signal(SIGSEGV, dumper);
}

/**
 * Deconstructor
 */
BugDumper::~BugDumper()
{
}

/**
 * Print a stack trace when program crash
 */
void BugDumper::dumper(int code)
{
	QString backTrace;
	/* Ignore other signals */
	signal(code, SIG_IGN); 
	
	/* manage signal */
	void *array[1024];
	size_t size;
	char **strings;
	size_t i;

	size = backtrace(array, 1024);
	strings = backtrace_symbols(array, size);

	for (i = 0; i < size; i++)
		backTrace.append(strings[i]);
	
	qDebug() << "-------------- PROGRAM CRASH --------------";
	qDebug() << backTrace;
	
}
