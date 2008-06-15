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

#ifndef SYNTAXHIGHLIGHTER_H_
#define SYNTAXHIGHLIGHTER_H_

#include <QSyntaxHighlighter>

/**
 * This class contains rules for syntax highlighting
 */
class SyntaxHighLighter : public QSyntaxHighlighter
{
	Q_OBJECT

private:
	QTextCharFormat singleLineCommentFormat;
	QRegExp singleLineCommentRegExp;

public:
	SyntaxHighLighter(QTextDocument *parent = 0);
	virtual ~SyntaxHighLighter();

protected:
	void highlightBlock(const QString &text);

};

#endif /*SYNTAXHIGHLIGHTER_H_*/
