/*-----------------------------------------------------------------------------
|  This file is part of Notatio Antiqua (c) 2009-2012 David Gippner           |
-------------------------------------------------------------------------------

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; version 3 of the License.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

-----------------------------------------------------------------------------*/

#ifndef NASETTINGS_H
#define NASETTINGS_H

#include <QDialog>
#include <QColor>
#include <QColorDialog>
#include <QSettings>
#include <QMessageBox>
#include <QColorDialog>

namespace Ui {
    class NASettings;
}

class NASettings : public QDialog {
    Q_OBJECT
    /**
     * @class NASettings
     * @brief Settings class, holds some variables for the settings dialog and the dialog itself
     */
public:
    NASettings(QWidget *parent = 0);
    ~NASettings();
    QColor keyword;
    QColor commentary;
    QColor pitch;
    QColor translation;
    QString fontsize;
    QString staffsize;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::NASettings *ui;
private slots:
    void modifyIni();
    void on_keywordsCC_clicked();
    void on_pitchesCC_clicked();
    void on_commentsCC_clicked();
    void on_translationsCC_clicked();
};

#endif // NASETTINGS_H
