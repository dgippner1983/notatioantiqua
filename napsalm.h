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
#ifndef NAPSALM_H
#define NAPSALM_H

#include <QFrame>
#include <QTextStream>
#include <QMultiMap>
#include <QDesktopServices>
#include <QFile>
#include <QMessageBox>
#include <QSettings>
#include <QFileDialog>
#include "hyphen.h"

namespace Ui {
class napsalm;
}

class napsalm : public QFrame
/**
     * @class napsalm
     * @brief Class for holding the information on psalm tone models
     */
{
    Q_OBJECT
    
public:
    explicit napsalm(QWidget *parent = 0);
    ~napsalm();
    
private slots:
    void on_toneCBm_currentIndexChanged(int index);
    void preparation();
    void gettone();

    void on_toneCBm_currentIndexChanged(const QString &arg1);

    void on_toneVariant_currentIndexChanged(const QString &arg1);

    void on_toneSolemn_stateChanged(int arg1);

    void on_closeButton_toggled(bool checked);

    void on_closeButton_clicked();

    void on_psalmText_textChanged();

    void on_generateButton_clicked();

private:
    Ui::napsalm *ui;
    struct NaPsalmTone {
        QString tone;
        QString clef;
        QString initium;
        QString isolemn;
        QString tuba;
        QString flex;
        QString mediant;
        QString msolemn;
        QMap< QString, QString > termination;
        };
    QList<NaPsalmTone> tones;
    HyphenDict *dict;
};

#endif // NAPSALM_H
