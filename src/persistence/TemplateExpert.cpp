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

#include "TemplateExpert.h"

/**
 * Constructor
 */
TemplateExpert::TemplateExpert()
{
}

/**
 * Deconstructor
 */
TemplateExpert::~TemplateExpert()
{
}

QByteArray TemplateExpert::template2string(QString tpl)
{
	QByteArray fileContent;
	
	QFile file(tpl);
	
	//open file
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return fileContent;
	
	//dump content
	while (!file.atEnd())
	{
		fileContent.append(file.readLine());
	}
	
	/* Replace the version */
	fileContent.replace(QString("<VISUAL_NETKIT_VERSION>"), VISUAL_NETKIT_VERSION);
	
	/* Junks for test */
	//qDebug() << "template:" << *fileContent;
	//QRegExp hostReg("<TOPOLOGY>(.+)</TOPOLOGY>");
	//qDebug() << "cap:" << hostReg.cap(1);
	/******************/
	
	return fileContent;
}
