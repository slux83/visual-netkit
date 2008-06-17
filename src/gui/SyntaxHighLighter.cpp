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
	HighlightingRule rule;
	
	/* Single line comment */
	singleLineCommentRegExp = QRegExp("#[^\n]*");
	singleLineCommentFormat.setForeground(Qt::blue);
	
	rule.format = singleLineCommentFormat;
	rule.pattern = singleLineCommentRegExp;
	
	highlightingRules.append(rule);
	
	/* Ipv4 address */
	ipv4RegExp = QRegExp("\\b(?:1\\d?\\d?|2(?:[0-4]\\d?|[6789]|5[0-5]?)?|[3-9]\\d?|0)(?:\\.(?:1\\d?\\d?|2(?:[0-4]\\d?|[6789]|5[0-5]?)?|[3-9]\\d?|0)){3}\\b", 
						Qt::CaseSensitive, QRegExp::RegExp2);
	
	/* Mac address */
	macAddressRegExp = QRegExp("\\b([0-9A-Fa-f]{2}:){5}[0-9A-Fa-f]{2}\\b");
	
	/* Network format */
	networkFormat.setForeground(Qt::darkMagenta);
	networkFormat.setFontItalic(true);
	networkFormat.setFontWeight(QFont::Bold);
	
	rule.format = networkFormat;
	rule.pattern = ipv4RegExp;
	highlightingRules.append(rule);
	
	rule.pattern = macAddressRegExp;
	highlightingRules.append(rule);
	
	/* Keywords */
	keywordFormat.setForeground(Qt::darkBlue);
	keywordFormat.setFontWeight(QFont::Bold);
	
	QStringList keywordPatterns;
	keywordPatterns	<< "\\bnetmask\\b" << "\\bbroadcast\\b" << "\\bup\\b"
					<< "\\bdown\\b" << "\\bifconfig\\b" << "\\bhw\\b"
					<< "\\bether\\b";
	
	foreach (QString pattern, keywordPatterns)
	{
		rule.pattern = QRegExp(pattern);
		rule.format = keywordFormat;
		highlightingRules.append(rule);
	}
	
	/* lab.conf rules */
	labInfoFormat.setFontWeight(QFont::Bold);
	labInfoFormat.setFontItalic(true);
	
	QStringList labPatterns;
	labPatterns << "\\bLAB_DESCRIPTION\\b" << "\\bLAB_VERSION\\b" << "\\bLAB_AUTHOR\\b"
				<< "\\bLAB_EMAIL\\b" << "\\bLAB_WEB\\b";
	
	foreach (QString pattern, labPatterns)
	{
		rule.pattern = QRegExp(pattern);
		rule.format = labInfoFormat;
		highlightingRules.append(rule);
	}
	
	hostsFormat.setForeground(Qt::darkRed);
	hostsFormat.setFontWeight(QFont::Bold);
	hostsRegExp = QRegExp("\\b[a-zA-Z0-9]+\\[([0-9]|[1-9][0-9])\\]");
	
	rule.pattern = hostsRegExp;
	rule.format = hostsFormat;
	
	highlightingRules.append(rule);
	
	/* String quota */
	stringQuoteRegExp = QRegExp("\".*\"");
	stringQuoteFormat.setForeground(Qt::darkGreen);
	rule.pattern = stringQuoteRegExp;
	rule.format = stringQuoteFormat;
	
	highlightingRules.append(rule);	
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
	foreach(HighlightingRule rule, highlightingRules)
	{
		QRegExp expr(rule.pattern);
		
		int index = text.indexOf(expr);
		while(index >= 0)
		{
			int length = expr.matchedLength();
			setFormat(index, length, rule.format);
			index = text.indexOf(expr, index + length);
		}
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
