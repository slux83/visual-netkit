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

#ifndef PLUGINSSHAREDAREA_H_
#define PLUGINSSHAREDAREA_H_

#include <QGraphicsTextItem>
#include <QMap>
#include <QString>

/**
 * This class is an extension of QGraphiscTextItem and contain a multi-line
 * area managed by plugins
 */
class PluginsSharedArea : public QGraphicsTextItem
{
	Q_OBJECT
	
private:
	//key:		PluginName
	//value:	PluginManagedLine
	QMap<QString, QString> pluginsArea;
	
private slots:
	void updateArea();
	
signals:
	void needAreaRepaint();
	
public:
	PluginsSharedArea();
	virtual ~PluginsSharedArea();
	void changeMyLine(QString pluginName, QString content);
	QMap<QString, QString> getPluginsArea() { return pluginsArea;};
};

#endif /*PLUGINSSHAREDAREA_H_*/
