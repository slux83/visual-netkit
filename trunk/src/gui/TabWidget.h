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

#ifndef TABWIDGET_H_
#define TABWIDGET_H_

#include <QTabWidget>
#include <QToolButton>
#include <QDebug>
#include <QMenu>

#include "handles/TabController.h"

/**
 * This is a QTabWidget with more features
 */
class TabWidget : public QTabWidget
{
	Q_OBJECT
	
private:
	QToolButton *closeTabButton, *tabListButton;
	QMenu *listTabMenu;

private slots:
	void showSelectedTab(QAction* action);
	
public slots:
	void updateTabList();
	void closeTab();
	
public:
	TabWidget(QWidget * parent = 0);
	virtual ~TabWidget();
};

#endif /*TABWIDGET_H_*/

