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

#include "LabOpener.h"
#include "../gui/handles/LabHandler.h"
#include "../gui/handles/VmMapper.h"
#include "../gui/handles/CdMapper.h"
#include "../gui/handles/LinkMapper.h"
#include "../gui/VirtualMachineItem.h"
#include "XMLParser.h"

/**
 * Constructor
 */
LabOpener::LabOpener(QString &labPathToOpen) : QObject()
{
	labPath = labPathToOpen;
	errorString = "Unknown error";
}

/**
 * Deconstructor
 */
LabOpener::~LabOpener()
{
}

/**
 * Start open lab steps
 */
void LabOpener::open()
{
	errorString.clear();
	
	//STEP 0
	if(!validateLab())
		return;
	
	//STEP 1
	if(!createLab())
		return;
	
	//STEP 2
	if(!fetchMachines())
		return;
	
	//STEP 3
	if(!fetchCds())
		return;
	
	//STEP 4
	if(!fetchHis())
		return;
	
	//STEP 5
	if(!createGraphicElements())
		return;

	//STEP 6 - last step
	if(loadPlugins())
		emit loadStepDone(7, true);
}

/**
 * [PRIVATE]
 * 
 * ## STEP-0 ##
 * Validate the lab verifing the existance of lab.conf and lab.xml
 * and for each machine its directory
 */
bool LabOpener::validateLab()
{
	bool valid = true;
	
	/* Check the lab.conf and lab.xml existance */
	if(!QFile::exists(labPath + "/lab.conf"))
	{
		valid = false;
		qWarning() << "LabOpener::validateLab()" << "lab.conf doesn't exist";
		errorString = "lab.conf doesn't exist.";
	}
	
	if(valid && !QFile::exists(labPath + "/lab.xml"))
	{
		valid = false;
		qWarning() << "LabOpener::validateLab()" << "lab.xml doesn't exist";
		errorString = "lab.xml doesn't exist.";
	}
	
	/* Get the content of lab.conf */
	QFile labConf(labPath + "/lab.conf");
	if (valid && !labConf.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		valid = false;
		qWarning() << "LabOpener::validateLab()" << "Unable to open lab.conf";
		errorString = "Unable to open lab.conf";
	}
	
	if(valid)	//continue if valid
	{
		/* Each subdir is a virtual machine. Checking the lab.conf consistency */
		QString labConfContent(labConf.readAll());
		
		//Validate the string HOST_NAME[ETH_NUMBER]="COLLISION_DOMAIN"
		QRegExp machineRegExp("^([a-zA-Z0-9]+)\\[.+\\]=\"?.+\"?");
		
		QStringList lines = labConfContent.split("\n", QString::SkipEmptyParts);
		foreach(QString line, lines)
		{
			machineRegExp.indexIn(line);
			QStringList capLine = machineRegExp.capturedTexts();
			if(capLine.size() == 2)
			{
				//Check the machine dir if exist
				QString machineName = capLine[1];
				if(!QDir(labPath + "/" + machineName).exists())
				{
					qWarning()	<< "LabOpener::validateLab() machine"
								<< machineName
								<< "has been declared inside lab.conf without its own directory.";
					errorString = "machine " + machineName +
						" has been declared inside lab.conf without its own directory.";
					valid = false;
				}
			}
		}
	}
	
	if(valid)	//update the paths to uncheck on tree
		LabHandler::getInstance()->setExcludedPaths(XMLParser::getExcludedPaths(labPath));
	
	emit loadStepDone(0, valid);
	return valid;
}

/**
 * [PRIVATE]
 * 
 * ## STEP-1 ##
 * Create the lab
 */
