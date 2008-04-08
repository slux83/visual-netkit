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
#define VISUAL_NETKIT_VERSION QString("SVN")

/* Config file names */
#define LAB_CONF QString("lab.conf")
#define ZEBRA_DAEMONS_CONF QString("daemons")
#define BGPD_CONF QString("bgpd.conf")
#define RIPD_CONF QString("ripd.conf")

/* Config file paths */
#define ZEBRA_PATH QString("zebra")
#define ETC_PATH QString("etc")

/* ZEBRA Daemons list */
inline QList<Daemon> getDaemons()
{
	QList<Daemon> l;
	l << Zebra << Bgpd << Ripd;
	qSort(l.begin(), l.end());
	return l;
}

/* Graphics label font */
#define GRAPHICS_FONT QFont("Sand Serif", 10)


#endif /*COMMONCONFIGS_H_*/
