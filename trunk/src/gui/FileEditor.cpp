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

#include "FileEditor.h"
#include "../common/CommonConfigs.h"

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
	
	//Connections
	connect(wrapCheck, SIGNAL(stateChanged(int)),
			this, SLOT(changeWrapMode(int)));
	
}

/**
 * Deconstructor
 */
FileEditor::~FileEditor()
{
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
