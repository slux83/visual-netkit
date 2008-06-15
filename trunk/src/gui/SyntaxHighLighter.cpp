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

#include "SyntaxHighLighter.h"

/**
 * Constructor
 */
SyntaxHighLighter::SyntaxHighLighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
	singleLineCommentRegExp = QRegExp("#[^\n]*");
	singleLineCommentFormat.setForeground(Qt::blue);
}

/**
 * Deconstructor
 */
SyntaxHighLighter::~SyntaxHighLighter()
{
}

/**
 * [PROTECTED-REIMPL]
 * Highlight a text block
 */
void SyntaxHighLighter::highlightBlock(const QString &text)
{
	int index = text.indexOf(singleLineCommentRegExp);
	while(index >= 0)
	{
		int length = singleLineCommentRegExp.matchedLength();
		setFormat(index, length, singleLineCommentFormat);
		index = text.indexOf(singleLineCommentRegExp, index + length);
	}
	
	//setCurrentBlockState(0);
	
//    foreach (HighlightingRule rule, highlightingRules) {
//        QRegExp expression(rule.pattern);
//        int index = text.indexOf(expression);
//        while (index >= 0) {
//            int length = expression.matchedLength();
//            setFormat(index, length, rule.format);
//            index = text.indexOf(expression, index + length);
//        }
//    }
//
//    setCurrentBlockState(0);
//
//    int startIndex = 0;
//    if (previousBlockState() != 1)
//        startIndex = text.indexOf(commentStartExpression);
//
//
//    while (startIndex >= 0)
//    {
//        int endIndex = text.indexOf(commentEndExpression, startIndex);
//        int commentLength;
//        if (endIndex == -1) {
//            setCurrentBlockState(1);
//            commentLength = text.length() - startIndex;
//        } else {
//            commentLength = endIndex - startIndex
//                            + commentEndExpression.matchedLength();
//        }
//        setFormat(startIndex, commentLength, multiLineCommentFormat);
//        startIndex = text.indexOf(commentStartExpression,
//                                                startIndex + commentLength);
//    }
}
