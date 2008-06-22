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

#include <QMessageBox>

#include "TabWidget.h"
#include "FileEditor.h"

/**
 * Constructor
 */
TabWidget::TabWidget(QWidget *parent) : QTabWidget(parent)
{
	/* Init the close tab button */
	closeTabButton = new QToolButton(this);
	closeTabButton->setIcon(QIcon(":/menu/close_icon"));
	closeTabButton->setToolTip(tr("Close the current Tab"));
	
	/* Init the tab list bottun */
	listTabMenu = new QMenu(this);
	tabListButton = new QToolButton(this);
	tabListButton->setIcon(QIcon(":/menu/tab_list_icon"));
	tabListButton->setToolTip(tr("Show tab list"));
	tabListButton->setMenu(listTabMenu);
	tabListButton->setPopupMode(QToolButton::InstantPopup);
	
	setCornerWidget(closeTabButton, Qt::TopRightCorner);
	setCornerWidget(tabListButton, Qt::TopLeftCorner);
	
	connect(closeTabButton, SIGNAL(clicked()), this, SLOT(closeTab()));
	
	/* connect action */
	connect(tabListButton, SIGNAL(triggered(QAction*)), this, SLOT(showSelectedTab(QAction*)));
}

/**
 * Deconstructor
 */
TabWidget::~TabWidget()
{
	delete closeTabButton;
	delete tabListButton;
}

/**
 * [SLOT]
 * Close tab handler
 */
void TabWidget::closeTab()
{
	/* Don't touch the graph tab */
	if(currentIndex() == 0)
	{
		return;
	}
	
	/* Get the current widget */
	QWidget *tab;
	if((tab = currentWidget()) == NULL)
		return;
	
	/* Subcasting */
	FileEditor *fe = static_cast<FileEditor*>(tab);
	
	if(fe->textHasChanged())
	{
		int resp = QMessageBox::question(this, "VisualNetkit - question",
				tr("The document has been modified. Do you want to save it before closing?"),
				QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);
		
		if(resp == QMessageBox::Cancel)
			return;
		
		//Save the changed document
		if(resp == QMessageBox::Yes)
		{
			QString error;
			if(!TabController::getInstance()->saveFile(fe, &error))
			{
				int forceClose = QMessageBox::warning(this, "VisualNetkit - warning",
						tr("There was an error during the document save. Do you want to force the tab closing?") +
						"\n\nError: " + error,
						QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
				
				if(forceClose == QMessageBox::No)
					return;
			}
		}
	}
	
	//remove tab from tab-stack
	removeTab(currentIndex());
	
	TabController::getInstance()->removeTab(fe);
}

/**
 * [PUBLIC-SLOT]
 * Build and update the tab menu
 */
void TabWidget::updateTabList()
{
	/* Clear and rebuild menu */
	listTabMenu->clear();
	
	for(int i=0; i<count(); i++)
	{
		QAction *action = new QAction(tabText(i), listTabMenu);
		action->setData(i);		//save the tab index
		
		/* Set appropriate icon */
		if(i==0)
			action->setIcon(QIcon(":/small/graph"));
		else
			action->setIcon(QIcon(":/small/file_conf"));
		
		listTabMenu->addAction(action);
	}
}

/**
 * [PRIVATE-SLOT]
 * Show the selected tab on tabs list
 */
void TabWidget::showSelectedTab(QAction* action)
{
	setCurrentIndex(action->data().toInt());
}
