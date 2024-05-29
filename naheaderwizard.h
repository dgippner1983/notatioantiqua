/*-----------------------------------------------------------------------------
|  This file is part of Notatio Antiqua (c) 2009-2012 David Gippner           |
-------------------------------------------------------------------------------

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; version 3 of the License.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

-----------------------------------------------------------------------------*/
#ifndef NAHEADERWIZARD_H
#define NAHEADERWIZARD_H

#include <QWizard>


namespace Ui {
    class NAHeaderWizard;
}

class NAHeaderWizard : public QWizard
{
    Q_OBJECT

public:
    explicit NAHeaderWizard(QWidget *parent = 0);
    ~NAHeaderWizard();
    QStringList header;


private slots:
    void accept();

private:
    Ui::NAHeaderWizard *ui;
};

#endif // NAHEADERWIZARD_H
