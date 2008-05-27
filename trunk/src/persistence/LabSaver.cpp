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

#include <QDir>

#include "LabSaver.h"

#include "../common/CommonConfigs.h"
#include "../gui/handles/LabHandler.h"
#include "../core/CollisionDomain.h"
#include "TemplateExpert.h"

#include "../plugin_framework/PluginRegistry.h"

/**
 * Constructor.
 * 
 * Path is the directory where to save the lab root folder.
 */
LabSaver::LabSaver(const QString & path)
{
	curPath = path;
}

LabSaver::~LabSaver()
{
}

/**
 * Saves current lab to filesystem.
 */
bool LabSaver::saveLab()
{
	bool allok = true;
	
	currentLab = LabFacadeController::getInstance()->getCurrentLab();
	
	if (allok && !createFolderSystem())
		allok = false;
	if (allok && !saveLabConf())
		allok = false;
	if (allok && !saveStartups())
		allok = false;
	if (allok && !saveVmsConf())
		allok = false;
	
	/* remove the lab */
	if(!allok)
	{
		//TODO: clear the junk lab 
	}
	
	return allok;
}

/** 
 * [PRIVATE]
 * Saves current lab configuration to lab.conf file.
 */
bool LabSaver::saveLabConf()
{
	bool allok = true;
	
	QFile file(curPath + "/" + currentLab->getName() + "/" + LAB_CONF);
	if (!file.open(QFile::WriteOnly | QFile::Text))
    {
    	qWarning() << "Cannot write file" << LAB_CONF << ":" << file.errorString();
        errorString = "Cannot write file " + LAB_CONF + ": " + file.errorString();
    	return false;
    }

	QTextStream out(&file);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	out << prepareLabConfText();
	QApplication::restoreOverrideCursor();
	
	return allok;
}

/**
 * [PRIVATE]
 * Save all startup files if any
 */
bool LabSaver::saveStartups()
{
	bool returnVal = true;
	QMapIterator<QString, VirtualMachine*> machineIterator(currentLab->getMachines());
	
	/* save all startups */
	while(machineIterator.hasNext())
	{
		machineIterator.next();
		QFile startup(curPath + "/" + currentLab->getName() + "/" + machineIterator.key() + ".startup");

		if(!startup.open(QFile::WriteOnly | QFile::Text))
		{
			qWarning()	<< "Cannot write startup file"
						<< machineIterator.key() + ".startup"
						<< ":" << startup.errorString();
			errorString = "Cannot write startup file " +
							machineIterator.key() + ".startup: " +
							startup.errorString();
			return false;
		}
		
		QTextStream out(&startup);
		QApplication::setOverrideCursor(Qt::WaitCursor);
		out << prepareStartupText(machineIterator.value());
		QApplication::restoreOverrideCursor();
	}
	
	return returnVal;
}

/**
 * [PRIVATE]
 * Prepare and return the string representing the startup file for vm
 */
QString LabSaver::prepareStartupText(VirtualMachine *vm)
{
	QString startupText = TemplateExpert::template2string(QString::fromUtf8(":/tpl/startup"));
	QString ifconfigContent;
	
	/* Reg expressions */
	QRegExp ifconfigReg("<IFCONFIG>(.+)</IFCONFIG>");
	QRegExp zebraReg("<ZEBRA>(.+)</ZEBRA>");
	
	/* Get the ifconfig line */
	ifconfigReg.indexIn(startupText);
	QString ifconfigLine = ifconfigReg.cap(1);
	
	/* iterate all interfaces */
	QListIterator<HardwareInterface*> ethIterator(vm->getInterfaces().values());
	while(ethIterator.hasNext())
	{
		HardwareInterface *hi = ethIterator.next();
		QString temp = ifconfigLine;	//clone line
		QString status;
		
		/* replace infos */
		temp.replace("<ETH_NAME>", hi->getName());
		
		(hi->getState())? status = "up" : status = "down";
		temp.replace("<STATUS>", status);
		
		/* append */
		ifconfigContent.append(temp + "\n");
	}
	
	/* Replace the ifconfig content inside <IFCONFIG> tags */
	startupText.replace(ifconfigReg, ifconfigContent);

	return startupText;
	
}

/**
 * [PRIVATE]
 * Prepares and returns lab.conf file text.
 * Returns an empty string if no current laboratory is set.
 */
