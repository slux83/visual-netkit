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

#include "Plugin.h"

/* constructor */
Plugin::Plugin(PluginProxy *proxy) : PluginInterface(proxy)
{
	name = "I'm a Plugin";
	description = "I'm doing nothing :3 But i'm here!";
}
