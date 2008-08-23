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

#include "LabPropertyController.h"
#include "LabHandler.h"

#define LAB_VERSION QString("1")
#define LAB_DESCRIPTION QString("2")
#define LAB_AUTHORS QString("3")
#define LAB_EMAIL QString("4")
#define LAB_WEB QString("5")

/**
 * Constructor
 */
LabPropertyController::LabPropertyController() : AbstractPropertyHandler()
{
	lab = NULL;
}

/**
 * Deconstructor
 */
LabPropertyController::~LabPropertyController()
{
}

/**
 * Get the model to attach at view (gui) side on property dock
 */
TreeModel* LabPropertyController::getComposedModel()
{
	QStringList header;
	header << tr("Property") << tr("Value");
	TreeModel *model = new TreeModel(header);
	
	if(!lab)
	{
		qWarning() << "LabPropertyController::getComposedModel() NULL lab";
		return model;
	}
	
	TreeItem *root = model->getRootItem();
	
	/* lab properties */
	TreeItem* last;
	
	//lab version
	root->insertChildren(root->childCount(), 1, root->columnCount());
	root->child(root->childCount() - 1)->setData(0, tr("Version"));
	root->child(root->childCount() - 1)->setData(1, lab->getVersion());
	last = root->child(root->childCount() - 1);
	last->setIsElementProperty(true);
	last->setId(LAB_VERSION);
	last->setDescription(tr("The Laboratory version"));
	last->setPropertyHandler(this);

	//lab description
	root->insertChildren(root->childCount(), 1, root->columnCount());
	root->child(root->childCount() - 1)->setData(0, tr("Description"));
	root->child(root->childCount() - 1)->setData(1, lab->getDescription());
	last = root->child(root->childCount() - 1);
	last->setIsElementProperty(true);
	last->setId(LAB_DESCRIPTION);
	last->setDescription(tr("The Laboratory description"));
	last->setPropertyHandler(this);
	
	//lab author(s)
	root->insertChildren(root->childCount(), 1, root->columnCount());
	root->child(root->childCount() - 1)->setData(0, tr("Author(s)"));
	root->child(root->childCount() - 1)->setData(1, lab->getAuthors());
	last = root->child(root->childCount() - 1);
	last->setIsElementProperty(true);
	last->setId(LAB_AUTHORS);
	last->setDescription(tr("The Laboratory author(s)"));
	last->setPropertyHandler(this);
	
	//lab email 
	root->insertChildren(root->childCount(), 1, root->columnCount());
	root->child(root->childCount() - 1)->setData(0, tr("Email"));
	root->child(root->childCount() - 1)->setData(1, lab->getEmail());
	last = root->child(root->childCount() - 1);
	last->setIsElementProperty(true);
	last->setId(LAB_EMAIL);
	last->setDescription(tr("The Laboratory author's email address"));
	last->setPropertyHandler(this);	
	
	//lab website
	root->insertChildren(root->childCount(), 1, root->columnCount());
	root->child(root->childCount() - 1)->setData(0, tr("Website"));
	root->child(root->childCount() - 1)->setData(1, lab->getWebsite());
	last = root->child(root->childCount() - 1);
	last->setIsElementProperty(true);
	last->setId(LAB_WEB);
	last->setDescription(tr("The Laboratory website"));
	last->setPropertyHandler(this);	
	return model;
}

/**
 * [UNUSED]
 */
TreeModel* LabPropertyController::getInitModel(QList<PluginProxy*> plugins)
{
	Q_UNUSED(plugins);
	
	QStringList header;
	header << tr("Property") << tr("Value");
	return new TreeModel(header);
}

/**
 * 
 */
bool LabPropertyController::saveChangedProperty(TreeItem *treeItem)
{	
	/* Is this property mapped with a Laboratory? */
	if(lab == NULL)
		return false;
	
	QString itemValue = treeItem->data(1).toString();
	
	/* Select the correct property to save inside the domain current lab */
	if(treeItem->getId() == LAB_VERSION)
		lab->setVersion(itemValue);
	
	if(treeItem->getId() == LAB_DESCRIPTION)
			lab->setDescription(itemValue);
	
	if(treeItem->getId() == LAB_AUTHORS)
			lab->setAuthors(itemValue);
	
	if(treeItem->getId() == LAB_EMAIL)
			lab->setEmail(itemValue);
	
	if(treeItem->getId() == LAB_WEB)
			lab->setWebsite(itemValue);
	
	LabHandler::getInstance()->getMainWindow()->writeLogMessage(tr("Lab property saved"));
	return true;
}

/**
 * [UNUSED]
 */
