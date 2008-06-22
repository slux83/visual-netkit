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

#ifndef TABCONTROLLER_H_
#define TABCONTROLLER_H_

#include <QObject>
#include <QTabWidget>
#include <QString>
#include <QMap>

#include "../FileEditor.h"

class TabWidget;

/**
 * This class is the handler/controller for main tab widget.
 * It can create new tabs, manage them and interact with the user`s actions
 */
class TabController : public QObject
{
	Q_OBJECT

private:
	static TabController* instance;
	TabWidget *tabWidget;
	
	//map: key=FilePath value=Widget file editor
	QMap<QString, FileEditor*> activeTabs;
	
public:
	TabController();
	virtual ~TabController();
	static TabController* getInstance();
	bool openTab(QString path);
	void removeTab(FileEditor* fileEditor);
	bool saveFile(FileEditor* fileEditor, QString *error = NULL);
	void closeAllTabs();

signals:
	void tabsHasChanged();
};

#endif /*TABCONTROLLER_H_*/
