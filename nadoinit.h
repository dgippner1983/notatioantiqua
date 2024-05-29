/*-----------------------------------------------------------------------------
|  This file is part of Notatio Antiqua (c) 2009-2012 David Gippner           |
-------------------------------------------------------------------------------

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; version 3 of the License.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

-----------------------------------------------------------------------------*/
#ifndef NADOINIT_H
#define NADOINIT_H

#include <QThread>
#include <QDirIterator>
#include <QSettings>
#include <QMessageBox>
class nadoinit : public QThread
{
    Q_OBJECT
private:
    void run();
public:
    QStringList LaTeX_Path, Lilypond_Path, Gregorio_Path, templateFolder, dictFolder, helpFolder;
    QString basePath, currentPath, endText;
    bool isinit;
    void setValues(QString path);
signals:
    QString pathHasChanged(QString path);

};

#endif // NADOINIT_H
