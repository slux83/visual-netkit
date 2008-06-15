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

/**
 * This header contains some defines for general purpose
 */

#ifndef COMMONCONFIGS_H_
#define COMMONCONFIGS_H_

#include "Types.h"

/* Visual Netkit Version */
#define VISUAL_NETKIT_VERSION "SVN"

/* Default lab name */
#define DEFAULT_LAB_NAME "unnamed_lab"

/* Config file names */
#define LAB_CONF QString("lab.conf")

/* Graphics label font */
#define GRAPHICS_FONT QFont("Sand Serif", 10, QFont::Bold)
#define GRAPHICS_FONT_SHARED QFont("Sand Serif", 9, QFont::Normal)

/* Default name for XML lab configuration file */
#define XML_DEFAULT_FILE_NAME QString("lab.xml")

/* Default plugin folder */
#define DEFAULT_PLUGIN_DIR QString("./bin/plugins")

/* Separator character */
#define SEPARATOR QString(QChar(149))

/* PNG file compression for exporting */
#define PNG_COPRESSION 30

/* File editor font */
#define FILE_EDITOR_FONT QFont("Monospace", 9, QFont::Normal)


#endif /*COMMONCONFIGS_H_*/
