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

#ifndef AREACONTROLLER_H_
#define AREACONTROLLER_H_

#include <QObject>
#include <QDebug>
#include <QTableWidget>

#include "AbstractPropertyHandler.h"

class AreaItem;

/**
 * Singletone class to control all areas and used also as
 * areas property controller
 */ 
class AreaController : public AbstractPropertyHandler
{
	
private:
	static AreaController* instance;
	QList<AreaItem *> areas;
	AreaItem* currentArea;
		
public:
	AreaController();
	virtual ~AreaController();
	static AreaController* getInstance();
	void loadNewArea(QPointF pos, qreal width, qreal height, QString color, QString text);
	
	QList<AreaItem *> getAreas() { return areas; };
	void renderAreaProperties(AreaItem* area);
	void addArea(AreaItem* newArea);
	void setChangedLabState();
	void deleteArea(AreaItem *aItem);
	void clear();
	
	//[IMPLEMENT] AbstractPropertyHandler
	TreeModel* getComposedModel();
	TreeModel* getInitModel(QList<PluginProxy*> plugins);
	bool saveChangedProperty(TreeItem *treeItem);
	QString removePluginProperty(QString pluginName, QString propertyUniqueId);
	QPair<PluginProperty*, QString> addPluginProperty(QString pluginName, QString propertyIdToAdd,
				QString parentPropertyUniqueId);
	PluginProxy* getPluginFromCurrentElement(QString pluginName);
};

#endif /*AREACONTROLLER_H_*/
