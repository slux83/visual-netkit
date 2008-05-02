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

#include "LabSaver.h"

#include "../common/CommonConfigs.h"
#include "../common/LabConf.h"
#include "../gui/handles/LabHandler.h"
#include "../core/CollisionDomain.h"

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

    qDebug() << "File 'lab.conf' saved";
	
	return allok;
}

/**
 * Prepares and returns lab.conf file text.
 * Returns an empty string if no current laboratory is set.
 */
QString LabSaver::prepareLabConfText()
{
	QString text("");
	
	if (currentLab != NULL)
	{
		text += LAB_DESCRIPTION + currentLab->getDescription() + "\n";
		text += LAB_VERSION + currentLab->getVersion() + "\n";
		text += LAB_AUTHOR + currentLab->getAuthors() + "\n";
		text += LAB_EMAIL + currentLab->getEmail() + "\n";
		text += LAB_WEB + currentLab->getWebsite() + "\n";
		
		QMap<QString, VirtualMachine *> vmsmap = currentLab->getMachines();
		QList<QString> vmsnames = vmsmap.keys();
		
		QMap<QString, CollisionDomain *> cdsmap = currentLab->getCollisionDomains();
		QList<QString> cdsnames = cdsmap.keys();
		
		for (int i=0; i < vmsnames.size(); i++)
		{
			VirtualMachine *vm = vmsmap.value(vmsnames.at(i));
			
			QString rname("unknown");
			rname = vm->getName();
			
			QMap<QString, HardwareInterface *> itfsmap = vm->getInterfaces();
			QList<QString> itfsnames = itfsmap.keys();
			
			for (int j=0; j < itfsnames.size(); j++) 
			{
				QString itf("unknown");
				itf = itfsnames.at(j);
				
				QString cd("unknown");
				HardwareInterface *hi = itfsmap.value(itfsnames.at(j));
				
				cd = hi->getMyCollisionDomain()->getName();
				
				QString str(ROUTER_TPL);
				str.replace("router", rname);
				str.replace("interface", itf);
				str.replace("collisiondomain", cd);
				
				text += str + "\n";
			}
		}
	}
	
	return text;
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
	
	//LabHandler::getInstance()->getMainWindow()->labTree. ;
	
	return allok;
}



