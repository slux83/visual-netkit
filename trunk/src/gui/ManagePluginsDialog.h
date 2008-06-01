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

#ifndef MANAGEPLUGINSDIALOG_H_
#define MANAGEPLUGINSDIALOG_H_

#include <QDebug>
#include <QWidget>
#include <QGraphicsItemGroup>
#include "ui_managePlugins.h"
class PluginLoaderFactory;

/**
 * With this dialog the user can manage plugins for a base element
 */
class ManagePluginsDialog : public QDialog, public Ui::ManagePluginsGui
{
	Q_OBJECT
	
private:
	QGraphicsItemGroup *baseElement;
	QList<PluginLoaderFactory*> allPlugins;
	QStringList elemPlugins;
	void clearPluginsList();
	QStringList getAddedPlugins();
	QStringList getDeletedPlugins();

private slots:
	void showPluginInfos(QListWidgetItem *);
	void handleUserConfirm();
	
public:
	ManagePluginsDialog(QGraphicsItemGroup *bElement, QWidget *parent = 0);
	virtual ~ManagePluginsDialog();
	void buildGui();
};

#endif /*MANAGEPLUGINSDIALOG_H_*/
