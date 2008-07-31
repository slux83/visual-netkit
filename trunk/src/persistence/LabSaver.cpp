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
#include "../plugin_framework/PluginProxy.h"

/**
 * Constructor.
 * 
 * Path is the directory where to save the lab root folder.
 */
LabSaver::LabSaver(const QString &path, bool backup)
{
	curPath = path;
	needBackup = backup;
	excludedPaths = LabHandler::getInstance()->getExcludePaths();
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
	
	if (allok && !createFolderSystem())		// creates folders
		allok = false;
	if (allok && !saveLabConf())			// saves lab.conf file
		allok = false;
	if (allok && !saveStartups())			// saves .startup files
		allok = false;
	if (allok && !saveTemplates())			// saves plugin templates
		allok = false;
	
	if(allok)
		removeUnusedStuff();
	
	// remove the lab
	if(!allok)
	{
		QDir rootDir;
		QString labPath = curPath ;
		if(rootDir.exists(labPath))
		{
			//allok = rootDir.rmdir(labPath);
			allok = removeDir(labPath);
			if(!allok)
				errorString = "Cannot remove lab dir '" + labPath + "'.";
		}
		else 
		{
			errorString = "Cannot remove lab dir '" + curPath + "':\n"+
						  "directory does not exist.";
		}
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
	
	/* exclude file? */
	if(excludedPaths.contains(LAB_CONF))
		return true;
	
	QFile file(curPath + "/" + LAB_CONF);
	
	/* Create a backup file */
	if(file.exists() && needBackup)
	{
		//Clear old backup
		if(QFile::exists(curPath + "/" + LAB_CONF + "~"))
			QFile::remove(curPath + "/" + LAB_CONF + "~");
		
		QFile::rename(curPath + "/" + LAB_CONF, curPath + "/" + LAB_CONF + "~");
	}
	
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
	
	// save all startups template
	while(machineIterator.hasNext())
	{
		machineIterator.next();
		
		/* Exclude this file? */
		if(excludedPaths.contains(machineIterator.key() + ".startup"))
			continue;
		
		QFile startup(curPath + "/" + machineIterator.key() + ".startup");
		
		/* Save a backup copy */
		if(startup.exists() && needBackup)
		{
			//Clear old backup
			if(QFile::exists(curPath + "/" + machineIterator.key() + ".startup~"))
				QFile::remove(curPath + "/" + machineIterator.key() + ".startup~");
			
			QFile::rename(curPath + "/" + machineIterator.key() + ".startup",
					curPath + "/" + machineIterator.key() + ".startup~");
		}

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
		
		/* Get the template content */
		QByteArray tplContent = TemplateExpert::template2string(QString::fromUtf8(":/tpl/startup"));
		
		QTextStream out(&startup);
		QApplication::setOverrideCursor(Qt::WaitCursor);
		
		out << tplContent;	//write file header
		
		QApplication::restoreOverrideCursor();
	}
	
	return returnVal;
}

/**
 * Saves all templates files if any
 */
bool LabSaver::saveTemplates()
{
	bool allok = true;
	
	QList<PluginProxy*> plugins = PluginRegistry::getInstance()->getAllProxies();
	
	//This list contains a list of allready backuped files.
	//In this way we can know when rename (backup) a template and when we can't.
	QStringList backupedFiles;

	// for each proxy
	for (int p=0; p<plugins.size(); p++)
	{		
		// get all the templates to save
		QMapIterator<QString, QString> tplIterator(plugins.at(p)->getTemplates());
		
		// for each template to be saved
		while (tplIterator.hasNext())
		{
			tplIterator.next();
			
			/* Exclude this file? */
			if(excludedPaths.contains(tplIterator.key()))
				continue;
			
			QStringList tplPathList = tplIterator.key().split("/");
			tplPathList.removeLast();
			QString tplPath = tplPathList.join("/");
			QString tplName = tplIterator.key().split("/").last();
			
			if (tplName.isEmpty())
			{
				qWarning() << "LabSaver::saveTemplates: error - null tplPath or tplName";
				qWarning() << "tplPath = " << tplPath;
				qWarning() << "tplName = " << tplName;
				return false;
			}
			QString path = curPath + "/" + tplPath;
			
			QFile tpl(path + "/" + tplName);
			QDir rootDir;
			
			// if current template exists on filesystem, append its content
			if(tpl.exists())
			{	
				/* Create a backup copy (.startup files are backuped by LabSaver::saveStartups() */
				if(!backupedFiles.contains(path + "/" + tplName) && needBackup
						&& !tpl.fileName().contains(QRegExp("startup$")))
				{
					backupedFiles.append(path + "/" + tplName);	//store filename
					
					//Clear old backup
					if(QFile::exists(path + "/" + tplName + "~"))
						QFile::remove(path + "/" + tplName + "~");
						
					QFile::rename(path + "/" + tplName,
							path + "/" + tplName + "~");
				}
				
				if(!tpl.open(QFile::Append))
				{
					qWarning()	<< "Cannot append content to file" << tplIterator.key() << ":" << tpl.errorString();
					errorString = "Cannot append content to file " + tplIterator.key() + tpl.errorString();
					return false;
				}
				QTextStream out(&tpl);
				QApplication::setOverrideCursor(Qt::WaitCursor);
				out << tplIterator.value();
				QApplication::restoreOverrideCursor();
			}
			else 
			{				
				// creates file and write the template content
				rootDir.mkpath(path);
				
				if (!tpl.open(QFile::WriteOnly | QFile::Text))
				{
					qWarning()	<< "Cannot write file" << tplIterator.key() << ":" << tpl.errorString();
					errorString = "Cannot write file " + tplIterator.key() + tpl.errorString();
					return false;
				}
				QTextStream out(&tpl);
				QApplication::setOverrideCursor(Qt::WaitCursor);
				out << tplIterator.value();
				QApplication::restoreOverrideCursor();
			}
		}
	}
	
	/* Store the ethX state at the end of each .startup files */
	QMapIterator<QString, VirtualMachine*> machineIterator(currentLab->getMachines());
	while(machineIterator.hasNext())
	{
		machineIterator.next();
		
		/* Exclude this file? */
		if(excludedPaths.contains(machineIterator.key() + ".startup"))
			continue;
		
		QFile startup(curPath + "/" + machineIterator.key() + ".startup");
		
		if(!startup.open(QFile::Append))
		{
			qWarning()	<< "Cannot append eth state inside startup file"
						<< machineIterator.key() + ".startup"
						<< ":" << startup.errorString();
			errorString = "Cannot append eth state inside startup file " +
							machineIterator.key() + ".startup: " +
							startup.errorString();
			return false;
		}
		
		/* Get the template content */
		QString ifconfig = "/sbin/ifconfig <HI> <HI_STATE> ## <COMMENT>\n";
		
		QTextStream out(&startup);
		QApplication::setOverrideCursor(Qt::WaitCursor);
		
		/* foreach link in this machine */
		foreach(HardwareInterface *hi, machineIterator.value()->getInterfaces().values())
		{
			QString ifConfigCopy = ifconfig;
			QString lineComment = QString("'").append(hi->getMyCollisionDomain()->getName()).append("' collision domain ##");
			ifConfigCopy.replace("<HI>", hi->getName());
			ifConfigCopy.replace("<HI_STATE>", (hi->getState())? "up" : "down");
			ifConfigCopy.replace("<COMMENT>", lineComment);
			
			out << ifConfigCopy.toUtf8();	//write interface status
		}
		QApplication::restoreOverrideCursor();
	}
	
	return allok;
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
		if(!rootDir.exists(curPath))
		{
			allok = rootDir.mkdir(curPath);
			if(!allok)
				errorString = "Cannot create root dir '" + curPath + "'.";
		}
		
		QMapIterator<QString, VirtualMachine*> machineIterator(currentLab->getMachines());
		
		/* iterates on machines */
		while(machineIterator.hasNext() && allok)
		{
			machineIterator.next();
			if(!rootDir.exists(curPath + "/" + machineIterator.key()))
				allok = rootDir.mkdir(curPath + "/" + machineIterator.key());
			
			if (!allok)
				errorString = "Cannot create dir '" + curPath + "/" + machineIterator.key() + "'.";
		}
	} 
	else 
	{
		errorString = "Current lab is NULL!";
		return false;
	}
	
	return allok;
}


