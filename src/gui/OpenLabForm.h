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

#ifndef OPENLABFORM_H_
#define OPENLABFORM_H_

#include <QFileDialog>
#include <QDir>
#include <QMessageBox>

#include "../persistence/LabOpener.h"
#include "ui_openLab.h"

/**
 * This QDialog is showed to the user when he want open a lab
 */
class OpenLabForm : public QDialog, public Ui::OpenLabUi
{
	Q_OBJECT

private:
	LabOpener *labOpener;
	QMap<int, QCheckBox*> steps;
	void initStepMap();
	
private slots:
	void browseLab();
	void handleAccept();
	void markLoadStep(int step, bool result);
	
public:
	OpenLabForm(QWidget *parent = 0);
	virtual ~OpenLabForm();
	void resetGui();
};

#endif /*OPENLABFORM_H_*/
