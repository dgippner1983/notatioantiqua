/*-----------------------------------------------------------------------------
|  This file is part of Notatio Antiqua (c) 2009-2012 David Gippner           |
-------------------------------------------------------------------------------

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; version 3 of the License.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

-----------------------------------------------------------------------------*/
#ifndef NACLEFSELECT_H
#define NACLEFSELECT_H

#include <QDialog>
namespace Ui {
    class NAClefSelect;
}

class NAClefSelect : public QDialog
{
    Q_OBJECT

public:
    explicit NAClefSelect(QWidget *parent = 0);
    ~NAClefSelect();
    QString clefS;

private:
    Ui::NAClefSelect *ui;

private slots:
    void on_buttonBox_accepted();
};

#endif // NACLEFSELECT_H
