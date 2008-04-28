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

#include "XMLExpert.h"
#include <QDebug>

XMLExpert::XMLExpert()
{
}

XMLExpert::~XMLExpert()
{
}

/**
 * Writes to filesystem the passed QDomDocument. If the specified file exists
 * overwrites it.
 */
bool XMLExpert::dumpDocument(QDomDocument *doc, QString *filename)
{	
	QString *file;
	if (filename->isNull() || filename->isEmpty())
		*file = XML_DEFAULT_FILE_NAME;
	else 
		file = filename;
	
	QFile data(*file);
	if (data.open(QFile::WriteOnly | QFile::Truncate)) 
	{
		QTextStream out(&data);
	    doc->save(out, 4, QDomNode::EncodingFromDocument);
	    return true;
	}
	return false;
}

/**
 * Reads the file named "filename" and returns a QDomDocument. If the file is 
 * empty or null returns an empty QDomDocument.
 */ 
QDomDocument* XMLExpert::readDocument(QString *filename)
{
	//preparo il documento
	QDomDocument *doc = new QDomDocument();
	
	//controllo il valore della stringa filename
	if (!filename->isNull() && !filename->isEmpty()) {
		QFile data(*filename);
		if (data.open(QFile::ReadOnly)) 
		{
			QTextStream in(&data);
			QString result = in.readAll();
			doc->setContent(result, new QString("\nError reading XML file."), 0, 0);
			
			// DEBUG CODE
			//QTextStream out(stdout);
			//out << "---- START STREAMING ----" << endl;
			//out << doc->toString();
		}
	}
	return doc;
}