bool LabOpener::createLab()
{
	bool valid = true;
	QString labContent;
	
	QDir labRoot(labPath);
	if(!labRoot.exists())
	{
		valid = false;
		errorString = tr("Lab root") + " " + labPath + " " + tr("don't exists."); 
	}
	
	/* Get the content of lab.conf */
	QFile labConf(labPath + "/lab.conf");
	if (valid && !labConf.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		valid = false;
		qWarning() << "LabOpener::createLab()" << "Unable to open lab.conf";
		errorString = "Unable to open lab.conf";
	}
	
	if(valid)
	{
		//Init a new laboratory
		LabHandler::getInstance()->openLab(labPath);
		
		QString error;
		QRectF sceneSize = XMLParser::getSceneSize(labPath, &error);
		
		if(!error.isEmpty())
		{
			errorString = error;
			valid = false;
		}
		
		if(valid)
			LabHandler::getInstance()->setSceneSize(sceneSize);
		
		//Get the lab properites
		QRegExp propertiesRegExp("^LAB_(DESCRIPTION|VERSION|AUTHOR|EMAIL|WEB)=(.+)");
		
		//get content and init lab properties
		labContent.append(labConf.readAll());
		foreach(QString line, labContent.split('\n', QString::SkipEmptyParts))
		{
			propertiesRegExp.indexIn(line);
			QStringList caps = propertiesRegExp.capturedTexts();
			
			// Clear value from sorrounding quotes
			QString value = caps[2];
			if(value.size() >= 2)
			{
				if(value.at(0) == QChar('"') || value.at(0) == QChar('\''))
					value = value.remove(0, 1);
				
				if(value.at(value.size() - 1) == QChar('"') || value.at(value.size() - 1) == QChar('\''))
					value = value.remove(value.size() - 1, 1);
			}
			
			if(caps[1] == "DESCRIPTION")
				LabFacadeController::getInstance()->getCurrentLab()->setDescription(value);
			
			if(caps[1] == "VERSION")
				LabFacadeController::getInstance()->getCurrentLab()->setVersion(value);
			
			if(caps[1] == "AUTHOR")
				LabFacadeController::getInstance()->getCurrentLab()->setAuthors(value);
			
			if(caps[1] == "EMAIL")
				LabFacadeController::getInstance()->getCurrentLab()->setEmail(value);
			
			if(caps[1] == "WEB")
				LabFacadeController::getInstance()->getCurrentLab()->setWebsite(value);						
		}
	}
	
	emit loadStepDone(1, valid); 
	
	return valid;
}

/**
 * [PRIVATE]
 * 
 * ## STEP-2 ##
 * Create Virtual machines for each subdir
 */
bool LabOpener::fetchMachines()
{
	bool valid = true;
	QDir labRoot(labPath);
	
	QStringList machines = labRoot.entryList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
	foreach(QString machine, machines)
	{
		VirtualMachine *vm = VmFacadeController::getInstance()->createNewVirtualMachine(machine);
		LabFacadeController::getInstance()->getCurrentLab()->addMachine(vm);
	}
	
	emit loadStepDone(2, valid);
	
	
	return valid;
}

/**
 * [PRIVATE]
 * 
 * ## STEP-3 ##
 * Create all collision domains
 */
bool LabOpener::fetchCds()
{
	bool valid = true;
	
	/* Get the content of lab.conf */
	QFile labConf(labPath + "/lab.conf");
	if (valid && !labConf.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		valid = false;
		qWarning() << "LabOpener::fetchCds()" << "Unable to open lab.conf";
		errorString = "Unable to open lab.conf";
	}
	
	if(valid)	//continue if valid
	{
		QString labConfContent(labConf.readAll());
		
		//Get Cd name HOST_NAME[ETH_NUMBER]="COLLISION_DOMAIN"
		QRegExp cdsRegExp("^[a-zA-Z0-9]+\\[.+\\]=(.+)");
		
		QStringList lines = labConfContent.split("\n", QString::SkipEmptyParts);
		foreach(QString line, lines)
		{
			cdsRegExp.indexIn(line);
			QStringList caps = cdsRegExp.capturedTexts();
			
			// Clear value from sorrounding quotes
			QString value = caps[1];
			if(value.size() >= 2)
			{
				if(value.at(0) == QChar('"') || value.at(0) == QChar('\''))
					value = value.remove(0, 1);
				
				if(value.at(value.size() - 1) == QChar('"') || value.at(value.size() - 1) == QChar('\''))
					value = value.remove(value.size() - 1, 1);
			}
			
			/* Create a new collision domain */
			if(value.trimmed() != "")
			{
				if(!LabFacadeController::getInstance()->getCurrentLab()->cdExist(value))
				{
					CollisionDomain *cd = new CollisionDomain(value);
					LabFacadeController::getInstance()->getCurrentLab()->addCollisionDomain(cd);
				}
			}
		}
	}
	
	emit loadStepDone(3, valid);
	return valid;

}

