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
bool XMLExpert::dumpDocument(QDomDocument *doc, QString labPath)
{	
	QFile data(labPath + "/" + XML_DEFAULT_FILE_NAME);
	
	/* Save a backup */
	if(data.exists())
	{
		QFile::rename(labPath + "/" + XML_DEFAULT_FILE_NAME,
				labPath + "/" + XML_DEFAULT_FILE_NAME + "~");
	}
	
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
 * Dump the labpath/lab.xml contend inside the document
 * @return empty document with error variable setted on error
 */ 
QDomDocument XMLExpert::readDocument(QString labPath, QString *error)
{
	//preparo il documento
	QDomDocument doc;
	QString errorMsg;
	int errorLine, errorColumn;
	
	QFile data(labPath.append("/").append(XML_DEFAULT_FILE_NAME));
	
	if(!data.open(QFile::ReadOnly))
	{
		if(error != NULL)
			error->append("Unable to open the lab.xml in read only mode inside ").append(labPath);
		return doc;
	}
	
	if (!doc.setContent(&data, &errorMsg, &errorLine, &errorColumn))
	{
		data.close();
		
		qWarning()	<< "Xml file invalid."
					<< "\nError:" << errorMsg
					<< "\nline:" << errorLine
					<< "\ncolumn:" << errorColumn;
		
		if(error != NULL)
			error->append("Invalid lab.xml inside ").append(labPath);
		
		return doc;
	}
	
	data.close();
	
	return doc;
}

