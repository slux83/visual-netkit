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

#ifndef ABSTRACTPROPERTYHANDLER_H_
#define ABSTRACTPROPERTYHANDLER_H_

#include <QObject>

#include "../tree_model/TreeModel.h"
#include "../tree_model/TreeItem.h"
#include "../../plugin_framework/PluginProxy.h"

/**
 * Abstract class for property handlers
 * (The handler that dinamically receive the tree changes)
 */
class AbstractPropertyHandler : public QObject
{

public:
	/**
	 * Get a TreeModel that is showed inside the property dock
	 */
	virtual TreeModel* getComposedModel() = 0;

	/**
	 * Get a TreeModel that is showed inside the init plugins property at init time
	 */
	virtual TreeModel* getInitModel(QList<PluginProxy*> plugins) = 0;

	/**
	 * Save a property (need a treeItem->getId() string split to get informations)
	 */
	virtual bool saveChangedProperty(TreeItem *treeItem) = 0;

	/**
	 * Get from the current `Base Element` the proxy of a plugin
	 */
	virtual PluginProxy* getPluginFromCurrentElement(QString pluginName) = 0;

	/**
	 * Remove a plugin property (consistance control by plugin)
	 */
	virtual QString removePluginProperty(QString pluginName, QString propertyUniqueId) = 0;

	/**
	 * Add a new property (with propertyIdToAdd) of a plugin as child of parentProperty(Id|Copy)
	 * Return the new PluginProperty* , or NULL on error with error string as second member of
	 * the QPair.
	 */
	virtual QPair<PluginProperty*, QString> addPluginProperty(QString pluginName, QString propertyIdToAdd,
			QString parentPropertyUniqueId) = 0;
};

#endif /*ABSTRACTPROPERTYHANDLER_H_*/