/**
 * [PRIVATE]
 * 
 * ## STEP-4 ##
 * Create all hardware interfaces
 */
bool LabOpener::fetchHis()
{
	bool valid = true;
	Laboratory *lab = LabFacadeController::getInstance()->getCurrentLab();
	
	/* Get the content of lab.conf */
	QFile labConf(labPath + "/lab.conf");
	if (valid && !labConf.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		valid = false;
		qWarning() << "LabOpener::fetchHis()" << "Unable to open lab.conf";
		errorString = "Unable to open lab.conf";
	}
	
	if(valid)	//continue if valid
	{
		QString labConfContent(labConf.readAll());
		
		//Get Cd name HOST_NAME[ETH_NUMBER]="COLLISION_DOMAIN"
		QRegExp allRegExp("^(.+)\\[(.+)\\]=(.+)");
		
		QStringList lines = labConfContent.split("\n", QString::SkipEmptyParts);
		foreach(QString line, lines)
		{
			allRegExp.indexIn(line);
			QStringList caps = allRegExp.capturedTexts();
			
			if(caps[0] == "")
				continue;
			
			// Clear value from sorrounding quotes on collision domain name
			QString value = caps[3];
			if(value.size() >= 2)
			{
				if(value.at(0) == QChar('"') || value.at(0) == QChar('\''))
					value = value.remove(0, 1);
				
				if(value.at(value.size() - 1) == QChar('"') || value.at(value.size() - 1) == QChar('\''))
					value = value.remove(value.size() - 1, 1);
			}
			
			//get vm and cd to connect by this link/hardware interface
			VirtualMachine *vm = lab->getMachines().value(caps[1]);
			CollisionDomain *cd = lab->getCollisionDomains().value(value);
			if(vm == NULL || cd == NULL)
			{
				valid = false;
				qWarning()	<< "LabOpener::fetchHis()"
							<< "Null virtual machine [" << (vm == NULL)
							<< "] or collision domain [" << (cd == NULL) << "]";
				
				if(vm == NULL)
					errorString = "Unknnown virtual machine " + caps[1];
				if(cd == NULL)
					errorString = "Unknnown collision domain " + value;
			}
			
			//Now, get the state of this hardware interface (default true|up)
			//readeng the startup file
			
			bool linkState = true;
			QFile startupFile(labPath + "/" + vm->getName() + ".startup");
			QRegExp linkStatusRegExp("\\b(up|down)\\b", Qt::CaseSensitive, QRegExp::RegExp2);
						
			if (valid && startupFile.open(QIODevice::ReadOnly | QIODevice::Text))
			{
				QString startupContent = startupFile.readAll(); 
				
				foreach(QString line, startupContent.split("\n", QString::SkipEmptyParts))
				{
					// check only my eth line
					if(!line.contains("eth" + caps[2]))
						continue;
					
					linkStatusRegExp.indexIn(line);

					//get the status
					QString capState = linkStatusRegExp.capturedTexts()[1];
					if(capState != "")
					{
						linkState = (capState == "up");
						break; //stop to the first
					}
				}
			}
			else
				qWarning() << "LabOpener::fetchHis()" << "Unable to open " << vm->getName().append(".startup");
			
			if(valid)		//ok, create the link
			{
				VmFacadeController::getInstance()->createNewHardwareIterface(
						vm, QString("eth").append(caps[2]), linkState, cd);
			}
		}
	}
	
	emit loadStepDone(4, valid);
	
	return valid;	
}

/**
 * [PRIVATE]
 * 
 * ## STEP-5 ##
 * Create all graphics elements and init mappings
 */
