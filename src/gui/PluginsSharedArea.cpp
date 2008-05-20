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

#include "PluginsSharedArea.h"
#include "../common/CommonConfigs.h"
#include <QFont>

/**
 * Costructor
 */
PluginsSharedArea::PluginsSharedArea() : QGraphicsTextItem()
{
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
	setZValue(1099);
	setFont(GRAPHICS_FONT_SHARED);
}

/**
 * Deconstructor
 */
PluginsSharedArea::~PluginsSharedArea()
{
}

/**
 * Change a plugin line
 * Called by the plugin proxy
 */
void PluginsSharedArea::changeMyLine(QString pluginName, QString content)
{
	pluginsArea.insert(pluginName, content);
	
	//need a repaint. emit the signal
	emit needAreaRepaint();
}

/**
 * [PRIVATE-SLOT]
 * Repaint/Recreate the shared text area
 */
void PluginsSharedArea::updateArea()
{
	/* get all contents */
	QStringList contents(pluginsArea.values());
	
	if(contents.size() == 0)
	{
		//hide the label area
		setVisible(false);
	}
	else
	{
		this->setPlainText(contents.join("\n"));
		setVisible(true);
	}
}