/**
 * [PRIVATE]
 * Removes passed dir and contained files.
 * NOTE: this function is copied from www.qtcentre.org/forum
 */
bool LabSaver::removeDir(const QString d)
{
	bool ret = true;
	QDir dir(d);

	if (dir.exists())
	{
		const QFileInfoList list = dir.entryInfoList();
		QFileInfo fi;
		for (int l = 0; l < list.size(); l++)
		{
			fi = list.at(l);
			if (fi.isDir() && fi.fileName() != "." && fi.fileName() != "..")
				ret = removeDir(fi.absoluteFilePath());
			else if (fi.isFile())
			{
				QFile f( fi.absoluteFilePath() );
				ret = f.remove();
				if (!ret)
					qWarning() << "Can't remove: " << fi.absoluteFilePath() << " (write-protect?)";
			}
		}
		dir.rmdir(d);
	}
	return ret;
}

/**
 * [PRIVATE]
 * Remove the dirs and files unused
 * @return a list of deleted machines/dirs
 */
QStringList LabSaver::removeUnusedStuff()
{
	QMap<QString, VirtualMachine*> activeMachines =
		currentLab->getMachines();
	
	QStringList removedDirs;
	
	/* 
	 * Now scan the lab root dir and delete unused machines
	 * (with files that match MACHINE_NAME*)
	 */
	QDir labRoot = QDir(curPath);
	labRoot.setFilter(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
	
	//search unused dirs
	foreach(QString dir, labRoot.entryList())
	{
		if(!activeMachines.contains(dir))
		{
			/* To delete:
			 *  - lab-path/machine/				(recoursively)
			 *  - lab-path/machine.startup		(if exist)
			 *  - lab-path/machine.shutdown		(if exist)
			 */
			
			removeDir(labRoot.absolutePath().append('/').append(dir));
			
			if(QFile::exists(labRoot.absolutePath().append('/').append(dir).append(".startup")))
				QFile::remove(labRoot.absolutePath().append('/').append(dir).append(".startup"));
			
			if(QFile::exists(labRoot.absolutePath().append('/').append(dir).append(".shutdown")))
				QFile::remove(labRoot.absolutePath().append('/').append(dir).append(".shutdown"));
				
			removedDirs << dir;
		}
	}
	
	return removedDirs;
	
}

/**
 * The strippedName() function call around curFile shortens
 * the file name to exclude the path.
 */
QString LabSaver::strippedName(const QString &fullFileName)
{
	return QFileInfo(fullFileName).fileName();
}


