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

#include "TabController.h"
#include "LabHandler.h"

/* Init the controller NULL instance */
TabController* TabController::instance = NULL;

/**
 * Constructor
 */
TabController::TabController() : QObject()
{
	//Get the tab widget from main gui
	tabWidget = LabHandler::getInstance()->getMainWindow()->centralTabWidget;
	connect(this, SIGNAL(tabsHasChanged()), tabWidget, SLOT(updateTabList()));
}

/**
 * Deconstructor
 */
TabController::~TabController()
{
}

/**
 * Singletone get instance
 */
TabController* TabController::getInstance()
{
	if (instance == NULL)
	{
		instance = new TabController();
	}

	return instance;
}

/**
 * Open the tab passed the file path; if the tab doesn't exist, create it.
 * The argument must be a full path
 */
bool TabController::openTab(QString path)
{
	if(activeTabs.contains(path))
	{
		tabWidget->setCurrentWidget(activeTabs.value(path));
		return true;
	}
	else
	{
		/* Read the file content */
		QFile file(path);
		
		if(!file.exists())
			return false;
		
		if(!file.open(QFile::ReadOnly))
			return false;
		
		QString fileContent = file.readAll();
		file.close();
		
		/* get the file name */
		QStringList splittedPath = path.split("/", QString::SkipEmptyParts);
		
		if(splittedPath.size() == 0)
			return false;
		
		/* Create a new file editor and focus it */
		FileEditor *fileEditor = new FileEditor(fileContent);
		
		tabWidget->addTab(fileEditor, splittedPath.last());
		activeTabs.insert(path, fileEditor);
		
		tabWidget->setCurrentWidget(fileEditor);
	}
	
	emit tabsHasChanged();	//emit signal
	
	return true;
}

/**
 * Destroy widget and clear from my list
 */
void TabController::removeTab(FileEditor* fileEditor)
{
	QString key = activeTabs.key(fileEditor);
	activeTabs.remove(key);
	
	delete fileEditor;
	
	emit tabsHasChanged();	//emit signal
}

/**
 * Save the changed content inside the passed file editor
 * If false is returned and a QString *error is passed,
 * this will be filled with an error description
 */
bool TabController::saveFile(FileEditor* fileEditor, QString *error)
{
	//Check consistance
	QString path = activeTabs.key(fileEditor); 
	QString pathBkp = path + "~";
	if(path == "")
	{
		qWarning() << "TabController::saveFile() file editor unknown";
		if(error != NULL)
			error->append(tr("Hey this is a BUG: file editor unmapped!"));
		return false;
	}
	
	QFile fileToSave(path);
	
	/* We need to save a backup? */
	if(fileEditor->needBackup())
	{
		//delete old bkp
		if(QFile::exists(pathBkp) && !QFile::remove(pathBkp))
		{
			qWarning()	<< "TabController::saveFile() Cannot remove the old backup" << pathBkp;
			if(error != NULL)
				error->append(tr("Cannot remove the old backup file ").append(pathBkp));
			
			return false;
		}
		
		if(!QFile::copy(path, pathBkp))
		{
			qWarning()	<< "TabController::saveFile() Cannot create the backup copy for file" << path;
			if(error != NULL)
				error->append(tr("Cannot create the backup copy for file ").append(path));
			
			return false;
		}
	}
	
	/* Ok, we can save the document */
	if(!fileToSave.open(QFile::WriteOnly | QFile::Text))
	{
		qWarning()	<< "TabController::saveFile() Cannot open the file" << path << "in write mode."
					<< fileToSave.errorString();
		if(error != NULL)
			error->append(fileToSave.errorString());
		
		return false;
	}
	QTextStream out(&fileToSave);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	out << fileEditor->fileTextEdit->toPlainText().toUtf8();
	QApplication::restoreOverrideCursor();
	
	fileToSave.close();

	
	return true;
}
