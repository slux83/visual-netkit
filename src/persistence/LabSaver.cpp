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

LabSaver::LabSaver()
{
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
	
	if (!saveLabConf())
		allok = false;
	if (!saveRoutersConf())
		allok = false;
	if (!createFolderSystem())
		allok = false;
	
	return allok;
}

/**
 * Saves current lab configuration to lab.conf file.
 */
bool LabSaver::saveLabConf()
{
	bool allok = true;
	
	QFile file(LAB_CONF);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        qWarning() << this << "Cannot write file " << LAB_CONF << file.errorString();
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << prepareLabConfText();
    QApplication::restoreOverrideCursor();
	
	return allok;
}

/**
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
				
				//TODO: extract only the ethernet number
				tmp.replace(QString("<ETH_NUMBER>"), ethIterator.key());
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
 * The strippedName() function call around curFile shortens the file name to exclude the path.
 */
QString LabSaver::strippedName(const QString &fullFileName)
{
	return QFileInfo(fullFileName).fileName();
}

/**
 * Saves passed router or host configuration to filesystem.
 * This function creates both routerX.conf file and routerX folder (including subdirectories and subfiles).
 */
bool LabSaver::saveRoutersConf()
{
	bool allok = true;
	return allok;
}

/**
 * Creates the lab folders as appearing in the lab tree in the Main GUI Window.
 */
bool LabSaver::createFolderSystem()
{
	bool allok = true;
	
	if (currentLab != NULL)
	{
		// create current lab main directory
		QDir maindir("");
		QString curpath = maindir.absolutePath();
		//maindir.setCurrent(curpath);
		
		// creates main lab dir and check if it's created
		allok = maindir.mkdir(currentLab->getName());
		
		// current folder is now maindir
		maindir.setCurrent(curpath +"/"+ currentLab->getName());
		qDebug() << maindir.absolutePath();
		
		QMapIterator<QString, VirtualMachine*> machineIterator(currentLab->getMachines());
		
		/* iterates on machines */
		while(machineIterator.hasNext())
		{
			machineIterator.next();
			maindir.mkdir(machineIterator.key());
		}
	} 
	else 
	{
		return false;
	}
	
	return allok;
}

