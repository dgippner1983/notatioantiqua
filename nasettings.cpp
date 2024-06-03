/*-----------------------------------------------------------------------------
|  This file is part of Notatio Antiqua (c) 2009-2024 David Gippner           |
-------------------------------------------------------------------------------

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; version 3 of the License.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

-----------------------------------------------------------------------------*/

#include <QtDebug>
#include <QDesktopServices>
#include "nasettings.h"
#include "ui_nasettings.h"

NASettings::NASettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NASettings)
{
    ui->setupUi(this);
    QSettings* preferences = new QSettings(QSettings::IniFormat,QSettings::UserScope,"DGSOFTWARE", "Notatio Antiqua");
    preferences->beginGroup("Paths");
    ui->latexE->setText(preferences->value("latexPath").toString());
    ui->gregorioE->setText(preferences->value("gregorioPath").toString());
    ui->lilypondE->setText(preferences->value("lilypondPath").toString());
    ui->templateE->setText(preferences->value("templateFolder").toString());
    ui->dictionariesE->setText(preferences->value("dictFolder").toString());
    ui->helpE->setText(preferences->value("helpFolder").toString());
    preferences->endGroup();
    preferences->beginGroup("Font");
    QFont prefFont;
    QFont docsFont;
    QFont docssFont;
    QFont doccsFont;
    QFont docinitFont;
    docsFont.setFamily(preferences->value("docserifFont").toString());
    docssFont.setFamily(preferences->value("docsansFont").toString());
    doccsFont.setFamily(preferences->value("doccsFont").toString());
    docinitFont.setFamily(preferences->value("docinitFont").toString());
    prefFont.setFamily(preferences->value("standardFont").toString());
    ui->NAfont->setCurrentFont(prefFont);
    ui->sfontSize->setValue(preferences->value("standardSize").toInt());
    ui->staffsizeBox->setValue(preferences->value("staffsize").toInt());
    ui->fontsizeBox->setValue(preferences->value("fontsize").toInt());
    ui->serifCB->setCurrentFont(docsFont);
    ui->sansserifCB->setCurrentFont(docssFont);
    ui->csfCB->setCurrentFont(doccsFont);
    ui->initialsCB->setCurrentFont(docinitFont);
    preferences->endGroup();
    preferences->beginGroup("Colors");
    keyword = preferences->value("keywords").value<QColor>();
    pitch = preferences->value("pitches").value<QColor>();
    commentary = preferences->value("commentaries").value<QColor>();
    translation = preferences->value("translations").value<QColor>();
    preferences->endGroup();
    connect(this,SIGNAL(accepted()),this,SLOT(modifyIni()));
}

NASettings::~NASettings()
{
    delete ui;
}

void NASettings::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void NASettings::modifyIni()
{
    QSettings* preferences = new QSettings(QSettings::IniFormat,QSettings::UserScope,"DGSOFTWARE", "Notatio Antiqua");
    preferences->beginGroup("Paths");
    preferences->setValue("latexPath",ui->latexE->text());
    preferences->setValue("gregorioPath",ui->gregorioE->text());
    preferences->setValue("lilypondPath",ui->lilypondE->text());
    preferences->setValue("templateFolder",ui->templateE->text());
    preferences->setValue("dictFolder",ui->dictionariesE->text());
    preferences->setValue("helpFolder",ui->helpE->text());
    preferences->endGroup();
    preferences->beginGroup("Font");
    QFont saveFont;
    saveFont = ui->NAfont->currentFont();
    preferences->setValue("standardFont",saveFont.family());
    preferences->setValue("standardSize",ui->sfontSize->value());
    preferences->setValue("staffsize",ui->staffsizeBox->value());
    preferences->setValue("fontsize",ui->fontsizeBox->value());
    preferences->setValue("docserifFont",ui->serifCB->currentFont().family());
    preferences->setValue("docsansFont",ui->sansserifCB->currentFont().family());
    preferences->setValue("docinitFont",ui->initialsCB->currentFont().family());
    preferences->setValue("doccsFont",ui->csfCB->currentFont().family());
    preferences->endGroup();
    preferences->beginGroup("Colors");
    preferences->setValue("keywords",keyword);
    preferences->setValue("pitches",pitch);
    preferences->setValue("commentaries",commentary);
    preferences->setValue("translations",translation);
    preferences->endGroup();
}

void NASettings::on_keywordsCC_clicked()
{
    keyword = QColorDialog::getColor(keyword,this,tr("Choose Keyword Color"),QColorDialog::ShowAlphaChannel);
    if (!keyword.isValid()) keyword = Qt::darkBlue;
}

void NASettings::on_pitchesCC_clicked()
{
    pitch = QColorDialog::getColor(pitch,this,tr("Choose Pitch Color"),QColorDialog::ShowAlphaChannel);
    if (!pitch.isValid()) pitch = Qt::darkRed;
}

void NASettings::on_commentsCC_clicked()
{
    commentary = QColorDialog::getColor(commentary,this,tr("Choose Pitch Color"),QColorDialog::ShowAlphaChannel);
    if (!commentary.isValid()) commentary = Qt::darkGreen;
}

void NASettings::on_translationsCC_clicked()
{
    translation = QColorDialog::getColor(translation,this,tr("Choose Pitch Color"),QColorDialog::ShowAlphaChannel);
    if(!translation.isValid()) translation = Qt::darkYellow;
}
