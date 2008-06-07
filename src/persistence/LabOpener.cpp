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

#include "LabOpener.h"

/**
 * Constructor
 */
LabOpener::LabOpener(QString &labPathToOpen)
{
	labPath = labPathToOpen;
}

/**
 * Deconstructor
 */
LabOpener::~LabOpener()
{
}

/**
 * [PRIVATE]
 * Validate the lab verifing the existance of lab.conf and lab.xml
 * and for each machine its directory
 */
bool LabOpener::validateLab()
{
	bool valid = true;
	
	/* Check the lab.conf and lab.xml existance */
	if(!QFile::exists(labPath + "/lab.conf"))
	{
		valid = false;
		qWarning() << "LabOpener::validateLab()" << "lab.conf don't extst";
	}
	
	if(!QFile::exists(labPath + "/lab.xml"))
	{
		valid = false;
		qWarning() << "LabOpener::validateLab()" << "lab.xml don't extst";
	}
	
	/* Get the content of lab.conf */
	QFile labConf(labPath + "/lab.conf");
	if (!labConf.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		valid = false;
		qWarning() << "LabOpener::validateLab()" << "Unable to open lab.conf";
	}
	
	QString labConfContent(labConf.readAll());
	
	/* Simple parse of lab.conf to check machines */
	QRegExp machineExtract("(^[a-zA-Z0-9]+)\\[.+\\]=.+");
	if(valid)
	{
		int pos = 0;
		
		while((pos = machineExtract.indexIn(labConfContent, pos)) != -1)
		{
			qDebug() << machineExtract.cap(1);
			pos += machineExtract.matchedLength();
		}
	}
	
	return valid;
}
