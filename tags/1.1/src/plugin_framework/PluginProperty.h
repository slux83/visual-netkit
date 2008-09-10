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

#ifndef PLUGINPROPERTY_H_
#define PLUGINPROPERTY_H_

#include <QString>
#include <QList>
#include <QDateTime>

/**
 * A plugin property
 */
class PluginProperty
{

private:
	QString name;
	QString defaultValue;
	QString description;
	QString value;
	QString id;
	QString uniqueId;
	quint16 min, max;	//min and max occurrences
	quint16 copyNumber;
	QList<PluginProperty*> childItems;
	PluginProperty* parent;
	
	void calculateUniqueId();
	
public:
	PluginProperty(QString propName, QString defValue, QString desc,
			QString idVal, quint16 minOcc, quint16 maxOcc);
	
	virtual ~PluginProperty();
	
	QString getName() { return name; };
	QString getDefaultValue() { return defaultValue; };
	QString getDescription() { return description; };
	QString getValue() { return value; };
	QString getId() { return id; };
	quint16 getMinOcc() { return min; };
	quint16 getMaxOcc() { return max; };
	QList<PluginProperty*> getChilds() { return childItems; };
	PluginProperty * getParent() { return parent; };
	quint16 getCopy() { return copyNumber; };
	QString getUniqueId() { return uniqueId; };
	
	void setName(QString &n) { name = n; };
	void setDefaultValue(QString &d) { defaultValue = d; };
	void setValue(QString &v) { value = v; };
	void setDescription(QString &d) { description = d; };
	void setId(QString idVal) { id = idVal; };
	void setUniqueId(QString uId) { uniqueId = uId; };
	void setCopy(quint16 copy) { copyNumber = copy; };
	void setParent(PluginProperty* myParent) { parent = myParent; };
	
	void appendChild(PluginProperty* prop) { childItems.append(prop); };
	void appendChilds(QList<PluginProperty*> props) { childItems << props; };
	bool destroyChild(PluginProperty* child);
	
};

#endif /*PLUGINPROPERTY_H_*/
