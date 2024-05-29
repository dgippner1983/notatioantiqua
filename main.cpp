/*-----------------------------------------------------------------------------
|  This file is part of Notatio Antiqua (c) 2009-2024 David Gippner           |
-------------------------------------------------------------------------------

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; version 3 of the License.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

-----------------------------------------------------------------------------*/

#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QtSvg>
#include <QFont>
#include "naprog.h"
#include "version.h"
#include "naapplication.h"

int main(int argc, char *argv[])
{
    naapplication a(argc, argv);
    // Installing Translations
    // 1 QT generic
    QTranslator* qtTrans = new QTranslator;
    if (qtTrans->load("qt_" + QLocale::system().name(), QLibraryInfo::path(QLibraryInfo::TranslationsPath)))
    qApp->installTranslator(qtTrans);
    // 2 NotatioAntiqua
    QTranslator* NATrans = new QTranslator;
    QString locale = QString(":/NaProg-%1").arg(QLocale::system().name());
    if (NATrans->load(locale))
    qApp->installTranslator(NATrans);
    // initializing application font
    QStringList ubuntus;
    ubuntus << "Ubuntu-B.ttf" << "Ubuntu-M.ttf" << "UbuntuMono-BI.ttf" << "Ubuntu-BI.ttf" << "Ubuntu-MI.ttf" << "UbuntuMono-R.ttf"
            << "Ubuntu-C.ttf" << "Ubuntu-R.ttf" << "UbuntuMono-RI.ttf" << "Ubuntu-L.ttf" << "Ubuntu-RI.ttf" << "Ubuntu-LI.ttf"
            << "UbuntuMono-B.ttf";
    int uID(-1);
    bool fontWarn(false);
    for (QStringList::const_iterator constIterator = ubuntus.constBegin(); constIterator != ubuntus.constEnd(); ++constIterator) {
        QFile res(":/fonts/font/" + *constIterator);
        if (res.open(QIODevice::ReadOnly) == false) {
            if (fontWarn == false) {
                QMessageBox::warning(0, "Notatio Antiqua", (QString)"Impossible to open " + QChar(0x00AB) + " Ubuntu Font " + QChar(0x00BB) + ".");
                fontWarn = true;
            }
        } else {
            uID = QFontDatabase::addApplicationFontFromData(res.readAll());
            if (uID == -1 && fontWarn == false) {
                QMessageBox::warning(0, "Notatio Antiqua", (QString)"Impossible to open " + QChar(0x00AB) + " Ubuntu Font " + QChar(0x00BB) + ".");
                fontWarn = true;
            }
        }
    }
    QFont appfont("Ubuntu", 12); // application general font
    QFile nastyle(":/notantiq.css"); // get included styles from central stylesheet
    nastyle.open(QFile::ReadOnly);
    QString nastyle_content (nastyle.readAll());
    naapplication::setOrganizationName("DGSOFTWARE");
    naapplication::setOrganizationDomain("http://www.gippner.eu");
    naapplication::setApplicationName("Notatio Antiqua");
    naapplication::setApplicationVersion(Version::versionString());
    naapplication::setFont(appfont);
    NaProg w;
    w.setStyleSheet(nastyle_content);
    w.showMaximized();
    return a.exec();
}
