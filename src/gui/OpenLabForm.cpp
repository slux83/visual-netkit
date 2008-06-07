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
	
	checkBox_1->setChecked(false);
	checkBox_1->setEnabled(false);
	
	checkBox_2->setChecked(false);
	checkBox_2->setEnabled(false);
	
	checkBox_3->setChecked(false);
	checkBox_3->setEnabled(false);
	
	checkBox_4->setChecked(false);
	checkBox_4->setEnabled(false);
	
	checkBox_5->setChecked(false);
	checkBox_5->setEnabled(false);
	
	checkBox_6->setChecked(false);
	checkBox_6->setEnabled(false);
	
	checkBox_7->setChecked(false);
	checkBox_7->setEnabled(false);
	
	checkBox_8->setChecked(false);
	checkBox_8->setEnabled(false);
}

/**
 * [PRIVATE-SLOT]
 * Handle user confirm
 */
void OpenLabForm::handleAccept()
{
	QString labDir = labPathLineEdit->text();
	LabOpener labOpener(labDir);
}
