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

#include "OpenLabForm.h"

/**
 * Constructor
 */
OpenLabForm::OpenLabForm(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	
	//init steps
	initStepMap();
	
	labOpener = new LabOpener();
	
	//connects
	connect(labOpener, SIGNAL(loadStepDone(int, bool)),
			this, SLOT(markLoadStep(int, bool)));
	connect(browseButton, SIGNAL(clicked()),
			this, SLOT(browseLab()));
	connect(buttonBox, SIGNAL(accepted()),
			this, SLOT(handleAccept()));
	
}

/**
 * Deconstructor
 */
OpenLabForm::~OpenLabForm()
{
	delete labOpener;
}

/**
 * [PRIVATE-SLOT]
 * Open the file dialog to get the path of the lab to open
 */
void OpenLabForm::browseLab()
{
	QString labDir = QFileDialog::getExistingDirectory(this, tr("Open Laboratory"),
                         QDir::homePath(),
                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	
	labPathLineEdit->setText(labDir);
}

/**
 * Reset the gui
 */
void OpenLabForm::resetGui()
{
	labPathLineEdit->clear();
	
	checkBox_0->setCheckState(Qt::Unchecked);
	checkBox_0->setEnabled(false);
	
	checkBox_1->setCheckState(Qt::Unchecked);
	checkBox_1->setEnabled(false);
	
	checkBox_2->setCheckState(Qt::Unchecked);
	checkBox_2->setEnabled(false);
	
	checkBox_3->setCheckState(Qt::Unchecked);
	checkBox_3->setEnabled(false);
	
	checkBox_4->setCheckState(Qt::Unchecked);
	checkBox_4->setEnabled(false);
	
	checkBox_5->setCheckState(Qt::Unchecked);
	checkBox_5->setEnabled(false);
	
	checkBox_6->setCheckState(Qt::Unchecked);
	checkBox_6->setEnabled(false);
	
	checkBox_7->setCheckState(Qt::Unchecked);
	checkBox_7->setEnabled(false);
	
	checkBox_8->setCheckState(Qt::Unchecked);
	checkBox_8->setEnabled(false);
}

/**
 * [PRIVATE-SLOT]
 * Handle user confirm
 */
void OpenLabForm::handleAccept()
{
	QString labDir = labPathLineEdit->text();
	labOpener->setPath(labDir);
	labOpener->open();
	
}

/**
 * [PRIVATE]
 * Init the checkbox map
 */
void OpenLabForm::initStepMap()
{
	steps.insert(0, checkBox_0);
	steps.insert(1, checkBox_1);
	steps.insert(2, checkBox_2);
	steps.insert(3, checkBox_3);
	steps.insert(4, checkBox_4);
	steps.insert(5, checkBox_5);
	steps.insert(6, checkBox_6);
	steps.insert(7, checkBox_7);
	steps.insert(8, checkBox_8);
	
}

/**
 * [PRIVATE-SLOT]
 * Mark the step passed with the result
 */
void OpenLabForm::markLoadStep(int step, bool result)
{
	if(!steps.contains(step))
	{
		qWarning() << "OpenLabForm::markLoadStep step" << step << "don't exist";
		return;
	}
	
	if(result)
		steps.value(step)->setCheckState(Qt::Checked);
	else
	{
		steps.value(step)->setCheckState(Qt::PartiallyChecked);
		
		QMessageBox::warning(this, tr("Visual Netkit - ERROR"),
				tr("There was an error during laboratory opening:") + "\n\n" + labOpener->getErrorString(),
				QMessageBox::Ok, QMessageBox::Ok);
		return;
	}
}
