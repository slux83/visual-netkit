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

#include "AreaController.h"
#include "LabHandler.h"
#include "../AreaItem.h"
#include "../undoCommands/UndoCommands.h"

/* Init the NULL instance */
AreaController* AreaController::instance = NULL;

#define AREA_TEXT "AREA_TEXT"

/**
 * Constructor
 */
AreaController::AreaController() : AbstractPropertyHandler()
{
	currentArea = NULL;
}

/**
 * Deconstructor
 */
AreaController::~AreaController()
{
}

/**
 * [STATIC]
 * Controller singletone get instance
 */
AreaController* AreaController::getInstance()
{
	if(instance == NULL)
		instance = new AreaController();
	
	return instance;
}

/**
 * Add a new area inside the areas list
 */
void AreaController::addArea(AreaItem* newArea)
{
	areas.append(newArea);
	
	//Set the log
	LabHandler::getInstance()->getMainWindow()->writeLogMessage(tr("Added an area"));
	
	//push to undo stack
	LabHandler::getInstance()->getUndoStack()->push(new AddAreaCommand(newArea));
}

/**
 * Foward to lab handler
 */
void AreaController::setChangedLabState()
{
	LabHandler::getInstance()->setChangedLabState();
}

/**
 * Destroy an area
 */
void AreaController::deleteArea(AreaItem *aItem)
{
	if(!aItem->scene())
	{
		qWarning() << "AreaController::deleteArea()" << "Area item have NULL scene";
		return;
	}
	
	//delete
	aItem->scene()->removeItem(aItem);
	areas.removeAll(aItem);
	
	//Set the log
	LabHandler::getInstance()->getMainWindow()->writeLogMessage(tr("Area deleted"));
	
	//push to undo stack
	LabHandler::getInstance()->getUndoStack()->push(new DeleteAreaCommand(aItem));
	
	//clear the property dock
	LabHandler::getInstance()->getMainWindow()->clearPropertyDock();
}

/**
 * Handle the link item selection and render the properties
 */
void AreaController::renderAreaProperties(AreaItem *area)
{
	currentArea = area;
	
	LabHandler *labHandler = LabHandler::getInstance();
	
	/* Clear the property editor and render properties */
	labHandler->getMainWindow()->clearPropertyDock(getComposedModel());
}

/**
 * Clear the controller
 */
void AreaController::clear()
{
	currentArea = NULL;
	areas.clear();
}

/**
 * Load (create) a saved area
 */
void AreaController::loadNewArea(QPointF pos, qreal width,
		qreal height, qreal z, QString color, QString text)
{
	//make a new area
	AreaItem *area = new AreaItem();
	area->setPos(pos);
	area->setRect(0, 0, width, height);
	area->setZValue(z);
	
	//create the color
	QStringList rgb = color.split(',');
	QColor areaBg;
	if(rgb.size() == 3)
	{
		areaBg.setRed(rgb[0].toInt());
		areaBg.setGreen(rgb[1].toInt());
		areaBg.setBlue(rgb[2].toInt());
	}
	
	//attach the color if valid
	if(areaBg.isValid())
		area->setCurrentCorol(areaBg);
	else
		qWarning() << "Color" << color << "is invalid as area background." << "Setting default color.";
	
	//text
	area->setLabel(text);
	
	//save and draw
	areas.append(area);
	LabHandler::getInstance()->getMainWindow()->getLabScene()->addItem(area);
	
}

/**
 * Get the model to attach at view (gui) side on property dock
 */
TreeModel* AreaController::getComposedModel()
{
	QStringList header;
	header << tr("Property") << tr("Value");
	TreeModel *model = new TreeModel(header);
	
	if(!currentArea)
	{
		qWarning() << "AreaController::getComposedModel() NULL current area";
		return model;
	}
	
	TreeItem *root = model->getRootItem();
	
	/* lab properties */
	TreeItem* last;
	
	//area text
	root->insertChildren(root->childCount(), 1, root->columnCount());
	root->child(root->childCount() - 1)->setData(0, tr("Text"));
	root->child(root->childCount() - 1)->setData(1, currentArea->getLabel());
	last = root->child(root->childCount() - 1);
	last->setIsElementProperty(true);
	last->setId(AREA_TEXT);
	last->setDescription(tr("The Area text"));
	last->setPropertyHandler(this);
	
	return model;
}

/**
 * [UNUSED]
 */
TreeModel* AreaController::getInitModel(QList<PluginProxy*> plugins)
{
	Q_UNUSED(plugins);
	
	QStringList header;
	header << tr("Property") << tr("Value");
	return new TreeModel(header);
}

/**
 * Save a property changed
 */
bool AreaController::saveChangedProperty(TreeItem *treeItem)
{	
	/* Is this property mapped with a Laboratory? */
	if(currentArea == NULL)
		return false;
	
	QString itemValue = treeItem->data(1).toString();
	
	/* Select the correct property to save inside the current area */
	if(treeItem->getId() == AREA_TEXT)
		currentArea->setLabel(itemValue);
	
	LabHandler::getInstance()->getMainWindow()->writeLogMessage(tr("Area property saved"));
	
	return true;
}

/**
 * Write a log line
 */
void AreaController::writeLogLine(QString text)
{
	LabHandler::getInstance()->getMainWindow()->writeLogMessage(text);
}

/**
 * [UNUSED]
 */
QString AreaController::removePluginProperty(QString pluginName, QString propertyUniqueId)
{
	Q_UNUSED(pluginName);
	Q_UNUSED(propertyUniqueId);
	
	return QObject::tr("You cannot remove properties for areas.");
}

/**
 * [UNUSED]
 */
QPair<PluginProperty*, QString> AreaController::addPluginProperty(QString pluginName,
		QString propertyIdToAdd, QString parentPropertyUniqueId)
{
	Q_UNUSED(pluginName);
	Q_UNUSED(propertyIdToAdd);
	Q_UNUSED(parentPropertyUniqueId);
	PluginProperty *p = NULL;
	
	return qMakePair(p, QObject::tr("You cannot add properties for areas."));
}

/**
 * [UNUSED]
 */
PluginProxy* AreaController::getPluginFromCurrentElement(QString pluginName)
{
	Q_UNUSED(pluginName);
	
	return NULL;
}
