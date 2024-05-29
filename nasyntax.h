/*-----------------------------------------------------------------------------
|  This file is part of Notatio Antiqua (c) 2009-2024 David Gippner           |
-------------------------------------------------------------------------------

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; version 3 of the License.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

-----------------------------------------------------------------------------*/


// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef NASYNTAX_H
#define NASYNTAX_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

//! [0]
class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QList<HighlightingRule> highlightingRules;

    QRegularExpression spcharStartExpression;
    QRegularExpression spcharEndExpression;
    QRegularExpression translationStartExpression;
    QRegularExpression translationEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat gabcCodeFormat;
    QTextCharFormat spcharFormat;
    QTextCharFormat translationFormat;
    QTextCharFormat nabcCodeFormat;
    QTextCharFormat quotationFormat;
};
//! [0]

#endif // NASYNTAX_H

