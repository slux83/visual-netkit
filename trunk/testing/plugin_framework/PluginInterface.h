/**
 * A simple plugin architecture.
 * Copyright (C) 2008  Alessio Di Fazio
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

#ifndef PLUGIN_INTERFACE_H_
#define PLUGIN_INTERFACE_H_

#include <QObject>
#include <QString>

/**
 * A simple plugin interface (pure abstract class)
 */
class PluginInterface : public QObject
{	
public:
	virtual QString getDescription() = 0;
};

/**
 *  the types of factories
 */
typedef PluginInterface* createPlugin_t();
typedef void destroyPlugin_t(PluginInterface*);

#endif