QString LabSaver::prepareLabConfText()
{	
	QByteArray labConf;
	
	if (currentLab != NULL)
	{
		/* Get the base template */
		labConf.append(TemplateExpert::template2string(QString::fromUtf8(":/tpl/lab")));
		
		/* Replace Lab informations */
		labConf.replace(QString("<DESCRIPTION>"), QByteArray().append(currentLab->getDescription()));
		labConf.replace(QString("<VERSION>"), QByteArray().append(currentLab->getVersion()));
		labConf.replace(QString("<AUTHOR>"), QByteArray().append(currentLab->getAuthors()));
		labConf.replace(QString("<EMAIL>"), QByteArray().append(currentLab->getEmail()));
		labConf.replace(QString("<WEB>"), QByteArray().append(currentLab->getWebsite()));
	
		/* Replace and build lab.conf content */
		QMapIterator<QString, VirtualMachine*> machineIterator(currentLab->getMachines());
		
		/* Get topology content */
		QRegExp hostReg("<TOPOLOGY>(.+)</TOPOLOGY>");
		hostReg.indexIn(labConf);
		QString hostDef = hostReg.cap(1);
		QByteArray labTopology;
		
		/* iterate machines */
		while(machineIterator.hasNext())
		{
			QString topologyLine(hostDef);
			machineIterator.next();
			
			//replace hostname
			topologyLine.replace(QString("<HOST>"), machineIterator.key());
			
			QMapIterator<QString, HardwareInterface*> ethIterator(machineIterator.value()->getInterfaces());

			/* Iterate interfaces */
			while(ethIterator.hasNext())
			{
				QString tmp(topologyLine);
				ethIterator.next();

				QString ethNumber = ethIterator.key();
				ethNumber.replace("eth", "");
				tmp.replace(QString("<ETH_NUMBER>"), ethNumber);
				tmp.replace(QString("<COLLISION_DOMAIN_NAME>"), 
						ethIterator.value()->getMyCollisionDomain()->getName());

				labTopology.append(tmp + "\n");	
			}
			
			//separe machines definition
			labTopology.append("\n");
			
		}
		labConf = QString(labConf).replace(hostReg, labTopology).toUtf8();
	}
	
	return labConf;
}

/**
 * The strippedName() function call around curFile shortens
 * the file name to exclude the path.
 */
QString LabSaver::strippedName(const QString &fullFileName)
{
	return QFileInfo(fullFileName).fileName();
}

/**
 * Saves passed router or host configuration to filesystem.
 */
bool LabSaver::saveVmsConf()
{
	bool allok = true;
	QMapIterator<QString, VirtualMachine*> machineIterator(currentLab->getMachines());
	
	/* iterates on machines */
	while(machineIterator.hasNext() && allok)
	{
		machineIterator.next();
		
	//	qDebug() << PluginRegistry::getInstance()->getVmProxy(machineIterator.value())->getPlugin()->getName();
		
		//TODO controllare l'esistenza dei plugin...se nn lo si fa segmentation fault!
	//	qDebug() << PluginRegistry::getInstance()->getVmProxy(machineIterator.value())->getPluginProperties();
	}	
	return allok;
}

/**
 * Creates the lab folders as appearing in the lab tree in the Main GUI Window.
 * This function creates both routerX.conf file and routerX folder
 * (including subdirectories and subfiles).
 */
bool LabSaver::createFolderSystem()
{
	bool allok = true;
	QDir rootDir;
	
	if (currentLab != NULL)
	{
		// creates main lab dir and check if it's created
		if(!rootDir.exists(curPath + "/" + currentLab->getName()))
		{
			allok = rootDir.mkdir(curPath + "/" + currentLab->getName());
			if(!allok)
				errorString = "Cannot create root dir '" + curPath + "/" + currentLab->getName() + "'.";
		}
		
		QMapIterator<QString, VirtualMachine*> machineIterator(currentLab->getMachines());
		
		/* iterates on machines */
		while(machineIterator.hasNext() && allok)
		{
			machineIterator.next();
			if(!rootDir.exists(curPath + "/" + currentLab->getName() + "/" + machineIterator.key()))
				allok = rootDir.mkdir(curPath + "/" + currentLab->getName() + "/" + machineIterator.key());
			
			if (!allok)
				errorString = "Cannot create dir '" + curPath + "/" + currentLab->getName() + "/" + machineIterator.key() + "'.";
		}
	} 
	else 
	{
		errorString = "Current lab is NULL!";
		return false;
	}
	
	return allok;
}
