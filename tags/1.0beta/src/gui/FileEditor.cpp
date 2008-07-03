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

#include "FileEditor.h"
#include "../common/CommonConfigs.h"
#include "handles/TabController.h"

/**
 * Constructor
 */
FileEditor::FileEditor(QString &fileContent, QWidget *parent) : QWidget(parent)
{
	setupUi(this);
	
	//set syntax highlighter
	highlighter = new SyntaxHighLighter(fileTextEdit->document());
	
	//Set font
	QFont font = FILE_EDITOR_FONT;
	font.setFixedPitch(true);
	
	fileTextEdit->setFont(font);
	
	//Set the contend inside the qtextarea
	fileTextEdit->setPlainText(fileContent);
	
	//Connection: wraph
	connect(wrapCheck, SIGNAL(stateChanged(int)),
			this, SLOT(changeWrapMode(int)));
	
	//Connection: save document
	connect(saveButton, SIGNAL(clicked()),
			this, SLOT(saveChangedDocument()));
	
	//Connection: mark modified
	connect(fileTextEdit, SIGNAL(textChanged()),
			this, SLOT(markModified()));
	
	//Connection: mark modified
	connect(fileTextEdit, SIGNAL(undoAvailable(bool)),
			this, SLOT(undoIsAvailable(bool)));
}

/**
 * Deconstructor
 */
FileEditor::~FileEditor()
{
	delete highlighter;
}

/**
 * [PRIVATE-SLOT]
 * Change the wrap mode of file text editor
 */
void FileEditor::changeWrapMode(int state)
{
	Q_UNUSED(state)
	
	if(wrapCheck->isChecked())
	{
		fileTextEdit->setLineWrapMode(QTextEdit::WidgetWidth);
	}
	else
	{
		fileTextEdit->setLineWrapMode(QTextEdit::NoWrap);
	}
}

/**
 * [PRIVATE-SLOT]
 * Save handler
 */
void FileEditor::saveChangedDocument()
{
	QString error;

	if(!TabController::getInstance()->saveFile(this, &error))
	{
		QMessageBox::warning(this, "VisualNetkit - error",
			tr("There was an error during the document save!") +
			"\n\nError: " + error,
			QMessageBox::Ok, QMessageBox::Ok);
	}
	else
	{
		saveButton->setDisabled(true);
		TabController::getInstance()->markTabAsModified(this, false);
	}
}

/**
 * [PRIVATE-SLOT]
 * Set tab as modified
 */
void FileEditor::markModified()
{
	TabController::getInstance()->markTabAsModified(this, true);
	saveButton->setDisabled(false);
}

/**
 * [PRIVATE-SLOT]
 * Set tab as modified
 */
void FileEditor::undoIsAvailable(bool state)
{
	TabController::getInstance()->markTabAsModified(this, state);
}

