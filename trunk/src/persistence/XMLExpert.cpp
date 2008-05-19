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

/**
 * Constructor
 */
XMLExpert::XMLExpert()
{
}

/**
 * Deconstructor
 */
XMLExpert::~XMLExpert()
{
}

/**
 * [STATIC]
 * Writes to filesystem the passed QDomDocument. If the specified file exists
 * overwrites it.
 */
bool XMLExpert::dumpDocument(QDomDocument *doc, QString path)
{	
	QFile data(path + "/" + XML_DEFAULT_FILE_NAME);
	bool returnVal = false;
	
	if (data.open(QFile::WriteOnly | QFile::Truncate)) 
	{
		QTextStream out(&data);
	    doc->save(out, 4, QDomNode::EncodingFromDocument);
	    returnVal = true;
	}
	
	//delete the dom
	delete doc;
	
	return returnVal;
}

/**
 * [STATIC]
 * Reads the file named "filename" and returns a QDomDocument. If the file is 
 * empty or null returns an empty QDomDocument.
 */ 
QDomDocument* XMLExpert::readDocument()
{
	//preparo il documento
	QDomDocument *doc = new QDomDocument();
	
	QFile data(XML_DEFAULT_FILE_NAME);
	if (data.open(QFile::ReadOnly)) 
	{
		QTextStream in(&data);
		QString result = in.readAll();
		doc->setContent(result, new QString("\nError reading XML file."), 0, 0);
	}
	
	return doc;
}