bool LabOpener::createGraphicElements()
{
	Laboratory *lab = LabFacadeController::getInstance()->getCurrentLab();
	
	/**
	 * Virtual machine items
	 */
	QListIterator<VirtualMachine*> vmIter = lab->getMachines().values();
	
	while(vmIter.hasNext())
	{
		VirtualMachine *vm = vmIter.next();
		VirtualMachineItem *vmItem = new VirtualMachineItem(vm->getName());
		QString error;
		QPointF vmP = XMLParser::getVmPosition(vm->getName(), labPath, &error);
		
		//XML error
		if(!error.isEmpty())
		{
			errorString.append(error);
			
			emit loadStepDone(5, false);
			return false;
		}
		
		error.clear();
		QPointF vmLabelPos = XMLParser::getVmLabelPosition(vm->getName(), labPath, &error);
		
		//XML error
		if(!error.isEmpty())
		{
			errorString.append(error);
			
			emit loadStepDone(5, false);
			return false;
		}
		
		vmItem->setLabelPos(vmLabelPos);
		
		error.clear();
		QPointF vmMultiLabelPos = XMLParser::getVmPluginsAreaPosition(vm->getName(), labPath, &error);
		
		//XML error
		if(!error.isEmpty())
		{
			errorString.append(error);
			
			emit loadStepDone(5, false);
			return false;
		}
		
		vmItem->setMultiLabelPos(vmMultiLabelPos);
		
		vmItem->setPos(vmP);
		VmMapper::getInstance()->addNewMapping(vmItem, vm);
	}
	
	/**
	 * Collision domain items
	 */
	QListIterator<CollisionDomain*> cdIter = lab->getCollisionDomains().values();
	
	while(cdIter.hasNext())
	{
		CollisionDomain *cd = cdIter.next();
		
		CollisionDomainItem *cdItem = new CollisionDomainItem(cd->getName());
		QString error;
		QPointF cdP = XMLParser::getCdPosition(cd->getName(), labPath, &error);
		
		//XML error
		if(!error.isEmpty())
		{
			errorString.append(error);
			
			emit loadStepDone(5, false);
			return false;
		}
		
		cdItem->setPos(cdP);
		CdMapper::getInstance()->addNewMapping(cdItem, cd);
		
		error.clear();
		QPointF cdLabelPos = XMLParser::getCdLabelPosition(cd->getName(), labPath, &error);
		
		//XML error
		if(!error.isEmpty())
		{
			errorString.append(error);
			
			emit loadStepDone(5, false);
			return false;
		}
		
		cdItem->setLabelPos(cdLabelPos);
		
		error.clear();
		QPointF cdMultiLabelPos = XMLParser::getCdPluginsAreaPosition(cd->getName(), labPath, &error);
		
		//XML error
		if(!error.isEmpty())
		{
			errorString.append(error);
			
			emit loadStepDone(5, false);
			return false;
		}
		
		cdItem->setMultiLabelPos(cdMultiLabelPos);
		
		/**
		 * Connect this Collision domain to all hosts
		 */
		foreach(HardwareInterface *hi, cd->getPeers())
		{
			VirtualMachineItem* vmItemToConnect =
				VmMapper::getInstance()->getVmItem(hi->getMyVirtualMachine());
			
			if(vmItemToConnect == NULL)
			{
				errorString.append("Virtual machine item unknown for virtual machine").append(" ").append(hi->getMyVirtualMachine()->getName());
				emit loadStepDone(5, false);
				break;
			}
			
			/* Make link and connect the mapping */
			LinkItem *link = new LinkItem(vmItemToConnect, cdItem, hi->getName());
			LinkMapper::getInstance()->addNewMapping(link, hi);	
		}
	}
	
	/**
	 * Create areas
	 */
	QString errorXml;
	QList< QMap<QString, QVariant> > areas = XMLParser::getAreas(labPath, &errorXml);
	
	if(!errorXml.isEmpty())
	{
		//some error corrupt
		errorString.append(errorXml);
		emit loadStepDone(5, false);
		return false;
	}
	
	QListIterator< QMap<QString, QVariant> > areasIt(areas);
	while(areasIt.hasNext())
	{
		/*	
		 * QMap
		 * {
		 *	"position"	= QPointF,
		 * 	"width"		= qreal,
		 * 	"height"	= qreal,
		 * 	"color"		= QString,		// "R,G,B"
		 * 	"label"		= QString 
		 * }
		 */
		QMap<QString, QVariant> areaInfo = areasIt.next();

		//Load the area
		AreaController::getInstance()->loadNewArea(areaInfo["position"].toPointF(),
				(qreal)areaInfo["width"].toDouble(),
				(qreal)areaInfo["height"].toDouble(),
				areaInfo["color"].toString(),
				areaInfo["label"].toString());
	}
	
	
	
	//ok, step 5 completed
	emit loadStepDone(5, true);
	
	return true;
}