QString LabPropertyController::removePluginProperty(QString pluginName, QString propertyUniqueId)
{
	Q_UNUSED(pluginName);
	Q_UNUSED(propertyUniqueId);
	
	return QObject::tr("You cannot remove properties for laboratory.");
}

/**
 * [UNUSED]
 */
QPair<PluginProperty*, QString> LabPropertyController::addPluginProperty(QString pluginName,
		QString propertyIdToAdd, QString parentPropertyUniqueId)
{
	Q_UNUSED(pluginName);
	Q_UNUSED(propertyIdToAdd);
	Q_UNUSED(parentPropertyUniqueId);
	PluginProperty *p = NULL;
	
	return qMakePair(p, QObject::tr("You cannot add properties for laboratory."));
}

/**
 * [UNUSED]
 */
PluginProxy* LabPropertyController::getPluginFromCurrentElement(QString pluginName)
{
	Q_UNUSED(pluginName);
	
	return NULL;
}

/**
 * Save the property passed, if it's mine
 * Return true if the property is saved correctly
 */
//bool LabPropertyController::saveChangedProperty(QTableWidgetItem * item)
//{
//	bool returnVal = false;
//	
//	/* Is this property mapped with a Laboratory? */
//	if(lab == NULL)
//		return false;
//		
//	/* Select the correct property to save inside the domain current lab */
//	switch (item->data(Qt::UserRole).toInt())
//	{
//		case
//			Version: lab->setVersion(item->data(Qt::DisplayRole).toString());
//			returnVal = true;
//			break;
//
//		case Description: lab->setDescription(item->data(Qt::DisplayRole).toString());
//			break;
//		
//		case 
//			Authors: lab->setAuthors(item->data(Qt::DisplayRole).toString());
//			returnVal = true;
//			break;
//		
//		case 
//			Email: lab->setEmail(item->data(Qt::DisplayRole).toString());
//			returnVal = true;
//			break;
//		
//		case 
//			Website: lab->setWebsite(item->data(Qt::DisplayRole).toString());
//			returnVal = true;
//			break;
//	}
//
//	LabHandler::getInstance()->getMainWindow()->writeLogMessage(tr("Lab property saved"));
//	return returnVal;
//}
//
///**
// * Render lab properties inside property dock
// */
//void LabPropertyController::renderLabProperties(QTableWidget *tableWidget)
//{
//	/* render infos inside the property editor */
//	tableWidget->setRowCount(5);
//
//	QTableWidgetItem *property = new QTableWidgetItem();
//	
//	//Lab version
//	property = new QTableWidgetItem();
//	property->setData(Qt::DisplayRole, tr("Version"));
//	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
//	tableWidget->setItem(0, 0, property);
//	
//	property = new QTableWidgetItem();
//	property->setData(Qt::DisplayRole, lab->getVersion());
//	property->setData(Qt::UserRole, Version);
//	tableWidget->setItem(0, 1, property);
//
//	//Lab description
//	property = new QTableWidgetItem();
//	property->setData(Qt::DisplayRole, tr("Description"));
//	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
//	tableWidget->setItem(1, 0, property);
//	
//	property = new QTableWidgetItem();
//	property->setData(Qt::DisplayRole, lab->getDescription());
//	property->setData(Qt::UserRole, Description);
//	tableWidget->setItem(1, 1, property);
//	
//	//Lab authors
//	property = new QTableWidgetItem();
//	property->setData(Qt::DisplayRole, tr("Authors"));
//	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
//	tableWidget->setItem(2, 0, property);
//	
//	property = new QTableWidgetItem();
//	property->setData(Qt::DisplayRole, lab->getAuthors());
//	property->setData(Qt::UserRole, Authors);
//	tableWidget->setItem(2, 1, property);
//	
//	//Lab email
//	property = new QTableWidgetItem();
//	property->setData(Qt::DisplayRole, tr("E-Mail"));
//	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
//	tableWidget->setItem(3, 0, property);
//	
//	property = new QTableWidgetItem();
//	property->setData(Qt::DisplayRole, lab->getEmail());
//	property->setData(Qt::UserRole, Email);
//	tableWidget->setItem(3, 1, property);
//	
//	//Lab Website
//	property = new QTableWidgetItem();
//	property->setData(Qt::DisplayRole, tr("Website"));
//	property->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);	//not editable
//	tableWidget->setItem(4, 0, property);
//	
//	property = new QTableWidgetItem();
//	property->setData(Qt::DisplayRole, lab->getWebsite());
//	property->setData(Qt::UserRole, Website);
//	tableWidget->setItem(4, 1, property);
//}

