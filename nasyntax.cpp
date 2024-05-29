/*-----------------------------------------------------------------------------
|  This file is part of Notatio Antiqua (c) 2009-2024 David Gippner           |
-------------------------------------------------------------------------------

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software Foundation;
version 3 of the License.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

-----------------------------------------------------------------------------*/

// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "nasyntax.h"

//! [0]
Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkGreen);
    keywordFormat.setFontWeight(QFont::Bold);
    const QString keywordPatterns[] = {
                                       QStringLiteral("\\blilypondfile\\b"), QStringLiteral("\\bdocumentclass\\b"), QStringLiteral("\\busepackage\\b"),
                                       QStringLiteral("\\bdef\\b"), QStringLiteral("\\bnewcommand\\b"), QStringLiteral("\\bhskip\\b"),
                                       QStringLiteral("\\bbegin\\b"), QStringLiteral("\\bend\\b"), QStringLiteral("\\bbook\\b"),
                                       QStringLiteral("\\boccasion\\b"), QStringLiteral("\\bname\\b"), QStringLiteral("\\bgabc-copyright\\b"),
                                       QStringLiteral("\\bscore-copyright\\b"), QStringLiteral("\\boffice-part\\b"), QStringLiteral("\\bmeter\\b"),
                                       QStringLiteral("\\barranger\\b"), QStringLiteral("\\bgabc-version\\b"), QStringLiteral("\\bauthor\\b"),
                                       QStringLiteral("\\bdate\\b"), QStringLiteral("\\bmanuscript\\b"), QStringLiteral("\\bmanuscript-reference\\b"),
                                       QStringLiteral("\\bmanuscript-storage-place\\b"), QStringLiteral("\\btranscriber\\b"), QStringLiteral("\\btranscription-date\\b"),
                                       QStringLiteral("\\bgregoriotex-font\\b"), QStringLiteral("\\bmode\\b"), QStringLiteral("\\binitial-style\\b"),
                                       QStringLiteral("\\buser-notes\\b"), QStringLiteral("\\bannotation\\b"), QStringLiteral("\\baiintro\\b"),
                                       QStringLiteral("\\bueberinitiale\\b"),QStringLiteral("\\bcommentary\\b"),QStringLiteral("\\bsources\\b"),QStringLiteral("\\bAbar\\b"), QStringLiteral("\\bVbar\\b")
    };
    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
        //! [0] //! [1]
    }
    //! [1]

    //! [2]
    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression(QStringLiteral("\\bQ[A-Za-z]+\\b"));
    rule.format = classFormat;
    highlightingRules.append(rule);
    //! [2]

    //! [3]
    singleLineCommentFormat.setForeground(Qt::red);
    rule.pattern = QRegularExpression(QStringLiteral("%%[^\n]*"));
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    //! [3]

    //! [4]
    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression(QStringLiteral("\".*\""));
    rule.format = quotationFormat;
    highlightingRules.append(rule);
    //! [4]

    //! [5]
    gabcCodeFormat.setForeground(Qt::darkRed);
    rule.pattern = QRegularExpression(QStringLiteral("(?<=\\()[^)]*"));
    rule.format = gabcCodeFormat;
    highlightingRules.append(rule);

    nabcCodeFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression(QStringLiteral("\\|([A-Za-z0-9].*)\\b[*^)]"));
    rule.format = nabcCodeFormat;
    highlightingRules.append(rule);

    spcharFormat.setForeground(Qt::darkBlue);
    spcharFormat.setFontWeight(QFont::Bold);

    translationFormat.setForeground(Qt::darkMagenta);
    translationFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression(QStringLiteral("\\[([A-Za-z0-9].*)\\b[*^\\]]"));
    rule.format = translationFormat;
    highlightingRules.append(rule);
    //! [5]

    //! [6]
    spcharStartExpression = QRegularExpression(QStringLiteral("<([A-Za-z][A-Za-z0-9]*)\\b[^>]*>(.*?)"));
    spcharEndExpression = QRegularExpression(QStringLiteral("</([A-Za-z][A-Za-z0-9]*)\\b[^>]*>"));
}
//! [6]

//! [7]
void Highlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : std::as_const(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    //! [7] //! [8]
    setCurrentBlockState(0);
    //! [8]

    //! [9]
    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(spcharStartExpression);

    //! [9] //! [10]
    while (startIndex >= 0) {
        //! [10] //! [11]
        QRegularExpressionMatch match = spcharEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + match.capturedLength();
        }
        setFormat(startIndex, commentLength, spcharFormat);
        startIndex = text.indexOf(spcharStartExpression, startIndex + commentLength);

    }
}
//! [11]
