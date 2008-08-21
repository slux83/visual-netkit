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

#ifndef PROPERTYEXPERT_H_
#define PROPERTYEXPERT_H_

#include <QMap>
#include <QPair>

#include "PluginProperty.h"

class QDomDocument;
class QDomElement;

/**
 * This is the class that manage plugin's properties and the xml description of
 * these.
 * It's also a factory of properties and it's accessed by plugin proxy
 */
class PropertyExpert
{
	
private:
	QString xmlPluginRes;
	bool isXmlValid;
	
	QDomDocument parseDocument();
	bool validateXml();
	PluginProperty* buildPropertyDepth(QDomElement propNode, PluginProperty* parent = NULL,
			PluginProperty* current = NULL);
	PluginProperty * searchPropertyPrivate(PluginProperty *parent, QString pUid);

public:
	PropertyExpert(QString &xmlRes);
	virtual ~PropertyExpert();
	QList<PluginProperty*> buildBaseProperties();
	QMap<QString, QString> parseXmlGlobalInfo();
	bool isXmlConfValid() { return isXmlValid; };
	PluginProperty * searchProperty(QList<PluginProperty*> properties, QString pUid);
	QMap<QString, QString> getPropertyInfo(QString propertyId);
	PluginProperty * newProperty(QString propName, QString defValue, QString desc,
			QString idVal, quint16 minOcc, quint16 maxOcc);
	
	// pair: <child name, child id>
	QList< QPair<QString, QString> > getChildsByParentId(QString id);
	QList< QPair<QString, QString> > getRootChilds();
};

#endif /*PROPERTYEXPERT_H_*/
