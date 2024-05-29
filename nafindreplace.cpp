/*-----------------------------------------------------------------------------
|  This file is part of Notatio Antiqua (c) 2009-2024 David Gippner           |
-------------------------------------------------------------------------------

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software Foundation;
version 3 of the License.
This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

-----------------------------------------------------------------------------*/
#include "nafindreplace.h"
#include "ui_nafindreplace.h"

nafindreplace::nafindreplace(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::nafindreplace)
{
    ui->setupUi(this);
    connect(ui->findB,SIGNAL(clicked()),this,SLOT(find()));
    connect(ui->replaceB,SIGNAL(clicked()),this,SLOT(replace()));
    connect(ui->replaceAllB,SIGNAL(clicked()),this,SLOT(replaceAll()));
}

nafindreplace::~nafindreplace()
{
    delete ui;
}
void nafindreplace::find()
{
    emit findText(ui->findE->text(),
                  ui->caseRB->isChecked(),
                  ui->wordsRB->isChecked(),
                  ui->backwardsRB->isChecked());
}
void nafindreplace::replace()
{
    emit replaceText(ui->findE->text(),
                     ui->replaceE->text(),
                     ui->caseRB->isChecked(),
                     ui->wordsRB->isChecked(),
                     ui->backwardsRB->isChecked(),
                     false);
}
void nafindreplace::replaceAll()
{
    emit replaceText(ui->findE->text(),
                     ui->replaceE->text(),
                     ui->caseRB->isChecked(),
                     ui->wordsRB->isChecked(),
                     ui->backwardsRB->isChecked(),
                     true);
}
void nafindreplace::on_replaceE_textChanged(const QString &arg1)
{
    if (arg1.isEmpty()) {
        ui->replaceB->setEnabled(false);
        ui->replaceAllB->setEnabled(false);
    } else {
    ui->replaceB->setEnabled(true);
    ui->replaceAllB->setEnabled(true);
    }
}

void nafindreplace::on_findE_textChanged(const QString &arg1)
{
    if (arg1.isEmpty()) ui->findB->setEnabled(false);
    else ui->findB->setEnabled(true);
}