/**
 * [PRIVATE]
 * 
 * ## STEP-6 ##
 * Create all plugins for each low lever element (vm, cd and hi)
 */
bool LabOpener::loadPlugins()
{
	QString err;
	PluginRegistry *registry = PluginRegistry::getInstance();
	
	/* Load plugins for all hardware interfaces */
	foreach(HardwareInterface *hi, LinkMapper::getInstance()->getHardwareInterfaces())
	{
		//init all plugins for this hardware interface
		QStringList plugins =
			XMLParser::getLinkPlugins(
					hi->getMyVirtualMachine()->getName(), hi->getName(), labPath, &err);
		
		//we have an xml error, stop all
		if(!err.isEmpty())
		{
			errorString.append(err);
			emit loadStepDone(6, false);
			return false;
		}
		
		foreach(QString plugin, plugins)
		{
			PluginProxy *proxy = registry->registerPlugin(plugin, hi);
			
			if(proxy == NULL)	//unknown plugin name
			{
				errorString.append("Unknow plugin name (").append(plugin).append(").");
				emit loadStepDone(6, false);
				return false;
			}
			
			/* Init the plugin */
			proxy->initPlugin(labPath);
			
			/* Add paths offered by pluns to tree */
			QList<QString> tplPaths = proxy->getTemplates().keys();
			// for each plugin and for each key in the templates map
			for (int j=0; j<tplPaths.size(); j++)
			{
				// adds path to tree
				LabHandler::getInstance()->addPathToTree(tplPaths.at(j));
			}
		}
	}

	/* Load plugins for all virtual machines */
	foreach(VirtualMachine *vm, VmMapper::getInstance()->getMachines())
	{
		//init all plugins for this vm
		QStringList plugins =
			XMLParser::getVmPlugins(vm->getName(), labPath, &err);
		
		//we have an xml error, stop all
		if(!err.isEmpty())
		{
			errorString.append(err);
			emit loadStepDone(6, false);
			return false;
		}
		
		foreach(QString plugin, plugins)
		{
			PluginProxy *proxy = registry->registerPlugin(plugin, vm);
			
			if(proxy == NULL)	//unknown plugin name
			{
				errorString.append("Unknow plugin name (").append(plugin).append(").");
				emit loadStepDone(6, false);
				return false;
			}
			
			/* Init the plugin */
			proxy->initPlugin(labPath);
			
			/* Add paths offered by pluns to tree */
			QList<QString> tplPaths = proxy->getTemplates().keys();
			// for each plugin and for each key in the templates map
			for (int j=0; j<tplPaths.size(); j++)
			{
				// adds path to tree
				LabHandler::getInstance()->addPathToTree(tplPaths.at(j));
			}
		}
	}

	/* Load plugins for all collision domains */
	foreach(CollisionDomain *cd, CdMapper::getInstance()->getCDs())
	{
		//init all plugins for this CD
		QStringList plugins =
			XMLParser::getCdPlugins(cd->getName(), labPath, &err);
		
		//we have an xml error, stop all
		if(!err.isEmpty())
		{
			errorString.append(err);
			emit loadStepDone(6, false);
			return false;
		}
		
		foreach(QString plugin, plugins)
		{
			PluginProxy *proxy = registry->registerPlugin(plugin, cd);
			
			if(proxy == NULL)	//unknown plugin name
			{
				errorString.append("Unknow plugin name (").append(plugin).append(").");
				emit loadStepDone(6, false);
				return false;
			}
			
			/* Init the plugin */
			proxy->initPlugin(labPath);
			
			/* Add paths offered by pluns to tree */
			QList<QString> tplPaths = proxy->getTemplates().keys();
			// for each plugin and for each key in the templates map
			for (int j=0; j<tplPaths.size(); j++)
			{
				// adds path to tree
				LabHandler::getInstance()->addPathToTree(tplPaths.at(j));
			}
		}
	}

	emit loadStepDone(6, true);
	
	LabHandler::getInstance()->setSaveLabState(true);
	LabHandler::getInstance()->setChangedLabState(false);
	
	
	return true;
}
